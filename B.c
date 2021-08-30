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

#include "Fila.h"

#define MAX_MSG 100

int main(int argc, char *argv[]) {
  int sd, rc, i, n, tam_ServA, msgidBN, msgidBNmin1;
  struct sockaddr_in ladoCliB;   /* dados do cliente local   */
  struct sockaddr_in ladoServA; 	/* dados do servidor remoto */
  
  char   msg[MAX_MSG];/* Buffer que armazena os dados que chegaram via rede */

/* confere o numero de argumentos passados para o programa */
  

/* Preenchendo as informacoes de identificacao do remoto */
  ladoCliB.sin_family 	    = AF_INET;
  ladoCliB.sin_addr.s_addr  = inet_addr("0.0.0.0");
  ladoCliB.sin_port 	      = htons(5000);

/* Preenchendo as informacoes de identificacao do cliente */
  ladoServA.sin_family 	    = AF_INET;
  ladoServA.sin_addr.s_addr = inet_addr("0.0.0.0");  
  ladoServA.sin_port 	      = htons(4000); 

/* Criando um socket. Nesse momento a variavel       */
/* sd contem apenas dados sobre familia e protocolo  */

  sd = socket(AF_INET,SOCK_DGRAM,0);
  if(sd<0) {
    printf("%s: n�o pode abrir o socket \n",argv[0]);
    exit(1); }

/* Relacionando o socket sd com a estrutura ladoCliB /*
/* Depois do bind, sd faz referencia a protocolo local, ip local e porta local */
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO,(char *)&tv,sizeof(tv));

  rc = bind(sd, (struct sockaddr *) &ladoCliB, sizeof(ladoCliB));
  if(rc<0) {
    printf("5000: n�o pode fazer um bind da porta\n");
    exit(1); }
  printf("{UDP, IP_Cli: %s, Porta_Cli: %u, IP_R: 127.0.0.1, Porta_R: 4000}\n", inet_ntoa(ladoCliB.sin_addr), ntohs(ladoCliB.sin_port));

	struct msgform *msgfila = (struct msgform*)malloc(sizeof(struct msgform)); 
	msgidBN=criar_fila(21);
	msgidBNmin1=abrir_fila(23);
	char file[100];
	char a;
	int j=0;
	int k=0;
	int fileended=0;

	while(1){
		printf("pass");
		if(receber_arquivo(msgfila, msgidBN)!=-1){
			printf("send%s",(*msgfila).mdata);
			while(fileended==0) { //
				while(a!='\0'){
					a = (*msgfila).mdata[j];
					file[j%MAX_MSG]=(*msgfila).mdata[j];
					j++;
					if(j%MAX_MSG==0 || a == '\0'){
						k++;
						break;
					}
				}
				if(a=='\0'){
					fileended=1;
				}
				//framefile //numero tambem(1,2,3,4 ...)
				//sendframe
				while(1){
					rc = sendto(sd, file, strlen(file), 0,(struct sockaddr *) &ladoServA, sizeof(ladoServA));
					if(rc<0) {
						printf("%s: nao pode enviar dados %d \n",argv[0],i-1);
						close(sd);
						return 1; 
					}
					//setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv));
					n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *) &ladoServA, &tam_ServA);
					printf("%s", msg);
					if(n!=-1 && msg[2]=='K'){
						break;
					}
				}
				
			} /* fim do for (laco) */
			j=0;
			a='1';
			fileended=0;
		}
		else{
			k=0;
			while(1){
				printf("recv\n");
				/* inicia o buffer */
				memset(msg,0x0,MAX_MSG);
				tam_ServA = sizeof(ladoServA);
				/* recebe a mensagem  */
				//setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv));
				printf("recv2\n");
				n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *) &ladoServA, &tam_ServA);
				if(n<0){
					break;
				}
				else if(n>0){
					rc = sendto(sd, "ACK", 3, 0,(struct sockaddr *) &ladoServA, sizeof(ladoServA));
				}
				printf("%d\n", n);
				mandar_arquivo(msg, msgidBNmin1);
				
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
				printf("{UDP, IP_L: %s, Porta_L: %u", inet_ntoa(ladoServA.sin_addr), ntohs(ladoServA.sin_port));
				printf(" IP_R: %s, Porta_R: %u} => %s\n",inet_ntoa(ladoCliB.sin_addr), ntohs(ladoCliB.sin_port), msg);
			}
		
		}
	}
  

  return 1;
} /* fim do programa */
