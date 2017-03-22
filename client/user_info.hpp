//
//  user_info.hpp
//  C_Project_Client
//
//  Created by Nattaphol on 11/11/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#ifndef user_info_h
#define user_info_h

#include <pthread.h>

#define BUFFSIZE 1024 /* message buffer size */
#define CLIENT_NAME_LEN 32 /* client name length */
#define CMDLEN 16 /* command length in packet*/
#define TIMESIZE 20 /* time buffer size */

struct PACKET {
    char command[CMDLEN]; /* command */
    char client_name[CLIENT_NAME_LEN]; /* client's name */
    char buff[BUFFSIZE]; /* payload */
    char time[TIMESIZE]; /* local server time */
    char date[TIMESIZE]; /* local server date */
};

struct USER {
    int sockfd; /* user's socket descriptor */
    char client_name[CLIENT_NAME_LEN]; /* user's name */
};

struct THREADINFO {
    pthread_t thread_ID;
    int sockfd;
};


#endif /* user_info_h */
