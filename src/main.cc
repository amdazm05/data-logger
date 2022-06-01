#include <datalogger.h>
#include <iostream>

int main(int argc, char * argv[])
{
    bool success=false;
    DataLogger ** loggers;

    loggers=new DataLogger *[3];
    
    SQLDataLogger sql;
    loggers[0] = &sql;

    ComplexDataLogger cdl;
    loggers[1] = &cdl;

    SimpleDataLogger sdl;
    loggers[2] = &sdl;

    char * buffer =NULL;
    buffer=new char[4];

    buffer[0]=0x55;
    buffer[1]=0x64;
    buffer[2]=0x74;
    buffer[4]=0x09;
    buffer[5]=0x09;
    buffer[6]=0x09;
    buffer[7]=0x09;
    buffer[8]=0x09;

    loggers[2]->write_data(buffer,9);
    loggers[1]->write_data(buffer,9);
    loggers[1]->write_data(buffer,9);

    return success ? EXIT_SUCCESS:EXIT_FAILURE;
}   