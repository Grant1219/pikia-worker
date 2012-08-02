#include <iostream>
#include <hiredis/hiredis.h>
#include <beanstalk/beanstalk.hpp>
#include <worker.hpp>

int main (int argc, char** argv) {
    redisContext* redis = redisConnect ("127.0.0.1", 6379);
    Beanstalk::Client bean ("127.0.0.1", 11300);

    pikia::worker worker ("host=localhost user=pikia password=illutia.is.gay", redis, bean);
    
    while (!worker.is_done () ) {
        worker.do_work ();
    }

    return 0;
}
