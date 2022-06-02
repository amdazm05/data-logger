#include <datalogger.h>
#include <sqlite3.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <chunkheader.h>

bool SQLDataLogger::init(std::string name,long long max_size)
{
    bool status=false;
    return status;
}

bool ComplexDataLogger::init(std::string name,long long max_size)
{
    bool status=false;
    this->loggerfile.open(name,std::ios::out | std::ios::binary ); //do this once
    this->seek_position=0;    //initially set position to 0
    this->max_size=max_size;
    if (this->loggerfile.is_open())
    {
        std::cout<<"Complex Logger: File opened successfully \n";
        status=true;
    }
    return status;
}

bool SimpleDataLogger::init(std::string name,long long max_size)
{
    bool status=false;
    this->max_size=max_size;
    this->seek_position=0;
    this->loggerfile.open(name,std::ios::out | std::ios::binary ); //do this once
    if (this->loggerfile.is_open())
    {
        std::cout<<"Simple Logger: File opened successfully \n";
        status=true;
    }
    return status;
}



// ##############################################################
//                      SQL LOGGER
// ##############################################################

bool SQLDataLogger::write_data(char * buffer, int size)
{
    bool status=false;
    

    return status;
}

bool SQLDataLogger::reopen_file(char *name)
{
    return true;
}



// ##############################################################
//                      COMPLEX LOGGER
// ##############################################################

void ComplexDataLogger::increment_position(long long iteration)
{
    this->seek_position+=iteration;
}

bool ComplexDataLogger::write_data(char * buffer, int size)
{
    bool status=false;
    loggerfile.seekp(this->seek_position);
    status= true;  
    for(int iterator=0; iterator <size; iterator ++)
    {
        if(this->seek_position <= this->max_size)
        {
            if
                (   
                    seek_position==0
                    || (seek_position % chunksize ==0
                    && seek_position >= chunksize)
                )
            {
                int sizeofheader=this->add_header();
            }
            this->loggerfile << buffer[iterator];
            increment_position(1);
        }
        else 
        {
            std::cout<<"Complex Logger : Maximum Size of file has been reached cannot not write anymore \n";
            status= false; 
            break;
        }
        
    }
   
    return status;
}

int ComplexDataLogger::add_header()
{
    deadbeef_header_stamp header;
    uint8_t * header_pointer = (uint8_t*)&header;
    int iter_header=0;
    while (header_pointer < (uint8_t*)&header + sizeof(header))
    {
        this->loggerfile << *header_pointer;
        header_pointer++;
        iter_header++;
        increment_position(1);
    }
    return sizeof(header);
}


bool ComplexDataLogger::reopen_file(char *name)
{
    bool status=false;
    this->loggerfile.open(name,std::ios::out | std::ios::binary | std::ios::ate ); //do this once
    this->seek_position=this->loggerfile.eof();
    // std::cout<<
    if (this->loggerfile.is_open())
    {
        std::cout<<"Complex Logger: File opened successfully \n";
        status=true;
    }
    return status;
}





// ##############################################################
//                      SIMPLE LOGGER
// ##############################################################

bool SimpleDataLogger::write_data(char * buffer, int size)
{
    bool status=false;
    this->loggerfile.seekp(this->seek_position);
    status= true;  
    for(int iterator=0; iterator <size; iterator ++)
    {
        if(this->seek_position <= this->max_size)
        {
            if
                (   
                    (seek_position % chunksize ==0
                    && seek_position >= chunksize)
                )
            {
                this->loggerfile << "\n";
                increment_position(1);
            }
            this->loggerfile << buffer[iterator];
            increment_position(1);
        }
        else 
        {
            std::cout<<"Simple logger :Maximum Size of file has been reached cannot not write anymore \n";
            status= false; 
            break;
        }
        
    }
    return status;
}

void SimpleDataLogger::increment_position(long long iteration)
{
    this->seek_position+=iteration;
}


bool SimpleDataLogger::reopen_file(char *name)
{
    return true;
}

