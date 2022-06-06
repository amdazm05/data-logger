#include <datalogger.h>
#include <iostream>
#include <chunkheader.h>
#include "dotenv.h"
#include <chunkheader.h>
#include <unistd.h>
int main(int argc, char * argv[])
{
    dotenv::env.load_dotenv("/home/mean-bean-ubuntu-machine/data-logger/.env");
    std::cout << "MAX_SIZE: " << dotenv::env["MAX_SIZE"] << std::endl;
    std::cout << "CHUNK_SIZE: " << dotenv::env["CHUNK_SIZE"] << std::endl;
    
    long long  max_size=stol(dotenv::env["MAX_SIZE"]);
    int chunk_size=stoi(dotenv::env["CHUNK_SIZE"]) ;

    std::string path1=dotenv::env["SIMPLE_FILE_PATH"];
    std::string path2 =dotenv::env["COMPLEX_FILE_PATH"];

    bool success=false;
    DataLogger ** loggers;
    loggers=new DataLogger *[3];
    
    SQLDataLogger sql;
    loggers[0] = &sql;

    ComplexDataLogger cdl;
    cdl.set_chunksize(chunk_size);
    loggers[1] = &cdl;

    SimpleDataLogger sdl;
    sdl.set_chunksize(chunk_size);
    loggers[2] = &sdl;

    char * buffer =NULL;
    buffer=new char[9];
    
    loggers[2]->init(path1,max_size);
    loggers[1]->init(path2,max_size);

    // while(true)
    // {
    //     if(loggers[1]->write_data("TrojanHorse",11))
    //     {
    //         continue;
    //     }
    //     else 
    //     {
    //         break;
    //     }
        
    // }


    // while(true)
    // {
    //     if(loggers[2]->write_data("TrojanHorse",11))
    //     {
    //         continue;
    //     }
    //     else 
    //     {
    //         break;
    //     }
    // }

    // for(int i=0;i<1;i++)
    // {
    //     loggers[1]->write_data("TrojanHorse",11);
    //     loggers[2]->write_data("TrojanHorse",11);
    // }

    // cdl.closefile();
    // sdl.closefile();
    // sleep(5);

    // loggers[1]->reopen_file(path2);
    // loggers[2]->reopen_file(path1);

    for(int i=0;i<1;i++)
    {
        loggers[1]->write_data("TrojanHorseisaDangerousVirusTrojanHorseisaDangerousVirus",56);
        // loggers[2]->write_data("TrojanHorse",11);
    }
    loggers[1]->write_data("Hereisatestcase",15);
    return success ? EXIT_SUCCESS:EXIT_FAILURE;
}   