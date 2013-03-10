/*
 *  header.h
 *  
 *
 *  Created by Agathe Oddon on 10/03/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>

const MAX_ELT=10;

typedef struct{
	long type;
	int req_clt;
	int id_clt;
	char text;
}message;

