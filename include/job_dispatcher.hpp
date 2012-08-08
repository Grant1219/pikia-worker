#pragma once

#include <map>
#include <memory>
#include <functional>
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <boost/thread.hpp>
#include <connection_bundle.hpp>
#include <job_context.hpp>

namespace pikia {
    class job_dispatcher {
        public:
            bool register_handler (uint32_t _id, std::function<bool (job_context&)> _callback);
            bool register_handler (uint32_t _id, const luabind::object& _callback);
            void unregister_handler (uint32_t _id);

            bool dispatch_job (job_context& _context);

            void clear_handlers ();

        private:
            std::map<uint32_t, std::function<bool (job_context&)> > jobHandlers;
    };
}
