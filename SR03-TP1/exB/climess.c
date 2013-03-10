#include "header.h"


int main(){
	int id_msg;
	int i;
	int long_msg=sizeof(message)-sizeof(long);
	int id_clt;
	message msg_recu, msg_envoi;
	
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
		msg_envoi.text='\0';
		
		msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
		msgrcv(id_msg, (void*)&msg_recu, long_msg, 14, 0);
		id_clt=msg_recu.id_clt;
		printf("Recu : %d \n",id_clt);

		int choix;
		while(1){
			printf("\nClient n°%d\n", id_clt);
			printf("-----------------\n");
			printf("1 : Demander la liste des produits\n");
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
					msg_envoi.text='b';
					msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
					msgrcv(id_msg, (void*)&msg_recu, long_msg, id_clt, 0);

					printf("- %c\n",msg_recu.text);
					break;
				default:
					break;
			}
		}
			}
}




