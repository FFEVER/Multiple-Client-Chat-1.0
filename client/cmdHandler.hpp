//
//  cmdHandler.hpp
//  C_Project_Client
//
//  Created by Nattaphol on 11/11/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#ifndef cmdHandler_hpp
#define cmdHandler_hpp

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "user_info.hpp"
#include "fileMan.hpp"
#include "serverHandler.hpp"

void login(struct USER *me);
void setalias(struct USER *me); /* change Username */
void logout(struct USER *me); /* logout from sever but not exit */
void login(struct USER *me); /* login to server */

void sendToAll(struct USER *me, char *msg); /* send packet to server by command "send" */
void sendToAlias(struct USER *me, char *target, char *msg); /* send packet to server by command "whisp" */
void status(struct USER *me); /* display Username, connected status, server port, ip, socket */

char* getTime(void);
char* getDate(void);

#endif /* cmdHandler_hpp */
