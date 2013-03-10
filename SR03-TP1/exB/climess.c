#include "header.h"

int main(){
	int id_msg;
	int i;
	int long_msg=sizeof(message)-sizeof(long);
	int id_clt;
	message msg;
	
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
		msg.type=1; // le serveur ne récupère que les msg de type 1
		msg.req_clt=-1;
		msgsnd(id_msg, (void*)&msg, long_msg, 0);
		msgrcv(id_msg, (void*)&msg, long_msg, 14, 0);
		id_clt=msg.id_clt;
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
					msg.type=1;
					msg.req_clt=3;
					msg.id_clt=id_clt;

					msgsnd(id_msg, (void*)&msg, long_msg, 0);
					msgrcv(id_msg, (void*)&msg, long_msg, id_clt, 0);

					printf("- %c\n",msg.text);
					break;
				default:
					break;
			}
		}
			}
}
