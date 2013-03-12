#include "header.h"
#include <errno.h>



int main(){
	
	int id_msg, long_msg=sizeof(message)-sizeof(long);
	int i,j;
	
	int nbre_clients=0, nbre_clients2=0;
	int generation_id=1;
	
	message msg_recu, msg_envoi;
	
	char* liste_produit[]={"carotte","navet","potimarron"};
	
	key_t key = ftok("./sr03p055.txt", 0);
	
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
			printf(system_call);
			printf("\n");
			// Suppression
			system(system_call);
			// Re-création de la file
			id_msg = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
		}
	}
	
		while(nbre_clients < 4){
			printf("0\n");
			nbre_clients++;
			printf("1\n");
			if(msgrcv(id_msg, (void*)&msg_recu, long_msg, 1, 0)==-1);
				printf("erreur msgrcv");
			else
				printf("Message recu\n");
			if(nbre_clients2 < 3){
				nbre_clients2++;
				switch(msg_recu.req_clt){
					case -1: // affectation num client
						printf("Affectation num client\n");
						msg_envoi.type=14;
						msg_envoi.id_clt=generation_id; // num
						msg_envoi.req_clt=0;
						//msg_envoi.text='a';
						
						generation_id++;
						msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
						nbre_clients++;
						printf("Envoyé\n");
						break;
						
					case 3: // liste produit
						printf("Liste produit\n");
						
						msg_envoi.type=1;
						msg_envoi.id_clt=msg_recu.id_clt; // num	
						msg_envoi.req_clt=0;
						
						msg_envoi.text[0]=' ';
						for(j=0;j<3;j++)
						{
							strcat(msg_envoi.text, liste_produit[j]);
							strcat(msg_envoi.text, " ");
						}
						printf("msg.text=%s\n", msg_envoi.text);
						msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
						break;
						
					case 4: // ajouter n objets
						printf("Ajouter n objets\n");
						
						int n = (int)msg_recu.text[0];
						msg_envoi.type=1;
						msg_envoi.id_clt=msg_recu.id_clt; // num	
						msg_envoi.req_clt=0;
						
						msg_envoi.text[0]=' ';
						for(j=0;j<n;j++)
						{
							strcat(msg_envoi.text, liste_produit[j]);
						}
						printf("msg.text=%s\n", msg_envoi.text);
						msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
						break;
						
				}
				nbre_clients2--;
			} else printf("Sortie else\n");
		}
		
	
	
	
	
}
