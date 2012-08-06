#pragma once

#include <iostream>
#include <sstream>
#include <lua.hpp>
#include <beanstalk/beanstalk.hpp>

namespace pikia {
    class job_buffer {
        public:
            static void bind (lua_State* _state);

        public:
            job_buffer ();
            job_buffer (const job_buffer& _buf);
            job_buffer (Beanstalk::Job& _job); 

            int32_t read_int ();
            std::string read_string ();

            void write_int (int32_t _value);
            void write_string (std::string _value);

            void clear ();
            void reset ();

            job_buffer& operator= (const job_buffer& _buf) {
                this->buf.str (_buf.buf.str () );
                this->buf.pubseekpos (_buf.pos);
                return *this;
            }

        private:
            template<typename T>
            void put (std::stringbuf& _buf, const T& _value) {
                _buf.sputn (reinterpret_cast<const char*> (&_value), sizeof (_value) );
            }

            template<typename T>
            void get (std::stringbuf& _buf, T& _value) {
                this->pos += _buf.sgetn (reinterpret_cast<char*> (&_value), sizeof (_value) );
            }

        private:
            std::streampos pos;
            std::stringbuf buf;
    };
}
