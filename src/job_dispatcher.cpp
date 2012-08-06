#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <job_dispatcher.hpp>

namespace pikia {
    void job_dispatcher::bind (lua_State* _state) {
        luabind::module (_state, "pikia") [
            luabind::class_<job_dispatcher> ("job_dispatcher")
                .def ("register_handler", (bool (job_dispatcher::*) (uint32_t, const luabind::object&) ) &job_dispatcher::register_handler)
        ];
    }

    job_dispatcher::job_dispatcher () {
        this->setup_lua ();
    }

    job_dispatcher::~job_dispatcher () {
        // callbacks must be destroyed before lua is unloaded because they contain references to lua objects
        this->close_lua ();
    }

    bool job_dispatcher::register_handler (uint32_t _id, boost::function<bool (job_context&)> _callback) {
        std::cout << "Registering callback for job ID: " << _id << std::endl;
        if (this->jobHandlers.find (_id) == this->jobHandlers.end () ) {
            this->jobHandlers[_id] = _callback;
            return true;
        }
        return false;
    }

    bool job_dispatcher::register_handler (uint32_t _id, const luabind::object& _callback) {
        std::cout << "Registering lua function for job ID: " << _id << std::endl;
        if (luabind::type (_callback) == LUA_TFUNCTION) {
            boost::shared_ptr<lua_job_handler> handler (new job_dispatcher::lua_job_handler (_callback) );
            return register_handler (_id, boost::function<bool (job_context&)> (boost::bind<bool> (&job_dispatcher::lua_job_handler::operator(), handler, _1) ) );
        }
        std::cout << "Error: The given lua object is not callable" << std::endl;
        return false;
    }

    bool job_dispatcher::dispatch_job (job_context& _context) {
        boost::mutex::scoped_lock lock (this->luaMutex);

        std::cout << "Dispatching event..." << std::endl;
        std::map<uint32_t, boost::function<bool (job_context&)> >::iterator it = this->jobHandlers.find (_context.id);
        if (it != this->jobHandlers.end () ) {
            try {
                return (*it).second (_context);
            }
            catch (const std::exception& e) {
                std::cout << "Error dispatching job: " << e.what () << std::endl;
            }
        }
        return false;
    }

    void job_dispatcher::reload () {
        this->close_lua();
        this->setup_lua();
    }

    void job_dispatcher::setup_lua () {
        boost::mutex::scoped_lock lock (this->luaMutex);

        std::cout << "Setting up lua..." << std::endl;

        this->lua = luaL_newstate ();
        luaL_openlibs (this->lua);

        // setup some bindings
        luabind::open (this->lua);

        job_buffer::bind (this->lua);
        job_context::bind (this->lua);
        job_dispatcher::bind (this->lua);

        luabind::globals (this->lua) ["dispatcher"] = this;

        // load the scripts
        int result;
        boost::filesystem::path luaExt (".lua");
        for (boost::filesystem::recursive_directory_iterator end, dir ("./scripts"); dir != end; ++dir) {
            if (boost::filesystem::is_regular_file (*dir) && (*dir).path ().extension () == luaExt) {
                std::cout << "Loading lua script: " << (*dir) << std::endl;
                result = luaL_dofile (this->lua, (*dir).path ().string ().c_str () );

                if (result > 0) {
                    std::cout << "Error loading script: " << (*dir) << std::endl;
                }
            }
        }
    }

    void job_dispatcher::close_lua () {
        boost::mutex::scoped_lock lock (this->luaMutex);

        std::cout << "Shutting down lua..." << std::endl;

        this->jobHandlers.clear ();
        lua_close (this->lua);
    }
}
