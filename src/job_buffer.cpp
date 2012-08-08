#include <luabind/luabind.hpp>
#include <job_buffer.hpp>

namespace pikia {
    void job_buffer::bind (lua_State* _state) {
        luabind::module (_state, "pikia") [
            luabind::class_<job_buffer> ("job_buffer")
                .def ("read_int8", &job_buffer::read_int<int8_t>)
                .def ("read_int16", &job_buffer::read_int<int16_t>)
                .def ("read_int32", &job_buffer::read_int<int32_t>)
                .def ("read_uint8", &job_buffer::read_int<uint8_t>)
                .def ("read_uint16", &job_buffer::read_int<uint16_t>)
                .def ("read_uint32", &job_buffer::read_int<uint32_t>)
                .def ("read_string", &job_buffer::read_string)
                .def ("write_int8", &job_buffer::read_int<int8_t>)
                .def ("write_int16", &job_buffer::read_int<int16_t>)
                .def ("write_int32", &job_buffer::write_int<int32_t>)
                .def ("write_uint8", &job_buffer::read_int<uint8_t>)
                .def ("write_uint16", &job_buffer::read_int<uint16_t>)
                .def ("write_uint32", &job_buffer::write_int<uint32_t>)
                .def ("write_string", &job_buffer::write_string)
                .def ("clear", &job_buffer::clear)
                .def ("reset", &job_buffer::reset)
        ];
    }

    job_buffer::job_buffer () : pos (0) {}

    job_buffer::job_buffer (const job_buffer& _buf)
        : pos (_buf.pos), buf (_buf.buf.str () ) {
        // set the position of this buffer to the copied buffer's position
        this->buf.pubseekpos (this->pos);
    }

    job_buffer::job_buffer (Beanstalk::Job& _job)
        : pos (0), buf (_job.body () ) {} 

    std::string job_buffer::read_string () {
        size_t length;
        get<size_t> (this->buf, length);
        char value[length];
        this->pos += this->buf.sgetn (value, length);
        return value;
    }

    void job_buffer::write_string (std::string _value) {
        put<size_t> (this->buf, _value.length () );
        this->buf.sputn (_value.c_str (), _value.length () ); 
    }

    void job_buffer::clear () {
        this->buf.str ("");
        this->reset ();
    }

    void job_buffer::reset () {
        this->pos = 0;
    }

    std::string job_buffer::buffer () {
        return this->buf.str ();
    }
}
