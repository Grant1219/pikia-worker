#pragma once

namespace pikia {
    class reloadable {
        public:
            reloadable () {}
            virtual ~reloadable () {}

            virtual void reload () {
                close ();
                setup ();
            }

        protected:
            virtual void setup () {}
            virtual void close () {}
    };
}
