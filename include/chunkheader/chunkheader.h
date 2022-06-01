#ifndef _chunkheader
#define _chunkheader
#define MACRO_INT 0xDEADBEEF
#include <stdint.h>
#include <iostream>
#include <time.h>
#include <chrono>

struct deadbeef_header_stamp
{
    const int header= MACRO_INT;
    unsigned char padding[4]={'#','#','#','#'};
    long long time = ([] () 
    {
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        std::chrono::system_clock::duration dtn = tp.time_since_epoch();
        
        return 
            dtn.count() * 
            std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
        
    })();
};

#endif