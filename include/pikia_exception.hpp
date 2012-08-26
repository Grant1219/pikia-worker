#include <stdexcept>

namespace pikia {
    class pikia_runtime_error : public std::runtime_error {
        public:
            pikia_runtime_error (const std::string& _what) : std::runtime_error (_what) {}
    };

    class pikia_logic_error : public std::logic_error {
        public:
            pikia_logic_error (const std::string& _what) : std::logic_error (_what) {}
    };
}
