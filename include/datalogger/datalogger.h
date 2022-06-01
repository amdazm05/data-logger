#ifndef _DATALOGGER
#define _DATALOGGER
#include <fstream>
class DataLogger
{
    protected:
        long long max_size;
    public:
        DataLogger(){}
        virtual bool init(char *name,long long max_size)=0;
        virtual bool write_data(char * buffer, int size)=0;
        virtual ~DataLogger(){}
};

class SQLDataLogger:public DataLogger
{
    public:
        SQLDataLogger(){}
        bool init(char *name,long long max_size);
        bool write_data(char * buffer, int size);
        ~SQLDataLogger(){}
};


class ComplexDataLogger:public DataLogger
{
    private:
        std::ofstream loggerfile;
        uint16_t chunksize=1024;
        long long seek_position;
        void increment_position(long long position);
    public:
        ComplexDataLogger(){}
        bool init(char *name,long long max_size);
        bool write_data(char * buffer, int size);
        int add_header();  //retruns size 
        ~ComplexDataLogger(){}
};


class SimpleDataLogger:public DataLogger
{   
    private:
        std::ofstream loggerfile;
        uint16_t chunksize=1024;
        long long seek_position;
        void increment_position(long long position);
    public:
        SimpleDataLogger(){}
        bool init(char *name,long long max_size);
        bool write_data(char * buffer, int size);
        ~SimpleDataLogger(){}
};



#endif