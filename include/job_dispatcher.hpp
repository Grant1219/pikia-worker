#pragma once

#include <map>
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <connection_bundle.hpp>
#include <job_context.hpp>

namespace pikia {
    class job_dispatcher {
        public:
            static void bind (lua_State* _state);

        public:
            job_dispatcher (boost::shared_ptr<connection_bundle> _bundle);
            ~job_dispatcher ();

            bool register_handler (uint32_t _id, boost::function<bool (job_context&)> _callback);
            bool register_handler (uint32_t _id, const luabind::object& _callback);
            void unregister_handler (uint32_t _id);

            bool dispatch_job (job_context& _context);

        private:
            void setup_lua ();
            void close_lua ();

        private:
            lua_State* lua;
            boost::shared_ptr<connection_bundle> bundle;

            std::map<uint32_t, boost::function<bool (job_context&)> > jobHandlers;

        private:
            class lua_job_handler {
                public:
                    lua_job_handler (const luabind::object _callback) : callback (_callback) {}

                    bool operator() (job_context& _context) {
                        return luabind::call_function<bool> (this->callback, boost::ref (_context) );
                    }

                private:
                    luabind::object callback;
                    lua_State* lua;
            };
    };
}
