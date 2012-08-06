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

    job_buffer::job_buffer () : pos (0) {}

    job_buffer::job_buffer (const job_buffer& _buf)
        : pos (_buf.pos), buf (_buf.buf.str () ) {
        // set the position of this buffer to the copied buffer's position
        this->buf.pubseekpos (this->pos);
    }

    job_buffer::job_buffer (Beanstalk::Job& _job)
        : pos (0), buf (_job.body () ) {} 

    int32_t job_buffer::read_int () {
        int32_t value;
        get<int32_t> (this->buf, value);
        return value;
    }

    std::string job_buffer::read_string () {
        size_t length;
        get<size_t> (this->buf, length);
        char value[length];
        this->pos += this->buf.sgetn (value, length);
        return value;
    }

    void job_buffer::write_int (int32_t _value) {
        put<int32_t> (this->buf, _value);
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
}
