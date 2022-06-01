#include <datalogger.h>
#include <sqlite3.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <chunkheader.h>

bool SQLDataLogger::init(char *name,long long max_size)
{
    bool status=false;
    return status;
}

bool ComplexDataLogger::init(char *name,long long max_size)
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

bool SimpleDataLogger::init(char *name,long long max_size)
{
    bool status=false;
    this->loggerfile.open(name,std::ios::out | std::ios::binary ); //do this once
    if (this->loggerfile.is_open())
    {
        std::cout<<"Simple Logger: File opened successfully \n";
        status=true;
    }
    return status;
}

bool SQLDataLogger::write_data(char * buffer, int size)
{
    bool status=false;
    

    return status;
}

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
            std::cout<<"Maximum Size of file has been reached cannot not write anymore \n";
            status= false; 
            break;
        }
        
    }
   
    return status;
}

int ComplexDataLogger::add_header()
{
    deadbeef_header_stamp header;
    unsigned char * header_pointer = (unsigned char*)&header;
    int iter_header=0;
    while (header_pointer < (unsigned char*)&header + sizeof(header))
    {
        this->loggerfile << *header_pointer;
        header_pointer++;
        iter_header++;
        increment_position(1);
    }
    return sizeof(header);
}

bool SimpleDataLogger::write_data(char * buffer, int size)
{
    bool status=false;
    if (this->loggerfile.is_open())
    {
        std::cout<<"File opened successfully";
    }

    for(int iterator=0;iterator < size ; iterator++)
    {
        this->loggerfile << buffer[iterator];
        if((iterator+1) % chunksize ==0)
        {
            this->loggerfile <<"\n";
        } 
    }
    return status;
}



