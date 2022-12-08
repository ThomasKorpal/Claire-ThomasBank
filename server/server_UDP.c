#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "../bank.h"

#define MAXPENDING 5    /* Max connection requests */
#define BUFFSIZE 450
void Die(char *mess) { perror(mess); exit(1); }

void Ecriture_Solde(char* buffer, operation* op){
  if(op!=NULL){
    strcpy(buffer, "RES_SOLDE ");
    
    char s[6];
    sprintf(s, "%d", op->montant);
    strcat(buffer, s);
    strcat(buffer, " ");
    strcat(buffer, op->date);
  }else{
    strcpy(buffer, "KO");
  }
}

void Ecriture_Operation(char* buffer, Account* compte){
  if(compte!=NULL){
    strcpy(buffer, "RES_OPERATIONS");

    for(int i=0; i<compte->index_archive; i++){
      strcat(buffer, " ");
      strcat(buffer, to_string(compte->archive[i]->type));
      strcat(buffer, " ");
      strcat(buffer, compte->archive[i]->date);
      buffer[strlen(buffer)-1]=' ';

      char s[6];
      sprintf(s, "%d", compte->archive[i]->montant);
      strcat(buffer, s);
    }
  }else{
    strcpy(buffer, "KO");
  }
}

void Recu_banque(char* buffer){

  if(buffer==NULL){
    printf("Probleme de recuperation de la requete\n");
    strcpy(buffer, "KO");
  }else if(strcmp(buffer, "EXIT")){
    const char* delim=" <>";
    char* requete=strtok(buffer, delim);
    int id_client=atoi(strtok(NULL, delim));
    int id_compte=atoi(strtok(NULL, delim));
    char* password=strtok(NULL, delim);

    if(!strcmp(requete, "AJOUT") || !strcmp(requete, "RETRAIT")){
      int som=atoi(strtok(NULL, delim));
      int verif;
      if(!strcmp(requete, "AJOUT")){
        verif=ajout(id_client, id_compte, password, som);
      }else{
        verif=retrait(id_client, id_compte, password, som);
      }

      if(verif){
        strcpy(buffer, "OK");
      }else{
        strcpy(buffer, "KO");
      }

    }else if(!strcmp(requete, "SOLDE")){
      operation* op=solde(id_client, id_compte, password);
      Ecriture_Solde(buffer, op);

    }else if(!strcmp(requete, "OPERATIONS")){
      Account* compte=operations(id_client, id_compte, password);
      Ecriture_Operation(buffer, compte);

    }else{
      printf("Probleme de comprehension de requete");
      strcpy(buffer, "KO");
    }
  }
  print_comptes(); 
}


void HandleClient(int sock, struct sockaddr_in echoclient) {
  char buffer[BUFFSIZE];
  int received = -1;


  for(;;){
    if((received=recvfrom(sock, buffer, BUFFSIZE, 0, (struct sockaddr*) &echoclient, (socklen_t*) sizeof(echoclient)))<0){
      Die("Failed to receive additional bytes from client");
    }

    printf("Recu : %s\n", buffer);

    if(sendto(sock, buffer, received, 0, (struct sockaddr *) &echoclient, sizeof(echoclient))!=received){
      Die("Failed to send bytes to client");
    }

    printf("Envoi : %s\n", buffer);

  }
}



int main(int argc, char *argv[]) {

  init_bank();
  print_comptes();

  int serversock;
  struct sockaddr_in echoserver, echoclient;

  if (argc != 2) {
    fprintf(stderr, "USAGE: echoserver <port>\n");
    exit(1);
  }
  // Create the TCP socket
  if ((serversock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    Die("Failed to create socket");
  }
  

  memset(&echoclient, 0, sizeof(echoclient));
  // Construct the server sockaddr_in structure
  memset(&echoserver, 0, sizeof(echoserver));       // Clear struct
  echoserver.sin_family = AF_INET;                  // Internet/IP 
  echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   // Incoming addr
  echoserver.sin_port = htons(atoi(argv[1]));       // server port


  // Bind the server socket
  if (bind(serversock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
    Die("Failed to bind the server socket");
  }

/*
  // Listen on the server socket
  if (listen(serversock, MAXPENDING) < 0) {
    Die("Failed to listen on server socket");
  }
*/

 /* 
  // Run until cancelled
  while (1) {
    unsigned int clientlen = sizeof(echoclient);
    // Wait for client connection
    if ((clientsock = accept(serversock, (struct sockaddr *) &echoclient, &clientlen)) < 0) {
      Die("Failed to accept client connection");
    }
    fprintf(stdout, "Client connected: %s\n", inet_ntoa(echoclient.sin_addr));
    HandleClient(clientsock);
  }
*/

  HandleClient(serversock, echoclient);
}