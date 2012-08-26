#include <vector>
#include <boost/filesystem.hpp>
#include <script_manager.hpp>

namespace pikia {
    void script_manager::bind (lua_State* _state) {
        luabind::module (_state, "pikia") [
            luabind::class_<script_manager> ("script_manager")
                .def ("register_handler", &script_manager::register_handler)
                .def ("unregister_handler", &script_manager::unregister_handler)
                .def ("dispatch_job", &script_manager::dispatch_job)
        ];
    }

    script_manager::script_manager (std::shared_ptr<configuration> _config, std::shared_ptr<job_dispatcher> _jobDispatcher)
        : config (_config), jobDispatcher (_jobDispatcher) {
        this->setup ();
    }

    script_manager::~script_manager () {
        this->close ();
    }

    void script_manager::setup () {
        boost::lock_guard<boost::mutex> lock (this->luaMutex);

        std::cout << "Setting up lua..." << std::endl;

        this->lua = luaL_newstate ();
        luaL_openlibs (this->lua);

        // setup some bindings
        luabind::open (this->lua);

        job_buffer::bind (this->lua);
        job_context::bind (this->lua);
        script_manager::bind (this->lua);

        luabind::globals (this->lua) ["script_manager"] = this;

        // load the scripts
        int result;
        boost::filesystem::path luaExt (".lua");
        for (boost::filesystem::recursive_directory_iterator end, dir (this->config->get<std::string> ("scripts.path") ); dir != end; ++dir) {
            if (boost::filesystem::is_regular_file (*dir) && (*dir).path ().extension () == luaExt) {
                std::cout << "Loading lua script: " << (*dir) << std::endl;
                result = luaL_dofile (this->lua, (*dir).path ().string ().c_str () );

                if (result > 0) {
                    std::cout << "Error loading script: " << (*dir) << std::endl;
                }
            }
        }
    }

    void script_manager::close () {
        boost::lock_guard<boost::mutex> lock (this->luaMutex);

        std::cout << "Shutting down lua..." << std::endl;

        lua_close (this->lua);
    }

    bool script_manager::register_handler (uint32_t _id, const luabind::object& _callback) {
        if (luabind::type (_callback) == LUA_TFUNCTION) {
            std::shared_ptr<lua_job_handler> handler (new script_manager::lua_job_handler (this, _callback) );
            return this->jobDispatcher->register_handler (_id, std::function<bool (job_context&)> (std::bind<bool> (&script_manager::lua_job_handler::operator(), handler, std::placeholders::_1) ) );
        }
        std::cout << "Error: The given lua object is not a function" << std::endl;
        return false;
    }

    void script_manager::unregister_handler (uint32_t _id) {
        this->jobDispatcher->unregister_handler (_id);
    }

    bool script_manager::dispatch_job (job_context& _context) {
        return this->jobDispatcher->dispatch_job (_context);
    }

    script_manager::lua_job_handler::lua_job_handler (script_manager* _manager, const luabind::object& _callback)
        : callback (_callback) {
        this->manager = _manager;
    }

    bool script_manager::lua_job_handler::operator() (job_context& _context) {
        boost::lock_guard<boost::mutex> lock (this->manager->luaMutex);
        return luabind::call_function<bool> (this->callback, boost::ref (_context) );
    }
}
