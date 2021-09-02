//REF:https://www.dca.ufrn.br/~adelardo/cursos/DCA409/node117.html
#include <stdlib.h>
#include <stdio.h>
#include "Fila.h"
#include <sys/time.h>
#include <signal.h>

int msgidAN,msgidANmin1;

void end_exec(int sigint){
	killfila(msgidAN);
	killfila(msgidANmin1);
    exit(0);
}

int main(int argc, char *argv[]) {
	signal(SIGINT, end_exec);
  	signal(SIGTSTP, end_exec);

	struct msgform *msg = (struct msgform*)malloc(sizeof(struct msgform));    
	char mensagem[4900];

	struct timeval tv;
	fd_set fds;
	int fdstdin = fileno(stdin);
	tv.tv_sec = 1;
  	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(fileno(stdin), &fds);
	fflush(stdout);
	select(fdstdin+1, &fds, NULL, NULL, &tv);

	msgidAN = abrir_fila(20);
	msgidANmin1 = criar_fila(22);

	while(1){
		if(receber_arquivo(msg, msgidANmin1)!=-1){
			printf("Arquivo recebido:%s\n", (*msg).mdata);
		}
		else{
			scanf("%s", mensagem);
			printf("\n");
			mandar_arquivo(mensagem, msgidAN);
		}
		
	}
	
	


	return 0;
}
