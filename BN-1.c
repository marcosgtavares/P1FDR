//REF:https://www.dca.ufrn.br/~adelardo/cursos/DCA409/node117.html
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Fila.h"
#include <sys/time.h>
#include <signal.h>
#include <string.h>

int msgidBN,msgidBNmin1;

struct msgform msg;

void end_exec(int sigint){
    killfila(msgidBN);
	killfila(msgidBNmin1);
    exit(0);
}

void receive_f(int siglrm){
	short fsz;
	alarm(1);
	if(receber_arquivo(&msg, msgidBNmin1)!=-1){
		fsz = (short)*(msg.mdata + 3);
		printf("\nQuadro recebido:");
		for(int i=0;i<(int)fsz;i++){
			printf("%c", msg.mdata[i+5]);
		}
		printf("\n");
		
	}
}

int main(int argc, char *argv[]) {
	signal(SIGINT, end_exec);
  	signal(SIGTSTP, end_exec);
	signal(SIGALRM, receive_f);

	char mensagem[4092];

	msgidBN = abrir_fila(21);
	msgidBNmin1 = criar_fila(23);
	alarm(1);
	while(1){
		printf("Insira uma mensagem para enviar para A:");
		scanf("%s", mensagem);
		printf("\n");
		mandar_arquivo(mensagem, msgidBN, strlen(mensagem)+1);
		printf("\nArquivo enviado para a fila:%s\n", mensagem);
	}


	return 0;
}
