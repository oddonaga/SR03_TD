/*
* header.h
*
*
* Created by Agathe Oddon on 10/03/13.
* Copyright 2013 __MyCompanyName__. All rights reserved.
*
*/

#include <stdio.h>
#include <string.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#include <errno.h>

const MAX_ELT=5;

typedef struct{
int id;
char description[20];
int en_stock;
float prix;
}produit;


typedef struct{
long type;
int req_clt;
int id_clt;
int id_produit;
int quantite;
int insuffisant;
int retour_produit_quantite[5];
produit liste_produit[5];
char text[10][20];
char text2[50];
}message;


typedef struct{
char desc_produit[5][20];
int quantite[5];
}panier_client;
