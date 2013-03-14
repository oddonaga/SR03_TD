#include "header.h"


int main(){
	int id_msg;
	int i,n;
	int long_msg=sizeof(message)-sizeof(long);
	int id_clt;
	message msg_recu, msg_envoi;
	
	key_t key = ftok("./sr03p055.txt", 0);
	
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
						printf("erreur msgrcv\n");
					msgrcv(id_msg, (void*)&msg_recu, long_msg, id_clt, 0);
					
					printf("%s\n",msg_recu.text2);
					
					break;
				case 2:
					// Demande Ajout n produits
					printf("Demande Ajout n produits\n");
					printf("Combien de produits voulez-vous ajouter ?\n");
					scanf("%d",&n);
					
					msg_envoi.type=1;
					msg_envoi.req_clt=4;
					msg_envoi.id_clt=id_clt;
					msg_envoi.id_produit = n;
					
					if(msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0))
						printf("\n");
					
					msgrcv(id_msg, (void*)&msg_recu, long_msg, id_clt, 0);
					for(i=0;i<n;i++)
					{
						printf("produit ajouté : %s\n",msg_recu.text[i]);
					}
					break;
				default:
					break;
			}
		}
	}
}
