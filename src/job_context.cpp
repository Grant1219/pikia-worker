#include <luabind/luabind.hpp>
#include <job_context.hpp>

namespace pikia {
    void job_context::bind (lua_State* _state) {
        luabind::module (_state, "pikia") [
            luabind::class_<job_context> ("job_context")
                .def_readonly ("id", &job_context::id)
                .def_readwrite ("buf", &job_context::buf)
        ];
    }
}
