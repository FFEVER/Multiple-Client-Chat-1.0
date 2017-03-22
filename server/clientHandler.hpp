//
//  clientHandler.hpp
//  C_SE_MyProjectFinal
//
//  Created by Nattaphol on 11/10/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#ifndef clientHandler_hpp
#define clientHandler_hpp

#include <stdio.h>
#include <string.h>
#include <time.h>

#include <errno.h> /* strerror & errno */

#include <pthread.h>

#include <unistd.h> /* read(), write() and close() socket */
#include <sys/types.h> /* definition of data types using in <netinet/in.h> & <sys/socket.h> */
#include <sys/socket.h> /* bind() connect() accept() socket() */

#include "client_info.hpp"
#include "linkedMan.hpp"
#include "logfileman.hpp"

char* getTime(void);
char* getDate(void);
void *clientHandler(void *t_info); /* client command handler thread */


#endif /* clientHandler_hpp */
