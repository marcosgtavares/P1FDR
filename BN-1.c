//REF:https://www.dca.ufrn.br/~adelardo/cursos/DCA409/node117.html

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#define MSGKEY     13 

struct msgform {
     long     mtype;
     int      pid;
     char     *mdata;
} ;


int main(int argc, char *argv[]) {
	struct msgform msg;    
	int msgid;  

	int fd = fopen("/A.c", "r");

	char buf[4096];
  ssize_t n;
  size_t len = 0;
  while (n = read(STDIN_FILENO, buf, sizeof buf)){
    if (n < 0) {
			if (errno == EAGAIN){
				continue;
				perror("read");
				break;
			}
			str = realloc(msg.mdata, len + n + 1);
			memcpy(msg.mdata + len, buf, n);
			len += n;
			str[len] = '\0';
    }
}
	/*  recuperacao do id da fila de mensagens do servidor       */
	if ((msgid = msgget(MSGKEY,0)) == -1) {
    printf("Erro na criacao da fila do servidor");
    exit(1);
  }

	msg.mdata = malloc(sizeof(char)*)
	msg.pid = getpid();
	msg.mtype = TYPE_CLIENT;

	if (msgsnd(msgid, &msg, len, 0) == -1) {
    printf("Erro no envio da mensagem") ;
    exit(1) ;
  }

	return 0;
}
