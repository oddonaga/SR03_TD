#include "header.h"




int main(){
	
	int id_msg, long_msg=sizeof(message)-sizeof(long);
	int i;
	
	int nbre_clients=0, nbre_clients2=0;
	int generation_id=1;
	
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
			nbre_clients++;
			message msg0;
			msgrcv(id_msg, (void*)&msg0, long_msg, 1, 0);
			printf("Message recu\n");
			if(nbre_clients2 < 2){
				nbre_clients2++;
				switch(msg0.req_clt){
					case -1: // affectation num client
						printf("Affectation num client\n");
						message msg;
						msg.type=14;
						msg.id_clt=generation_id; // num
						msg.req_clt=0;
						msg.text='a';
						
						generation_id++;
						msgsnd(id_msg, (void*)&msg, long_msg, 0);
						nbre_clients++;
						break;
						
					case 3: // liste produit
						printf("Liste produit\n");
						message msg2;
						msg2.type=msg0.id_clt;
						msg2.id_clt=1; // num	
						msg2.req_clt=0;
						msg2.text='4';
						printf("msg.text=%c\n",msg2.text);
						msgsnd(id_msg, (void*)&msg2, long_msg, 0);
						
				}
				nbre_clients2--;
			}
		}
		
		
		
	}
	
	
	
}
