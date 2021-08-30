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

#define MAX_MSG 100

int main(int argc, char *argv[]) {
  int sd, rc, i, n, tam_ServA;
  struct sockaddr_in ladoCliB;   /* dados do cliente local   */
  struct sockaddr_in ladoServA; 	/* dados do servidor remoto */
  
  char   msg[MAX_MSG];/* Buffer que armazena os dados que chegaram via rede */

/* confere o numero de argumentos passados para o programa */
  if(argc<3)  {
    printf("uso correto: %s <ip_do_servidor> <porta_do_servidor> <dado1> ... <dadoN> \n", argv[0]);
    exit(1);  }

/* Preenchendo as informacoes de identificacao do remoto */
  ladoCliB.sin_family 	    = AF_INET;
  ladoCliB.sin_addr.s_addr  = inet_addr("127.0.0.1");
  ladoCliB.sin_port 	      = htons(5000);

/* Preenchendo as informacoes de identificacao do cliente */
  ladoServA.sin_family 	    = AF_INET;
  ladoServA.sin_addr.s_addr = inet_addr("127.0.0.1");  
  ladoServA.sin_port 	      = htons(4000); 

/* Criando um socket. Nesse momento a variavel       */
/* sd contem apenas dados sobre familia e protocolo  */

  sd = socket(AF_INET,SOCK_DGRAM,0);
  if(sd<0) {
    printf("%s: n�o pode abrir o socket \n",argv[0]);
    exit(1); }

/* Relacionando o socket sd com a estrutura ladoCliB /*
/* Depois do bind, sd faz referencia a protocolo local, ip local e porta local */
  rc = bind(sd, (struct sockaddr *) &ladoCliB, sizeof(ladoCliB));
  if(rc<0) {
    printf("5000: n�o pode fazer um bind da porta\n");
    exit(1); }
  printf("{UDP, IP_Cli: %s, Porta_Cli: %u, IP_R: 127.0.0.1, Porta_R: 4000}\n", inet_ntoa(ladoCliB.sin_addr), ntohs(ladoCliB.sin_port));

	while(1){
		if(fila>0){
			getfileonline
			while(!fileended) { //
				framefile //numero tambem(1,2,3,4 ...)
				//sendframe
				rc = sendto(sd, argv[i], strlen(argv[i]), 0,(struct sockaddr *) &ladoServB, sizeof(ladoServB));
				if(rc<0) {
					printf("%s: nao pode enviar dados %d \n",argv[0],i-1);
					close(sd);
					exit(1); }
				printf("Enviando parametro %d: %s\n", i-2, argv[i]);
				if(ackintime){
					frame++
					if(endfile){
						--fila
					}
				}
			} /* fim do for (laco) */
		}
		else{
			while(1){
				/* inicia o buffer */
				memset(msg,0x0,MAX_MSG);
				tam_ServB = sizeof(ladoServB);
				/* recebe a mensagem  */
				n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *) &ladoServB, &tam_ServB);
				if(crc){
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
				}
				
				/* imprime a mensagem recebida na tela do usuario */
				printf("{UDP, IP_L: %s, Porta_L: %u", inet_ntoa(ladoServB.sin_addr), ntohs(ladoServB.sin_port));
				printf(" IP_R: %s, Porta_R: %u} => %s\n",inet_ntoa(ladoCliA.sin_addr), ntohs(ladoCliA.sin_port), msg);
			}
		
		}
	}
  

  return 1;
} /* fim do programa */
