#pragma once

#include <memory>
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <boost/thread.hpp>
#include <reloadable.hpp>
#include <configuration.hpp>
#include <job_context.hpp>
#include <job_dispatcher.hpp>

namespace pikia {
    class script_manager : public reloadable {
        public:
            static void bind (lua_State* _state);

        public:
            script_manager (std::shared_ptr<configuration> _config, std::shared_ptr<job_dispatcher> _jobDispatcher);
            ~script_manager ();

        private:
            void setup ();
            void close ();

            bool register_handler (uint32_t _id, const luabind::object& _callback);
            void unregister_handler (uint32_t _id);

            bool dispatch_job (job_context& _context);

        private:
            lua_State* lua;
            boost::mutex luaMutex;

            std::shared_ptr<configuration> config;
            std::shared_ptr<job_dispatcher> jobDispatcher;

        private:
            class lua_job_handler {
                public:
                    lua_job_handler (script_manager* _manager, const luabind::object& _callback);

                    bool operator() (job_context& _context);

                private:
                    script_manager* manager;
                    luabind::object callback;
                    lua_State* lua;
            };
    };
}
