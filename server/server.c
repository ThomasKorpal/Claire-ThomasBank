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


void Recu_banque(char* buffer){

  if(buffer==NULL){
    printf("Probleme de recuperation de la requete\n");
    strcpy(buffer, "KO");
    exit(1);
  }
  const char* delim=" <>";
  char* requete=strtok(buffer, delim);
  char* id=strtok(NULL, delim);
  char* compte=strtok(NULL, delim);
  char* password=strtok(NULL, delim);
  char* somme;

  int id_client=atoi(id);
  int id_compte=atoi(compte);

  printf("Requete : %s, Id_client : %d, id_compte : %d, Password : %s\n", requete, id_client, id_compte, password);

  if(!strcmp(requete, "AJOUT") || !strcmp(requete, "RETRAIT")){
    somme=strtok(NULL, delim);
    int som=atoi(somme);
    printf("Somme : %d\n", som);
    if(!strcmp(requete, "AJOUT")){
      ajout(id_client, id_compte, password, som);
    }else{
      retrait(id_client, id_compte, password, som);
    }
    strcpy(buffer, "OK");

  }else if(!strcmp(requete, "SOLDE")){
    operation* op=solde(id_client, id_compte, password);
    strcpy(buffer, "RES_SOLDE ");
    
    char s[6];
    sprintf(s, "%d", op->montant);
    strcat(buffer, s);
    strcat(buffer, " ");
    strcat(buffer, op->date);

  }else if(!strcmp(requete, "OPERATIONS")){
    Account* compte=operations(id_client, id_compte, password);
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
    printf("Probleme de comprehension de requete");
    strcpy(buffer, "KO");
  } 
}


void HandleClient(int sock) {
  char buffer[BUFFSIZE];
  int received = -1;
  /* Receive message */
  if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
    Die("Failed to receive initial bytes from client");
  }
  buffer[received]='\0';
  printf("Recu du buffer 1 : %s\n", buffer);

// Send bytes and check for more incoming data in loop
  while (received > 0) {
  // Send back received data 
    if (send(sock, buffer, received, 0) != received) {
      Die("Failed to send bytes to client");
    }
    printf("Envoi du buffer : %s\n", buffer);

    //Check for more data
    if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
      Die("Failed to receive additional bytes from client");
    }
    buffer[received]='\0';
    printf("Recu du buffer : %s\n", buffer);

    Recu_banque(buffer);
    printf("Modification du buffer : %s\n", buffer);
    received=strlen(buffer);

    print_comptes();
  }

/*
  // Send bytes and check for more incoming data in loop
  while (received > 0) {
    //Recu_banque(buffer);
    //printf("Modification du buffer : %s", buffer);
  // Send back received data 
    if (send(sock, buffer, received, 0) != received) {
      Die("Failed to send bytes to client");
    }
    printf("Envoi du buffer : %s\n", buffer);
    //Check for more data
    if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
      Die("Failed to receive additional bytes from client");
    }
    buffer[received]='\0';
    printf("Recu du buffer : %s\n", buffer);
  }
*/

  printf("Fermeture de la connexion du client\n");
  freeListClients();
  close(sock);
}



int main(int argc, char *argv[]) {

  init_bank();
  print_comptes();

  int serversock, clientsock;
  struct sockaddr_in echoserver, echoclient;

  if (argc != 2) {
    fprintf(stderr, "USAGE: echoserver <port>\n");
    exit(1);
  }
  /* Create the TCP socket */
  if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    Die("Failed to create socket");
  }
  /* Construct the server sockaddr_in structure */
  memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
  echoserver.sin_family = AF_INET;                  /* Internet/IP */
  echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
  echoserver.sin_port = htons(atoi(argv[1]));       /* server port */


  /* Bind the server socket */
  if (bind(serversock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
    Die("Failed to bind the server socket");
  }
  /* Listen on the server socket */
  if (listen(serversock, MAXPENDING) < 0) {
    Die("Failed to listen on server socket");
  }

  /* Run until cancelled */
  while (1) {
    unsigned int clientlen = sizeof(echoclient);
    /* Wait for client connection */
    if ((clientsock = accept(serversock, (struct sockaddr *) &echoclient, &clientlen)) < 0) {
      Die("Failed to accept client connection");
    }
    fprintf(stdout, "Client connected: %s\n", inet_ntoa(echoclient.sin_addr));
    HandleClient(clientsock);
  }
}
