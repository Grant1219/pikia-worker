#include <luabind/luabind.hpp>
#include <job_buffer.hpp>

namespace pikia {
    void job_buffer::bind (lua_State* _state) {
        luabind::module (_state, "pikia") [
            luabind::class_<job_buffer> ("job_buffer")
                .def ("read_int", &job_buffer::read_int)
                .def ("read_string", &job_buffer::read_string)
                .def ("write_int", &job_buffer::write_int)
                .def ("write_string", &job_buffer::write_string)
        ];
    }

    int32_t job_buffer::read_int () {
        int32_t value;
        get<int32_t> (this->buf.rdbuf (), value);
        return value;
    }

    std::string job_buffer::read_string () {
        size_t length;
        get<size_t> (this->buf.rdbuf (), length);
        char value[length];
        this->buf.rdbuf ()->sgetn (value, length);
        return value;
    }

    void job_buffer::write_int (int32_t _value) {
        put<int32_t> (this->buf.rdbuf (), _value);
    }

    void job_buffer::write_string (std::string _value) {
        put<size_t> (this->buf.rdbuf (), _value.length () );
        this->buf.rdbuf ()->sputn (_value.c_str (), _value.length () ); 
    }

    void job_buffer::clear () {
        this->buf.str ("");
    }
}
