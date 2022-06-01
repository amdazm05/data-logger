#include <datalogger.h>
#include <iostream>
#include <chunkheader.h>

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
    buffer=new char[9];
    loggers[2]->init("b.dat",1024);
    loggers[1]->init("c.dat",2048);


    while(true)
    {
        if(loggers[1]->write_data("TrojanHorse",11))
        {
            continue;
        }
        else 
        {
            break;
        }
    }


    while(true)
    {
        if(loggers[2]->write_data("TrojanHorse",11))
        {
            continue;
        }
        else 
        {
            break;
        }
    }

    return success ? EXIT_SUCCESS:EXIT_FAILURE;
}   