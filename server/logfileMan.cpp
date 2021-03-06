//
//  logfileMan.cpp
//  C_SE_MyProjectFinal
//
//  Created by Nattaphol on 11/10/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#include "logfileMan.hpp"

void logFileCreate(){
    FILE *logfile;
    /* overwrite a exist server_log.txt and create a new one */
    logfile = fopen("server_log.txt", "w");
    if(logfile == 0)
        fprintf(stderr,"Can't create server_log.txt");
    fclose(logfile);
    return;
}

void logFileWrite(const char *msg){
    FILE *logfile;
    /* update logfile */
    logfile = fopen("server_log.txt", "a");
    if(logfile == 0){
        fprintf(stderr,"Can't open server_log.txt");
    }
    else{
        fprintf(logfile, "%s", msg);
    }
    /* save the updates */
    fclose(logfile);
    return;
}

void error(const char *msg){
    logFileWrite(msg); /* write error to server logfile */
    perror(msg); /* print error generated by errno */
    exit(1); /* terminates program */
}
