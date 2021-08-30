#ifndef FILA_H_
#define FILA_H_

#define MSG_SIZE_TEXT 4096  /* tamanho do texto das mensagens  */

#define MSG_SIZEMAX 4100   /* MSG_SIZE_TEXT + sizeof(int)     */

struct msgform {
     long     mtype;
     int      pid;
     char     mdata[MSG_SIZE_TEXT];
};

int criar_fila(int MSGKEY);
int abrir_fila(int MSGKEY);
void mandar_arquivo(char *file, int msgid);
void receber_arquivo(struct msgform *msg, int msgid);

#endif