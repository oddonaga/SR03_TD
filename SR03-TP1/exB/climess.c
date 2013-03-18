#include "header.h"


int main(){
int id_msg;
int i,n;
int long_msg=sizeof(message)-sizeof(long);
int id_clt;
message msg_recu, msg_envoi;

panier_client panier = {{"carotte","pizza","raviolis","navet","patate"} , {0,0,0,0,0}};

key_t key = ftok("./a.txt", 0);

if(key==-1)
perror("error ftok");

id_msg = msgget(key, 0);
if(id_msg==-1)
perror("msgget() - pas de file !");	

else
{
printf("\nMessage\n");

// Demande numero client
printf("Demande numero client\n");
msg_envoi.type=1; // le serveur ne récupère que les msg de type 1
msg_envoi.req_clt=-1;
msg_envoi.id_clt=-1;
//msg_envoi.text[0]='\0';

msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
msgrcv(id_msg, (void*)&msg_recu, long_msg, 14, 0);
id_clt=msg_recu.id_clt;
printf("Recu : %d \n",id_clt);

int choix, choix2;
while(1){
printf("\nClient n°%d\n", id_clt);
printf("-----------------\n");
printf("1 : Demander la liste des produits\n");
printf("2 : Ajouter n produits au panier\n");
printf("3 : Voir le panier\n");
printf("4 : Vider le panier\n");
printf("-----------------\n");
printf("Choix : ");
scanf("%d",&choix);

switch (choix) {
case 1:
// Demande liste produit
printf("Demande liste produit\n");
msg_envoi.type=1;
msg_envoi.req_clt=3;
msg_envoi.id_clt=id_clt;

if(msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0))
printf("erreur msgrcv\n");
msgrcv(id_msg, (void*)&msg_recu, long_msg, id_clt, 0);

int nbre_elts=0;
for(i=0;i<MAX_ELT;i++)
if(strcmp(msg_recu.text[i],".")>0){
printf("- %d",i+1);
printf(" : %s\n",msg_recu.text[i]);
nbre_elts++;
}

printf("\nTapez le numéro du produit pour obtenir des informations, 0 pour revenir au menu : ");
scanf("%d",&choix2);

if(choix2<1 || choix2>nbre_elts)
break;

// Détail produit
msg_envoi.type=1;
msg_envoi.req_clt=5;
msg_envoi.id_clt=id_clt;
msg_envoi.id_produit=choix2;

if(msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0))
printf("\n");
msgrcv(id_msg, (void*)&msg_recu, long_msg, id_clt, 0);

printf("%s\n",msg_recu.text2);

break;
case 2:
// Demande Ajout n produits
printf("Demande Ajout n produits\n");
printf("Tapez le numéro du produit que vous voulez ajouter : \n");
 scanf("%d",&choix);
 printf("Combien voulez-vous en ajouter ?\n");
 scanf("%d",&n);

 msg_envoi.type=1;
 msg_envoi.req_clt=4;
 msg_envoi.id_clt=id_clt;
 msg_envoi.id_produit = choix;
 msg_envoi.quantite = n;
 
 if(msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0))
	printf("\n");
 
 	msgrcv(id_msg, (void*)&msg_recu, long_msg, id_clt, 0);
 	
 	if(msg_recu.insuffisant==1)
 		printf("Stock du produit %s insuffisant\n\n", msg_recu.text2);
 	else
 	{
 	 panier.quantite[choix-1] = panier.quantite[choix-1]+n;
	 printf("%d %s ajoutés\n\n", n, msg_recu.text2);
 	}
	 
break;
case 3:
// Afficher panier
printf("\nPanier :\n");
for(i=0;i<5;i++)
{
	if(panier.quantite[i]>0)
	printf("%s : %d\n", panier.desc_produit[i], panier.quantite[i]);
}	 
break;

case 4:
// Vider panier
 msg_envoi.type=1;
 msg_envoi.req_clt=6;
 msg_envoi.id_clt=id_clt;
 
 for(i=0;i<5;i++)
{
	msg_envoi.retour_produit_quantite[i]=panier.quantite[i];
}	
 
 if(msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0))
	printf("\n");

for(i=0;i<5;i++)
{
	panier.quantite[i]=0;
}	 
printf("\nLe panier est desormais vide !\n");

break;

default:
break;
}
}
}
}
