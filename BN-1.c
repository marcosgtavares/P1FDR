//REF:https://www.dca.ufrn.br/~adelardo/cursos/DCA409/node117.html
#include <stdlib.h>
#include <stdio.h>
#include "Fila.h"
#include <sys/time.h>
#include <signal.h>

int msgidBN,msgidBNmin1;

void end_exec(int sigint){
    killfila(msgidBN);
	killfila(msgidBNmin1);
    exit(0);
}

int main(int argc, char *argv[]) {
	signal(SIGINT, end_exec);
  	signal(SIGTSTP, end_exec);

	struct msgform *msg = (struct msgform*)malloc(sizeof(struct msgform));    
	char mensagem[4900];
	struct timeval tv;
	fd_set fds;
	tv.tv_sec = 1;
  	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	select(0+1, &fds, NULL, NULL, &tv);

	msgidBN = abrir_fila(21);
	msgidBNmin1 = criar_fila(23);

	while(1){
		if(receber_arquivo(msg, msgidBNmin1)!=-1){
			printf("Arquivo recebido:%s\n", (*msg).mdata);
		}
		else{
			scanf("%s", mensagem);
			printf("\n");
			mandar_arquivo(mensagem, msgidBN);
		}
		
	}

	return 0;
}
