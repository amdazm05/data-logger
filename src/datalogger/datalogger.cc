#include <datalogger.h>
#include <sqlite3.h>
#include <chrono>
#include <iostream>
#include <fstream>

bool SQLDataLogger::init()
{
    bool status=false;
    return status;
}

bool ComplexDataLogger::init()
{
    bool status=false;
    return status;
}

bool SimpleDataLogger::init()
{
    bool status=false;
    return status;
}

bool SQLDataLogger::write_data(char * buffer, int size)
{
    bool status=false;
    

    return status;
}

bool ComplexDataLogger::write_data(char * buffer, int size)
{
    bool status=false;
    loggerfile.open("Complexlogger.dat",std::ios::out );
    if (loggerfile.is_open())
    {
        std::cout<<"File opened successfully";
    }

    for(int iterator=0;iterator <size ; iterator++)
    {
        if((iterator) % wordlen ==0)
        {
            std::time_t t = std::time(0);
            loggerfile <<header[0];
            loggerfile <<header[1];
            loggerfile <<header[2];
            loggerfile <<t;
        }
        loggerfile << buffer[iterator];
    }
    loggerfile.close();
    return status;
}

bool SimpleDataLogger::write_data(char * buffer, int size)
{
    bool status=false;
    loggerfile.open("Simplelogger.dat",std::ios::out );
    if (loggerfile.is_open())
    {
        std::cout<<"File opened successfully";
    }

    for(int iterator=0;iterator < size ; iterator++)
    {
        loggerfile << buffer[iterator];
        if((iterator+1) % wordlen ==0)
        {
            loggerfile <<"\n";
        }
            
    }
    loggerfile.close();
    return status;
}



