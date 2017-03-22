//
//  logfileMan.hpp
//  C_SE_MyProjectFinal
//
//  Created by Nattaphol on 11/10/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#ifndef logfileMan_hpp
#define logfileMan_hpp

#include <stdio.h>
#include <stdlib.h>

void logFileCreate(void); /* create a new server logfile */
void logFileWrite(const char *msg); /* update a server logfile */
void error(const char *msg);/* print ERROR and exit() */

#endif /* logfileMan_hpp */
