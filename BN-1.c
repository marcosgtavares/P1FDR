//REF:https://www.dca.ufrn.br/~adelardo/cursos/DCA409/node117.html
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Fila.h"
#include <sys/time.h>
#include <signal.h>

int msgidBN,msgidBNmin1;

struct msgform *msg;

void end_exec(int sigint){
    killfila(msgidBN);
	killfila(msgidBNmin1);
    exit(0);
}

void receive_f(int siglrm){
	alarm(1);
	if(receber_arquivo(msg, msgidBNmin1)!=-1){
		printf("Pacote recebido:%s\n", (msg->mdata + 5));
	}
}

int main(int argc, char *argv[]) {
	signal(SIGINT, end_exec);
  	signal(SIGTSTP, end_exec);
	signal(SIGALRM, receive_f);

	msg = (struct msgform*)malloc(sizeof(struct msgform));    
	char mensagem[4092];

	msgidBN = abrir_fila(21);
	msgidBNmin1 = criar_fila(23);
	alarm(1);
	while(1){
		scanf("%s", mensagem);
		printf("\n");
		mandar_arquivo(mensagem, msgidBN);
	}

	return 0;
}
