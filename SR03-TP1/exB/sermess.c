#include "header.h"




int main(){
	
	int id_msg, long_msg=sizeof(message)-sizeof(long);
	int i;
	
	int nbre_clients=0, nbre_clients2=0;
	int generation_id=1;
	
	message msg_recu, msg_envoi;
	
	char* liste_produit[]={"carotte","navet","potimarron"};
	
	key_t key = ftok("./a.txt", 0);
	
	if(key==-1)
		perror("error ftok");
	id_msg = msgget(key, IPC_CREAT|IPC_EXCL|0666);
	if(id_msg==-1)
		perror("msgget() - File existe deja !");	
	
	else
	{
		while(nbre_clients < 4){
			printf("0\n");
			nbre_clients++;
			printf("1\n");
			msgrcv(id_msg, (void*)&msg_recu, long_msg, 1, 0);
			printf("Message recu\n");
			if(nbre_clients2 < 2){
				nbre_clients2++;
				switch(msg_recu.req_clt){
					case -1: // affectation num client
						printf("Affectation num client\n");
						msg_envoi.type=14;
						msg_envoi.id_clt=generation_id; // num
						msg_envoi.req_clt=0;
						msg_envoi.text='a';
						
						generation_id++;
						msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
						nbre_clients++;
						printf("Envoyé\n");
						break;
						
					case 3: // liste produit
						printf("Liste produit\n");
						
						msg_envoi.type=msg_recu.id_clt;
						msg_envoi.id_clt=1; // num	
						msg_envoi.req_clt=0;
						msg_envoi.text='c';
						printf("msg.text=%c\n", msg_envoi.text);
						msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
						
				}
				nbre_clients2--;
			} else printf("Sortie else\n");
		}
		
		
		
	}
	
	
	
}
