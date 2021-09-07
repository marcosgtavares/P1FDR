//REF:https://www.dca.ufrn.br/~adelardo/cursos/DCA409/node117.html
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "Fila.h"
#include <sys/time.h>
#include <signal.h>
#include <string.h>

int msgidAN,msgidANmin1;

struct msgform msg;


void end_exec(int sigint){
	killfila(msgidAN);
	killfila(msgidANmin1);
    exit(0);
}

void receive_f(int siglrm){
	short fsz;
	alarm(1);
	if(receber_arquivo(&msg, msgidANmin1)!=-1){
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

	msgidAN = abrir_fila(20);
	msgidANmin1 = criar_fila(22);
	alarm(1);
	while(1){
		printf("Insira uma mensagem para enviar para B:");
		scanf("%s", mensagem);
		printf("\n");
		mandar_arquivo(mensagem, msgidAN, strlen(mensagem)+1);
		printf("\nArquivo enviado para a fila:%s\n", mensagem);
	}
	
	return 0;
}
