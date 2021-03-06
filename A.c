/***********************************/
/* FGA/EngSofware/FRC  udpClient.c */
/* Prof. Fernando W Cruz           */
/***********************************/ 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>


#include "Fila.h"
#include "crc16.h"
int sd;

void end_exec(int sigint){
    close(sd);
    exit(0);
}

struct pduframe{
	char fm;
	short cnt;
	short dsz;
	char *data;
	short crc;
};

int sim_err(int odd){
	srand(time(NULL));
	if (rand()%odd == odd-1){
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	signal(SIGINT, end_exec);
	signal(SIGTSTP, end_exec);

	int rc, i, n, tam_ServB, msgidAN, msgidANmin1;
	struct sockaddr_in ladoCliA;   /* dados do cliente local   */
	struct sockaddr_in ladoServB; 	/* dados do servidor remoto */
	
	int MAX_MSG = atoi(argv[1]);

  	char   msg[MAX_MSG];/* Buffer que armazena os dados que chegaram via rede */

	/* confere o numero de argumentos passados para o programa */

	/* Preenchendo as informacoes de identificacao do remoto */
	ladoServB.sin_family 	   = AF_INET;
	ladoServB.sin_addr.s_addr = inet_addr("0.0.0.0");
	ladoServB.sin_port 	     = htons(5000);

	/* Preenchendo as informacoes de identificacao do cliente */
	ladoCliA.sin_family 	     = AF_INET;
	ladoCliA.sin_addr.s_addr  = inet_addr("0.0.0.0");  
	ladoCliA.sin_port 	       = htons(4000); 

	/* Criando um socket. Nesse momento a variavel       */
	/* sd contem apenas dados sobre familia e protocolo  */

	sd = socket(AF_INET,SOCK_DGRAM,0);
	if(sd<0) {
		printf("%s: n�o pode abrir o socket \n",argv[0]);
		exit(1); 
	}

	/* Relacionando o socket sd com a estrutura ladoCliA /*
	/* Depois do bind, sd faz referencia a protocolo local, ip local e porta local */
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO,(char *)&tv,sizeof(tv));

  	rc = bind(sd, (struct sockaddr *) &ladoCliA, sizeof(ladoCliA));
  	if(rc<0) {
		printf("%s: n�o pode fazer um bind da porta\n", argv[0]);
		exit(1); 
	}
  	printf("{UDP, IP_Cli: %s, Porta_Cli: %u, IP_R: 0.0.0.0, Porta_R: 5000}\n", inet_ntoa(ladoCliA.sin_addr), ntohs(ladoCliA.sin_port));

	struct msgform *msgfila = (struct msgform*)malloc(sizeof(struct msgform)); 
	msgidAN=criar_fila(20);
	
	unsigned char *file = (unsigned char*)malloc(sizeof(char)*MAX_MSG);
	char a='1';
	int j=0;
	int k=0;
	int fileended=0;
	int firsttime=0;
	struct pduframe actualframe;
	actualframe.data=(unsigned char*)malloc(sizeof(char)*(MAX_MSG-7));
	unsigned char crcfchk[2];
	int sz;
	short recyet[100];
	int found=0;
	int m=0;
	for(int i=0;i<100;i++){
		recyet[i]=0;
	}

	while(1){
		if(receber_arquivo(msgfila, msgidAN)!=-1){
			while(fileended==0) { //
				while(a!='\0'){
					a = (*msgfila).mdata[j];
					file[5+j%(MAX_MSG-7)]=(*msgfila).mdata[j];
					actualframe.data[j%(MAX_MSG-7)]=(*msgfila).mdata[j];
					j++;
					if(j%(MAX_MSG-7)==0 || a == '\0'){
						actualframe.fm = 'm';
						file[0]='m';
						k++;
						if(k==1){
							actualframe.fm = 'f';
							file[0]='f';
						}
						actualframe.cnt=(short)k;
						*(file + 1)=(short)k;
						break;
					}
					
				}
				if(a=='\0'){
					fileended=1;
				}
				actualframe.dsz = (short)strlen(actualframe.data);
				*(file + 3)=(short)strlen(actualframe.data);

				if(actualframe.dsz<(short)(MAX_MSG-7)){
					sz = (MAX_MSG-7) - (int)actualframe.dsz;

					for(int l=0; l<sz; l++){

						file[5+actualframe.dsz+l]='\0';
						actualframe.data[actualframe.dsz+l]='\0';
					}

				}
				actualframe.crc = calcula_CRC((unsigned char *)file, MAX_MSG-2);
				*(file + MAX_MSG-2) = calcula_CRC((unsigned char *)file, MAX_MSG-2);


				//framefile //numero tambem(1,2,3,4 ...)
				//sendframe
				while(1){
					if(sim_err(3)==1){
						printf("\nCRC ERR\n");
						*(file + MAX_MSG-2)=(short)222;
					}
					else{
						*(file + MAX_MSG-2) = calcula_CRC((unsigned char *)file, MAX_MSG-2);
					}
					if(sim_err(3)==0){
						rc = sendto(sd, file, MAX_MSG, 0,(struct sockaddr *) &ladoServB, sizeof(ladoServB));
					}
					else{
						printf("\nSENDDATA ERR\n");
					}

					if(rc<0) {
						printf("%s: nao pode enviar dados %d \n",argv[0],i-1);
						close(sd);
						return 1; 
					}
					//setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv));
					n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *) &ladoServB, &tam_ServB);
					if(n!=-1 && msg[2]=='K'){
						break;
					}
				}
				
			} /* fim do for (laco) */
			k=0;
			j=0;
			a='1';
			fileended=0;
		}
		else{
			k=0;  
			while(1){
				/* inicia o buffer */
				memset(file,0x0,MAX_MSG);
				tam_ServB = sizeof(ladoServB);
				/* recebe a mensagem  */
				//setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv));
				n = recvfrom(sd, file, MAX_MSG, 0, (struct sockaddr *) &ladoServB, &tam_ServB);
				
				*crcfchk = calcula_CRC((unsigned char *)file, MAX_MSG-2);

				if(n<0){
					break;
				}
				else if(n>0 && *(file + MAX_MSG-2)==*crcfchk){
					if(sim_err(3)==0){
						rc = sendto(sd, "ACK", 3, 0,(struct sockaddr *) &ladoServB, sizeof(ladoServB));
					}
					else{
						printf("\nACK ERR\n");
					}
					
					if(firsttime==0){
						msgidANmin1=abrir_fila(22);
						firsttime=1;
					}

					if(file[0]=='f'){
						for(int i=0;i<100;i++){
							recyet[i]=0;
						}
						m=0;
					}
					for(int i=0; i<100; i++){
						if((short)*(file + 1)==recyet[i]){
							found = 1;
							break;
						}
						if(recyet[i]==0){
							break;
						}
					}
					if(found!=1){
						recyet[m]=(short)*(file + 1);
						mandar_arquivo(file, msgidANmin1, MAX_MSG);
						m++;
					}
					else{
						found=0;
					}
					
					
				}
				

				/*if(crc){
					if(notreadyet){
						addtofile
						sendack
						if(msg == endfile || msg == lastfile){
							sendfilen-1
						}
						if(msg == lastfile){ //se o frame tiver indicador de fim de arquivo
							break;
						}
					}
				}*/
				

				
				
			}
		
		}
	}
  
  

  return 1;
} /* fim do programa */
