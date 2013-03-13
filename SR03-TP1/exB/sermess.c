#include "header.h"




int main(){
	
	int id_msg, long_msg=sizeof(message)-sizeof(long);
	int i,j;
	
	int nbre_clients=0, nbre_clients2=0;
	int generation_id=2;
	
	message msg_recu, msg_envoi;
	
	//produit liste_produit[]={
//		"carotte","pizza","navet","potimarron","raviolis","patate",".",".",".","."};

	produit p1={0, "carotte", 5, 1.2}, p2={1, "pizza", 10, 3.5}, p3={2, "raviolis", 6, 2.5},
	p4={3, "navet", 1, 1.7}, p5={4, "patate", 20, 0.2};
	produit liste_produit[]={p1,p2,p3,p4,p5};
	
	
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
			nbre_clients++;
			if(msgrcv(id_msg, (void*)&msg_recu, long_msg, 1, 0)==-1)
				printf("erreur msgrcv\n");
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
						
						
						msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
						nbre_clients++;
						printf("Envoyé : %d\n",generation_id);
						generation_id++;
						break;
						
					case 3: // liste produit
						printf("Liste produit\n");
						
						msg_envoi.type=msg_recu.id_clt;
						msg_envoi.id_clt=msg_recu.id_clt; // num	
						msg_envoi.req_clt=0;
						
						for(j=0;j<MAX_ELT;j++)
							strcpy(msg_envoi.text[j], liste_produit[j].description);
						
						printf("Envoi de la liste %s\n", msg_envoi.text[2]);
						msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
						break;
						
					case 5: // détails pour un produit
						printf("Détail produit\n");
						msg_envoi.type=msg_recu.id_clt;
						msg_envoi.id_clt=msg_recu.id_clt; // num	
						msg_envoi.req_clt=0;
						
						
						produit p=liste_produit[msg_recu.id_produit];
						char tmp[50];
						sprintf(msg_envoi.text2,"Produit %s -> prix : %.1f€, %d en stock\n",p.description, p.prix, p.en_stock); 
								
						msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
						break;
						
					case 4: // ajouter n objets
						printf("Ajouter n objets\n");
						
						//int n = (int)msg_recu.text[0];
//						msg_envoi.type=1;
//						msg_envoi.id_clt=msg_recu.id_clt; // num	
//						msg_envoi.req_clt=0;
//						
//						msg_envoi.text[0]=' ';
//						for(j=0;j<n;j++)
//						{
//							strcat(msg_envoi.text, liste_produit[j]);
//						}
//						printf("msg.text=%s\n", msg_envoi.text);
//						msgsnd(id_msg, (void*)&msg_envoi, long_msg, 0);
//						break;
						
				}
				nbre_clients2--;
			} else printf("Sortie else\n");
		}
		
	
	
	
	
}
