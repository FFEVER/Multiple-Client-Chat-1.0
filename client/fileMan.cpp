//
//  fileMan.cpp
//  C_SE_MyProjectFinal
//
//  Created by Nattaphol on 11/10/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#include "fileMan.hpp"

void logFileCreate(){
    FILE *logFile = fopen("client_log.txt", "w");
    if(logFile == 0){
        fprintf(stderr, "%s, Can't open client_log.txt.",strerror(errno));
    }
    fclose(logFile);
    return;
}

void logFileWrite(const char *msg){
    FILE *logFile = fopen("client_log.txt", "a");
    if (logFile == 0) {
        fprintf(stderr, "%s, Can't open client_log.txt.",strerror(errno));
    }
    else fprintf(logFile, "%s", msg);
    
    fclose(logFile);
    return;
}

void chatLogFileCreate(){
    FILE *chatFile = fopen("chat_log.txt", "w");
    if(chatFile == 0){
        fprintf(stderr, "%s, Can't open chat_log.txt.",strerror(errno));
    }
    fclose(chatFile);
    return;
}

void chatLogFileWrite(const char *msg){
    FILE *chatFile = fopen("chat_log.txt", "a");
    if (chatFile == 0) {
        fprintf(stderr, "%s, Can't open chat_log.txt.",strerror(errno));
    }
    else fprintf(chatFile, "%s", msg);
    
    fclose(chatFile);
    return;
}
