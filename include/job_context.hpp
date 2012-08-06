#pragma once

#include <lua.hpp>
#include <job_buffer.hpp>

namespace pikia {
    class job_context {
        public:
            static void bind (lua_State* _state);

        public:
            uint32_t id;
            job_buffer buf;
    };
}
