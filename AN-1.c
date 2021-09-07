//REF:https://www.dca.ufrn.br/~adelardo/cursos/DCA409/node117.html
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "Fila.h"
#include <sys/time.h>
#include <signal.h>

int msgidAN,msgidANmin1;

struct msgform *msg;


void end_exec(int sigint){
	killfila(msgidAN);
	killfila(msgidANmin1);
    exit(0);
}

void receive_f(int siglrm){
	alarm(1);
	if(receber_arquivo(msg, msgidANmin1)!=-1){
		printf("Pacote recebido:%s\n", (msg->mdata + 5));
	}
}

int main(int argc, char *argv[]) {
	signal(SIGINT, end_exec);
  	signal(SIGTSTP, end_exec);
	signal(SIGALRM, receive_f);

	msg = (struct msgform*)malloc(sizeof(struct msgform));    
	char mensagem[4092];

	msgidAN = abrir_fila(20);
	msgidANmin1 = criar_fila(22);
	alarm(1);
	while(1){
		scanf("%s", mensagem);
		printf("\n");
		mandar_arquivo(mensagem, msgidAN);
	}
	
	return 0;
}
