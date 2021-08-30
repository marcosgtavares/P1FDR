//REF:https://www.dca.ufrn.br/~adelardo/cursos/DCA409/node117.html

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#include "./Fila.h"

int criar_fila(int MSGKEY){
    return msgget(MSGKEY, 0600|IPC_CREAT);
}

int abrir_fila(int MSGKEY){
    return msgget(MSGKEY, 0); 
}

void mandar_arquivo(char *file, int msgid){
    struct msgform msg;

    msg.pid = getpid();
	msg.mtype = 2;
    strcpy(msg.mdata, file);

    printf("%s",msg.mdata);

    if (msgsnd(msgid, &msg, MSG_SIZEMAX, 0) == -1) {
        printf("Erro no envio da mensagem") ;
    }
}

void receber_arquivo(struct msgform *msg, int msgid){
    if (msgrcv(msgid, msg, MSG_SIZEMAX,2, 0) == -1) {
        perror("Erro na recepcao da mensagem") ;
    }
    
}