#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <memory.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int p[2];
	int pid1, pid2;
	int *status;
	
	if(pipe(p) == -1) printf("Pipe error\n");
	else{
		pid1=fork();
		switch(pid1){
			case -1:
				printf("fork");
				break;

			case 0:
				printf("\nDébut fils1\n");
				// fils 1
				close(p[0]);

				//Ecriture
				FILE* fp = fopen("input.txt","r"); 
				if(fp==NULL) printf("Erreur ouverture ficher\n");
				char buffer[70], buffer2[1024];

				int length;
				printf("\n\nDonnées envoyées par le fils1 : \n");
				while(fgets(buffer,70,fp)!=NULL){
					length = strlen(buffer)-1;
					buffer[length]=NULL;
					
					sprintf(buffer2, ">[%03d][%s]<", length, buffer); 
					
					printf("F1 : |%s| length:%d\n", buffer2,strlen(buffer2));
					write(p[1], buffer2, strlen(buffer2));
				}			
				printf("\nFin fils1\n");
				close(p[1]);
				break;

			default: 
				
				pid2=fork();
				switch(pid2){
					case (pid_t)-1:
						perror("fork");
						break;

					case (pid_t)0:
						// fils 2
						printf("\nDébut fils2\n");
						close(p[1]);

						//Lecture
						char buffer3[20], tmp[20], total[1024]; int res, i;
						printf("\n\nDonnées reçues par le fils2 : \n");
						while(res=read(p[0], buffer3, 20)!=NULL){
							printf("F2 : |%s|\n",buffer3);
							sprintf(total, "%s%s", total, buffer3); 
						}
						
						
						printf("\n\nVariable total : |%s| %d\n",total,strlen(total));
						
						char ligne[70]; int j=0, k;
						
						
						printf("\n\nDonnées mise en forme par le fils2 : \n");
						for (i=0; i<strlen(total); i++) {
							if(total[i]=='>'){
								for(k=0;k<70;k++) ligne[k]=NULL;
								j=0;
								i+=7;
								while (total[i]!='<') {
									ligne[j]=total[i];
									i++;
									j++;
								}
								ligne[strlen(ligne)-1]=NULL;
								printf("Recu>>>>%s<<<<\n",ligne);

							}
						}
						
						
						printf("\nSortie fils2\n");
						close(p[0]);
					break;

					default:
						// pere
						printf("\nDébut père\n");
						close(p[0]);
						close(p[1]);
						waitpid(pid2,status,WUNTRACED);
						waitpid(pid1,status,WUNTRACED);
						printf("\nSortie père\n");
						exit(0);
						break;
				}
		}		
	
	}


}
