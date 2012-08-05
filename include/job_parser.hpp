#pragma once

#include <sstream>
#include <beanstalk/beanstalk.hpp>

namespace pikia {
    class job_parser {
        public:
            job_parser (Beanstalk::Job& _job) {}

        private:
            std::stringbuf buf;
    };
}
