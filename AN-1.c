//REF:https://www.dca.ufrn.br/~adelardo/cursos/DCA409/node117.html
#include <stdlib.h>
#include <stdio.h>
#include "Fila.h"


int main(int argc, char *argv[]) {
	struct msgform *msg = (struct msgform*)malloc(sizeof(struct msgform));    
	int msgidAN,msgidANmin1;  
	char mensagem[4900];

	msgidAN = abrir_fila(20);
	msgidANmin1 = criar_fila(22);

	while(1){
		if(receber_arquivo(msg, msgidANmin1)!=-1){
			printf("Arquivo recebido:%s\n", (*msg).mdata);
		}
		else{
			scanf("%s", mensagem);
			mandar_arquivo(mensagem, msgidAN);
		}
		
	}

	


	return 0;
}
