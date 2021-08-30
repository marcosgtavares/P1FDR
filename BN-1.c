//REF:https://www.dca.ufrn.br/~adelardo/cursos/DCA409/node117.html
#include <stdlib.h>
#include <stdio.h>
#include "Fila.h"


int main(int argc, char *argv[]) {
	struct msgform *msg = (struct msgform*)malloc(sizeof(struct msgform));    
	int msgidBN,msgidBNmin1;  
	char mensagem[4900];

	msgidBN = abrir_fila(21);
	msgidBNmin1 = criar_fila(23);

	while(1){
		if(receber_arquivo(msg, msgidBNmin1)!=-1){
			printf("Arquivo recebido:%s\n", (*msg).mdata);
		}
		else{
			scanf("%s", mensagem);
			mandar_arquivo(mensagem, msgidBN);
		}
		
	}

	


	return 0;
}
