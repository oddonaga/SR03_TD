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
#include <errno.h>

const MAX_ELT=3;
const MAX_TAILLE_STRING=20;

typedef struct{
	long type;
	int req_clt;
	int id_clt;
	char text[10][10];
}message;

