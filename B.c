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

  
	/* inicia o buffer */
  memset(msg,0x0,MAX_MSG);
  tam_ServA = sizeof(ladoServA);
  /* recebe a mensagem  */
  n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *) &ladoServA, &tam_ServA);
    
  /* imprime a mensagem recebida na tela do usuario */
  printf("{UDP, IP_L: %s, Porta_L: %u", inet_ntoa(ladoServA.sin_addr), ntohs(ladoServA.sin_port));
  printf(" IP_R: %s, Porta_R: %u} => %s\n",inet_ntoa(ladoCliB.sin_addr), ntohs(ladoCliB.sin_port), msg);

	/* Enviando um pacote para cada parametro informado */
  for(i=1;i<argc;i++) {
    rc = sendto(sd, argv[i], strlen(argv[i]), 0,(struct sockaddr *) &ladoServA, sizeof(ladoServA));
    if(rc<0) {
      printf("%s: nao pode enviar dados %d \n",argv[0],i-1);
      close(sd);
      exit(1); }
    printf("Enviando parametro %d: %s\n", i-2, argv[i]);
  } /* fim do for (laco) */

  

  return 1;
} /* fim do programa */
