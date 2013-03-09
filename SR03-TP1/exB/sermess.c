#include <ipc.h>
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
