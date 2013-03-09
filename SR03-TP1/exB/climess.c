#include <ipc.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <msg.h>


int main(){
	int id_msg;
	int long_msg=sizeof(message)-sizeof(long);
	int id_clt;
	message msg;
	
	key_t key = ftok("./sr03p055.txt", 0);
	
	if(key==-1)
		perror("error ftok");

	id_msg = msgget(key, 0);
	if(id_msg==-1)
	{
		perror("msgget() - pas de file !");	
	}
	else
	{
		// Demande numero client
		msg.type=1; // le serveur ne récupère que les msg de type 1
		msg.req_clt=-1;
		msgsnd(id_msg, (void*)&msg, long_msg, 0);
		msgrcv(id_msg, (void*)&msg, long_msg, 14, 0);
		id_clt=msg.id_clt;

		// Demande liste produit
		msg.type=1;
		msg.req_clt=3;
		msgsnd(id_msg, (void*)&msg, long_msg, 0);
		msgrcv(id_msg, (void*)&msg, long_msg, 14, 0);
	}
}
