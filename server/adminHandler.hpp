//
//  adminHandler.hpp
//  C_SE_MyProjectFinal
//
//  Created by Nattaphol on 11/10/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#ifndef adminHandler_hpp
#define adminHandler_hpp

#include <stdio.h>
#include <stdlib.h> /* malloc() free() exit() atoi() */
#include <errno.h> /* strerror & errno */
#include <string.h>

#include <pthread.h>

#include <sys/types.h> /* definition of data types using in <netinet/in.h> & <sys/socket.h> */
#include <unistd.h> /* read(), write() and close() socket */
#include <sys/socket.h> /* bind() connect() accept() socket() */

#include "linkedman.hpp"
#include "logfileMan.hpp"

#define CMDLEN 16


void *commandHandler(void *null);

int kickClient(struct CLIENT_LLIST *ll,char cli_name[CLIENT_NAME_LEN]);/* kick client out of server */


#endif /* adminHandler_hpp */
