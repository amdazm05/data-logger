#include <datalogger.h>
#include <sqlite3.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <chunkheader.h>
#include <fcntl.h>

bool SQLDataLogger::init(std::string name,long long max_size)
{
    bool status=false;
    return status;
}

bool ComplexDataLogger::init(std::string name,long long max_size)
{
    bool status=false;
    this->max_size=max_size;
    this->loggerfile.open(name,std::ios::in |std::ios::out| std::ios::binary |std::ios::ate | std::ios::trunc ); //do this once
    alotFilesize();
    this->seek_position=0;
    std::cout<<seek_position<<std::endl;
    if(seek_position!=0)
    {
        std::cout<<"Closing append mode"<<std::endl;
        this->loggerfile.open(name,std::ios::in |std::ios::out| std::ios::binary |std::ios::ate  | std::ios::trunc);
    }
    if(seek_position%chunksize!=0 )
    {
        seek_position=seek_position-(seek_position%chunksize)+25;
        loggerfile.seekp(seek_position);
    }
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
    this->loggerfile.open(name,std::ios::in |std::ios::out| std::ios::binary |std::ios::ate |std::ios::app ); //do this once
    this->seek_position=this->loggerfile.tellp();
    if(seek_position%chunksize!=0 )
    {
        seek_position=seek_position-(seek_position%chunksize)+25;
        loggerfile.seekp(seek_position);
    }
    this->max_size=max_size;
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

bool SQLDataLogger::reopen_file(std::string)
{
    return true;
}



// ##############################################################
//                      COMPLEX LOGGER
// ##############################################################
bool ComplexDataLogger::alotFilesize()
{   
    this->loggerfile.seekg(0);
    for(int i=0; i<max_size; i++)   //outputting spces to create file
        this->loggerfile<<' ';
}

void ComplexDataLogger::increment_position(long long iteration)
{
    this->seek_position+=iteration;
}

int ComplexDataLogger::get_size_of_header()
{
    return sizeof(deadbeef_header_stamp);
}

bool ComplexDataLogger::write_data(char * buffer, int size)
{
    bool status=false;
    loggerfile.seekg(seek_position);
    int iterator=0;
    int sizeofheader=get_size_of_header();

    if(size>chunksize)
    {
  
        std::cout<<"SIZE"<<(chunksize-sizeofheader)<<std::endl;
        std::cout<<size/(chunksize-sizeofheader)<<std::endl;
        for(;iterator<size/(chunksize-sizeofheader);iterator++)
        {
            if(seek_position < max_size)
            {
                add_header();
                loggerfile.write(buffer+ (chunksize-sizeofheader)*iterator,chunksize-sizeofheader);
                increment_position(chunksize-sizeofheader);
            }
        }
            
    }

    if(size<(chunksize-sizeofheader) || size%(chunksize-sizeofheader))
    {
        if(seek_position < max_size)
        {
            add_header();
            std::cout<<buffer+iterator*(chunksize-sizeofheader)<<" - "<<size%chunksize<<std::endl;
            loggerfile.write(buffer+iterator*(chunksize-sizeofheader),size%(chunksize-sizeofheader));
            increment_position(chunksize-sizeofheader);
        }
    }
    
    std::cout<<seek_position<<std::endl;
    return status;
}

int ComplexDataLogger::add_header()
{
    deadbeef_header_stamp header;
    uint8_t * header_pointer = (uint8_t*)&header;
    int iter_header=0;
    this->loggerfile.write((char *)header_pointer,sizeof(header));
    increment_position(sizeof(header));
    return sizeof(header);
}


bool ComplexDataLogger::reopen_file(std::string name)
{
    bool status=false;
    this->loggerfile.open(name,std::ios::in |std::ios::out| std::ios::binary |std::ios::ate  ); //do this once
    this->seek_position=this->loggerfile.tellp();
    if(seek_position%chunksize!=0 )
    {
        seek_position=seek_position-(seek_position%chunksize)+25;
        loggerfile.seekp(seek_position);
    }

    if (this->loggerfile.is_open())
    {
        std::cout<<"Complex Logger: File reopened successfully \n";
        status=true;
    }
    return status;
}

void ComplexDataLogger::closefile()
{
    this->loggerfile.close();
}


void ComplexDataLogger::set_chunksize(int chunksize)
{
    std::cout<<"CHUNK SIZE SET TO :"<<chunksize<<std::endl;
    this->chunksize=chunksize;
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
                this->loggerfile << "#";
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


bool SimpleDataLogger::reopen_file(std::string name)
{
    bool status=false;
    this->loggerfile.open(name,std::ios::in |std::ios::out| std::ios::binary |std::ios::ate); //do this once
    this->seek_position=this->loggerfile.tellp();
    if(seek_position%chunksize!=0 )
    {
        seek_position=seek_position-(seek_position%chunksize)+25;
        loggerfile.seekp(seek_position);
    }
    if (this->loggerfile.is_open())
    {
        std::cout<<"Simple Logger: File reopened successfully \n";
        status=true;
    }
    return status;
    return true;
}

void SimpleDataLogger::set_chunksize(int chunksize)
{
    std::cout<<"CHUNK SIZE SET TO :"<<chunksize<<std::endl;
    this->chunksize=chunksize;
}

void SimpleDataLogger::closefile()
{
    this->loggerfile.close();
}