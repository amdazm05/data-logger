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
    this->max_size=max_size;
    this->file_name=name;
    alotFilesize();
    seek_position=find_seek_position();
    // this->seek_position=0; 
    //this section evaluates the number of headers present in an existing file 
    //and tells what position to start writing in.
    std::cout<<"SEEKER :"<<seek_position<<std::endl;

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
int ComplexDataLogger::find_seek_position()
{
    int position=0;
    this->loggerfile.open(file_name,std::ios::in |std::ios::binary );
    char * buffer=new char [chunksize];
    deadbeef_header_stamp headercheck;
    uint64_t time=0; 
    for(int i=0;i<max_size/chunksize;i++)
    {
        loggerfile.seekg(chunksize*i);
        loggerfile.read(buffer,chunksize);
        uint32_t number=  (((uint32_t)buffer[0]         & 0x000000FF)
                          |((uint32_t)buffer[1]<<8      & 0x0000FF00)
                          |((uint32_t)buffer[2]<<16     & 0x00FF0000)
                          |((uint32_t)buffer[3]<<24     & 0xFF000000));
        if(number ==(uint32_t)headercheck.header)
        {
            long long timeval = (((uint64_t)buffer[8]          & 0x00000000000000FF)
                                |((uint64_t)buffer[9]<<8       & 0x000000000000FF00)
                                |((uint64_t)buffer[10]<<16     & 0x0000000000FF0000)
                                |((uint64_t)buffer[11]<<24     & 0x00000000FF000000)
                                |((uint64_t)buffer[12]<<32     & 0x000000FF00000000)
                                |((uint64_t)buffer[13]<<40     & 0x0000FF0000000000)
                                |((uint64_t)buffer[14]<<48     & 0x00FF000000000000)
                                |((uint64_t)buffer[15]<<56     & 0xFF00000000000000)
                          );
            if(timeval>=time)
            {
                time=timeval;
                position++;
            }
        }
    }
    position=position*chunksize;
    std::cout<<"POSITION: "<<std::dec<<position<<std::endl;
    this->loggerfile.close();
    return position;
}
bool ComplexDataLogger::alotFilesize()
{   
    bool status =false;
    this->loggerfile.open(file_name,std::ios::in |std::ios::binary |std::ios::ate);
    int size=loggerfile.tellg();
    loggerfile.close();
    if(size <0)
    {
        std::cout<<"FILE DOES NOT EXIST"<<std::endl;
        loggerfile.open(file_name,std::ios::out |std::ios::binary);
        std::cout<<"NEW FILE CREATED"<<std::endl;
        std::cout<<"FILE SIZE IS "<<max_size<<std::endl;
        for(int i=0; i<max_size; i++)   //outputting spces to create file
            this->loggerfile<<' ';
        loggerfile.close();
        status=true;
    }

    else
    {
        std::cout<<"FILE EXISTS SIZE:"<<size<<" CONTINUING WRITING"<<std::endl;
        if(size<max_size)
        {
            std::cout<<"FILE SIZE IS SMALLER THAN DEFINED FILE SIZE"<<std::endl;
            loggerfile.open(file_name,std::ios::app);
            for(int i=0; i<max_size-size; i++)   //filling the empty places with " "
            {
                this->loggerfile<<' ';
            }
            if (this->loggerfile.is_open())
            {
                std::cout<<"FILE RESIZED TO"<<max_size<<std::endl;
            }
            loggerfile.close(); // close the file 
        }
        status=true;
    }
    return status;
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
    this->loggerfile.open(file_name,std::ios::out |std::ios::in | std::ios::binary  );
    bool status=false;
    this->loggerfile.seekg(this->seek_position);
    int iterator=0;
    int sizeofheader=get_size_of_header();
    std::cout<<"CURRENT TELLG:"<<loggerfile.tellg()<<std::endl;
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
    
    std::cout<<"SEEK POSITION :"<<seek_position<<std::endl;
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