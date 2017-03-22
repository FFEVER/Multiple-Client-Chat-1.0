//
//  client_info.hpp
//  C_SE_MyProjectFinal
//
//  Created by Nattaphol on 11/10/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#ifndef client_info_hpp
#define client_info_hpp

#include <pthread.h> /* threads */

#define CLIENT_NAME_LEN 32 /* limit lenght of client's name. */
#define CMDLEN 16 /* command string length */
#define BUFFSIZE 1024 /* limit lenght of text */
#define TIMESIZE 20

struct PACKET{
    char command[CMDLEN]; /* client sent command */
    char cli_name[CLIENT_NAME_LEN]; /* client name */
    char buff[BUFFSIZE]; /* text payload */
    char time[TIMESIZE]; /* local server time */
    char date[TIMESIZE]; /* local server date */
};

struct THREADINFO{
    pthread_t thread_ID; /* thread's pointer */
    int sockfd; /* socket file descriptor for each client */
    char cli_name[CLIENT_NAME_LEN]; /* client's name */
};

#endif /* client_info_hpp */
