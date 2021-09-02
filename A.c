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
    exit(1); }

/* Relacionando o socket sd com a estrutura ladoCliA /*
/* Depois do bind, sd faz referencia a protocolo local, ip local e porta local */
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO,(char *)&tv,sizeof(tv));

  rc = bind(sd, (struct sockaddr *) &ladoCliA, sizeof(ladoCliA));
  if(rc<0) {
    printf("%s: n�o pode fazer um bind da porta\n", argv[0]);
    exit(1); }
  	printf("{UDP, IP_Cli: %s, Porta_Cli: %u, IP_R: 127.0.0.1, Porta_R: 5000}\n", inet_ntoa(ladoCliA.sin_addr), ntohs(ladoCliA.sin_port));

	struct msgform *msgfila = (struct msgform*)malloc(sizeof(struct msgform)); 
	msgidAN=criar_fila(20);
	
	char *file = (char*)malloc(sizeof(char)*MAX_MSG);
	char a='1';
	int j=0;
	int k=0;
	int fileended=0;
	int firsttime=0;
	struct pduframe actualframe, actualframe2;
	actualframe.data=(char*)malloc(sizeof(char)*(MAX_MSG-7));
	short crcfchk;
	short sz;

	while(1){
		printf("bigloop\n");
		if(receber_arquivo(msgfila, msgidAN)!=-1){
			printf("Send\n");
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
					sz = (short)(MAX_MSG-7) - actualframe.dsz;

					for(int l=0; l<sz; l++){

						(short)file[5+actualframe.dsz+l];
						actualframe.data[actualframe.dsz+l]='\0';
					}

				}
				actualframe.crc = calcula_CRC((unsigned char *)&actualframe, MAX_MSG-2);
				*(file + 5 + actualframe.dsz + sz) = calcula_CRC((unsigned char *)&file, MAX_MSG-2);

				//framefile //numero tambem(1,2,3,4 ...)
				//sendframe
				printf("%s", actualframe.data);
				while(1){
					rc = sendto(sd, file, MAX_MSG, 0,(struct sockaddr *) &ladoServB, sizeof(ladoServB));
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
			printf("Rec\n");
			k=0;  
			while(1){
				printf("loopRec\n");
				/* inicia o buffer */
				memset(file,0x0,MAX_MSG);
				tam_ServB = sizeof(ladoServB);
				/* recebe a mensagem  */
				//setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv));
				n = recvfrom(sd, file, MAX_MSG, 0, (struct sockaddr *) &ladoServB, &tam_ServB);
				
				crcfchk = calcula_CRC((unsigned char *)file, MAX_MSG-2);

				if(n<0){
					break;
				}
				else if(n>0){
					rc = sendto(sd, "ACK", 3, 0,(struct sockaddr *) &ladoServB, sizeof(ladoServB));
					if(firsttime==0){
						msgidANmin1=abrir_fila(22);
						firsttime=1;
					}
					
					mandar_arquivo(file, msgidANmin1);
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
				

				
				
				/* imprime a mensagem recebida na tela do usuario */
				printf("{UDP, IP_L: %s, Porta_L: %u", inet_ntoa(ladoServB.sin_addr), ntohs(ladoServB.sin_port));
				printf(" IP_R: %s, Porta_R: %u} => %s\n",inet_ntoa(ladoCliA.sin_addr), ntohs(ladoCliA.sin_port), file);
			}
		
		}
	}
  
  

  return 1;
} /* fim do programa */
