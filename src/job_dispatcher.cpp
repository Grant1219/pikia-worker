#include <util.hpp>
#include <job_dispatcher.hpp>

namespace pikia {
    job_dispatcher::job_dispatcher (boost::shared_ptr<connection_bundle> _bundle)
    : bundle (_bundle) {
        this->setup_lua ();
    }

    job_dispatcher::~job_dispatcher () {
        this->close_lua();
    }

    bool job_dispatcher::register_handler (uint32_t _id, const boost::function<bool (job_context&)>& _callback) {
        return true;
    }

    bool job_dispatcher::register_handler (uint32_t _id, const luabind::object& _callback) {
        std::cout << "Lua job handler registered!" << std::endl;
        return true;
    }

    void job_dispatcher::setup_lua () {
        std::cout << "Setting up lua..." << std::endl;

        this->lua = luaL_newstate ();
        luaL_openlibs (this->lua);

        // setup some bindings
        luabind::open (this->lua);

        luabind::module (this->lua, "pikia") [
            luabind::class_<job_dispatcher> ("job_dispatcher")
                .def ("register_handler", (bool (job_dispatcher::*) (uint32_t, const luabind::object&) ) &job_dispatcher::register_handler)
        ];

        luabind::globals (this->lua) ["dispatcher"] = this;

        // load the scripts
        std::vector<std::string> scripts;
        scripts.push_back ("scripts/test.lua");

        for (std::vector<std::string>::iterator it = scripts.begin (); it != scripts.end (); ++it) {
            std::cout << "Loading lua script: " << (*it) << std::endl;
            luaL_dofile (this->lua, (*it).c_str () );
        }
    }

    void job_dispatcher::close_lua () {
        lua_close (this->lua);
    }
}
