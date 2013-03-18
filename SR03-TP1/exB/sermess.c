#include "header.h"




int main(){
	
	int id_msg, long_msg=sizeof(message)-sizeof(long);
	int i,j;
	
	int nbre_clients=0, nbre_clients2=0;
	int generation_id=2;
	
	produit p;
	
	message msg_recu, msg_envoi;
	
	//produit liste_produit[]={
	// "carotte","pizza","navet","potimarron","raviolis","patate",".",".",".","."};
	
	produit p1={0, "carotte", 5, 1.2}, p2={1, "pizza", 10, 3.5}, p3={2, "raviolis", 6, 2.5},
	p4={3, "navet", 1, 1.7}, p5={4, "patate", 20, 0.2};
	produit liste_produit[]={p1,p2,p3,p4,p5};
	
	
	key_t key = ftok("./a.txt", 0);
	
	if(key==-1)
		perror("error ftok");
	id_msg = msgget(key, IPC_CREAT|IPC_EXCL|0666);
	if(id_msg==-1)
	{
		//perror("msgget() - File existe deja !");
		if(EEXIST == errno) // File de Message déjà existante
		{
			// Suppression de la file --> system("ipcs -q");
			char buffer_key[50];
			sprintf(buffer_key, "%d", key);
			char system_call[150];
			strcpy(system_call, "ipcrm -Q ");
			strcat(system_call, buffer_key);
			//printf(system_call);
			printf("\n");
			// Suppression
			system(system_call);
			// Re-création de la file
			id_msg = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
		}
	}
	
	while(nbre_clients < 4){
		if(msgrcv(id_msg, (void*)&msg_recu, long_msg, 1, 0)==-1)
			printf("erreur msgrcv\n");
		else
			printf("Message recu\n");
		
		if(nbre_clients2 < 3){
			nbre_clients2++;
			
			switch(msg_recu.req_clt){
				case -1: // affectation num client
					printf("Affectation num client\n");
					nbre_clients++;
					msg_envoi.type=14;
					msg_envoi.id_clt=generation_id; // num
					msg_envoi.req_clt=0;
					
					
					msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
					
					printf("Envoyé : %d\n\n",generation_id);
					generation_id++;
					break;
					
				case 3: // liste produit
					printf("Liste produit\n");
					
					msg_envoi.type=msg_recu.id_clt;
					msg_envoi.id_clt=msg_recu.id_clt; // num
					msg_envoi.req_clt=0;
					
					for(j=0;j<MAX_ELT;j++)
						strcpy(msg_envoi.text[j], liste_produit[j].description);
					
					printf("Envoi de la liste\n\n");
					msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
					break;
					
				case 5: // détails pour un produit
					printf("Détail produit\n");
					msg_envoi.type=msg_recu.id_clt;
					msg_envoi.id_clt=msg_recu.id_clt; // num
					msg_envoi.req_clt=0;
					
					p=liste_produit[msg_recu.id_produit-1];
					char tmp[50];
					sprintf(msg_envoi.text2,"Produit %s -> prix : %.1f€, %d en stock\n",p.description, p.prix, p.en_stock);
					
					printf("Envoi description du produit %s\n\n", p.description);
					msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
					break;
					
				case 4: // ajouter n objets
					printf("Ajouter n objets\n");
					
					msg_envoi.type=msg_recu.id_clt;
					msg_envoi.id_clt=msg_recu.id_clt; // num
					msg_envoi.req_clt=0;
					
					if(msg_recu.quantite>liste_produit[msg_recu.id_produit-1].en_stock)
					{
						msg_envoi.insuffisant = 1;
						printf("stock insuffisant pour le produit %s\n", liste_produit[msg_recu.id_produit-1].description);
					}
					else
					{
						msg_envoi.insuffisant = 0;
						liste_produit[msg_recu.id_produit-1].en_stock = liste_produit[msg_recu.id_produit-1].en_stock - msg_recu.quantite;
					}
					
					sprintf(msg_envoi.text2, "%s", liste_produit[msg_recu.id_produit-1].description);
					printf("%s, quantite restante : %d\n\n", liste_produit[msg_recu.id_produit-1].description, liste_produit[msg_recu.id_produit-1].en_stock);
					
					msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
					break;
					
				case 6: //vider panier
					printf("Retour des objets du panier en stock\n");
					for(j=0;j<5;j++)
					{
						liste_produit[j].en_stock = liste_produit[j].en_stock + msg_recu.retour_produit_quantite[j];
					}	
					
			}
			
			nbre_clients2--;
		} else printf("Sortie else\n");
	}
	
	
	
	
	
}



/*#include <ipc.h>
 #include <stdio.h>
 #include <string.h>
 #include <sys/types.h>
 #include <msg.h>
 
 
 typedef struct{
 long type;
 int req_clt;
 int id_clt;
 char* chaine_msg;
 }message;
 
 int main(){
 
 int id_msg, long_msg=sizeof(message)-sizeof(long);
 message msg;
 int nbre_clients=0, nbre_clients2=0;
 int generation_id=1;
 
 key_t key = ftok("./sr03p055.txt", 0);
 
 if(key==-1)
 perror("error ftok");
 id_msg = msgget(key, IPC_CREAT|IPC_EXCL|0666);
 if(id_msg==-1)
 {
 perror("msgget() - File existe deja !");	
 }
 else
 {
 while(nbre_clients<5){
 msgrcv(id_msg, (void*)&msg, long_msg, 1, 0);
 switch(req_clt){
 case -1: // affectation num client
 msg.type=14;
 msg.id_clt=generation_id; // num
 generation_id++;
 msgsnd(id_msg, (void*)&msg, long_msg, 0);
 nbre_clients++;
 nbre_clients2++;
 break;
 
 case 3: // liste produit
 msg.type=msg.id_clt;
 msg.id_clt=1; // num
 msgsnd(id_msg, (void*)&msg, long_msg, 0);
 }
 }
 
 if(nbre_client){
 }
 
 }
 
 
 
 }
 */