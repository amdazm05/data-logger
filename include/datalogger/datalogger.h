#ifndef _DATALOGGER
#define _DATALOGGER
#include <fstream>
class DataLogger
{
    public:
        DataLogger(){}
        virtual bool init()=0;
        virtual bool write_data(char * buffer, int size)=0;
        virtual ~DataLogger(){}
};

class SQLDataLogger:public DataLogger
{
    public:
        SQLDataLogger(){}
        bool init();
        bool write_data(char * buffer, int size);
        ~SQLDataLogger(){}
};


class ComplexDataLogger:public DataLogger
{
    private:
        const uint8_t header[3]={0x55,0xEE,0x45};
        std::ofstream loggerfile;
        uint16_t wordlen=2;
    public:
        ComplexDataLogger(){}
        bool init();
        bool write_data(char * buffer, int size);
        ~ComplexDataLogger(){}
};


class SimpleDataLogger:public DataLogger
{   
    private:
        std::ofstream loggerfile;
        uint16_t wordlen=2;
    public:
        SimpleDataLogger(){}
        bool init();
        bool write_data(char * buffer, int size);
        ~SimpleDataLogger(){}
};



#endif