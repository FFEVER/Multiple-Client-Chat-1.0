//
//  serverHandler.hpp
//  C_Project_Client
//
//  Created by Nattaphol on 11/11/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#ifndef serverHandler_hpp
#define serverHandler_hpp

#include <stdio.h>
#include <string.h>


#include <netdb.h> /* gethostbyname() & h_addr */
#include <unistd.h> /* read() & close() socket */

#include "user_info.hpp"
#include "fileMan.hpp"

#define SERVERIP "127.0.0.1"
#define SERVERPORT 8080


int connectWithServer(void); /* connect with a server */

void *receiver(void *param); /* server response receiver handler thread */

#endif /* serverHandler_hpp */
