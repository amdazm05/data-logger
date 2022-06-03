#ifndef _chunkheader
#define _chunkheader
#define MACRO_INT 0xDEADBEEF
#include <stdint.h>
#include <iostream>
#include <time.h>
#include <chrono>

struct deadbeef_header_stamp
{
    int header;
    long long time;
    deadbeef_header_stamp()
    {
        header = MACRO_INT;
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        std::chrono::system_clock::duration dtn = tp.time_since_epoch();
        time = dtn.count() * 
            std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
    }
};

#endif