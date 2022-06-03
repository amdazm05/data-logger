#ifndef _DATALOGGER
#define _DATALOGGER
#include <fstream>
class DataLogger
{
    protected:
        long long max_size;
    public:
        DataLogger(){}
        virtual bool init(std::string name,long long max_size)=0;
        virtual bool write_data(char * buffer, int size)=0;
        virtual bool reopen_file(std::string)=0;
        virtual ~DataLogger(){}
};

class SQLDataLogger:public DataLogger
{
    public:
        SQLDataLogger(){}
        bool init(std::string name,long long max_size);
        bool write_data(char * buffer, int size);
        bool reopen_file(std::string);
        ~SQLDataLogger(){}
};


class ComplexDataLogger:public DataLogger
{
    private:
        std::fstream loggerfile;
        uint16_t chunksize; //default if not specified
        long long seek_position;
        void increment_position(long long position);
    public:
        ComplexDataLogger(){}
        bool init(std::string name,long long max_size);
        bool write_data(char * buffer, int size);
        bool reopen_file(std::string);
        void closefile();
        bool alotFilesize();
        void set_chunksize(int chunksize);
        int add_header();  //retruns size 
        ~ComplexDataLogger(){}
};


class SimpleDataLogger:public DataLogger
{   
    private:
        std::fstream loggerfile;
        uint16_t chunksize=1024;
        long long seek_position;
        void increment_position(long long position);
    public:
        SimpleDataLogger(){}
        bool init(std::string name,long long max_size);
        bool write_data(char * buffer, int size);
        void set_chunksize(int chunksize);
        bool alotFilesize();
        void closefile();
        bool reopen_file(std::string);
        ~SimpleDataLogger(){}
};



#endif