#include <boost/filesystem.hpp>
#include <job_dispatcher.hpp>

namespace pikia {
    bool job_dispatcher::register_handler (uint32_t _id, std::function<bool (job_context&)> _callback) {
        std::cout << "Registering callback for job ID: " << _id << std::endl;
        if (this->jobHandlers.find (_id) == this->jobHandlers.end () ) {
            this->jobHandlers[_id] = _callback;
            return true;
        }
        return false;
    }

    void job_dispatcher::unregister_handler (uint32_t _id) {
        std::map<uint32_t, std::function<bool (job_context&)> >::iterator it = this->jobHandlers.find (_id);
        if (it != this->jobHandlers.end () )
            this->jobHandlers.erase (it);
    }

    bool job_dispatcher::dispatch_job (job_context& _context) {
        std::cout << "Dispatching event..." << std::endl;
        std::map<uint32_t, std::function<bool (job_context&)> >::iterator it = this->jobHandlers.find (_context.id);
        if (it != this->jobHandlers.end () ) {
            try {
                return (*it).second (_context);
            }
            catch (const std::exception& e) {
                std::cout << "Error dispatching job: " << e.what () << std::endl;
            }
        }
        else
            throw job_invalid ();

        return false;
    }

    void job_dispatcher::clear_handlers () {
        this->jobHandlers.clear ();
    }
}
