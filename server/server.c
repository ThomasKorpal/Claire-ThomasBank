#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "../bank.h"

#define MAXPENDING 5    /* Max connection requests */
#define BUFFSIZE 1000
#define PORT 2000

void Die(char *mess) { perror(mess); exit(1); }


//Ecriture dans le buffer pour une requete de solde
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

//Ecriture dans le buffer pour une requete d'operation
void Ecriture_Operation(char* buffer, Account* compte){
  if(compte!=NULL){
    strcpy(buffer, "RES_OPERATIONS");

    for(int i=0; i<compte->index_archive; i++){
      strcat(buffer, " ");
      strcat(buffer, to_string(compte->archive[i]->type));
      strcat(buffer, " ");
      strcat(buffer, compte->archive[i]->date);
      //buffer[strlen(buffer)-1]=' ';

      char s[6];
      sprintf(s, "%d", compte->archive[i]->montant);
      strcat(buffer, s);
    }
  }else{
    strcpy(buffer, "KO");
  }
}

//Fonction de traitement de la requete du client
void Recu_banque(char* buffer){

  //Traitement de la requete seulement si le client ne demande pas de quitter ou si le buffer non vide
  if(strcmp(buffer, "EXIT") && strcmp(buffer, "")){
    const char* delim=" <>"; //les caracteres qui permettent de separer les informations bancaires du client

    //Fragmentation de la requete
    char* requete=strtok(buffer, delim);
    int id_client=atoi(strtok(NULL, delim));
    int id_compte=atoi(strtok(NULL, delim));
    char* password=strtok(NULL, delim);

    //Traitement de la requete selon l'action demandee
    if(!strcmp(requete, "AJOUT") || !strcmp(requete, "RETRAIT")){
      int som=atoi(strtok(NULL, delim)); //somme souhaitee par le client
      int verif;

      if(!strcmp(requete, "AJOUT")){
        verif=ajout(id_client, id_compte, password, som); //Ajout de la somme d'argent dans le compte client voulue
      }else{
        verif=retrait(id_client, id_compte, password, som); //Retrait de la somme du compte client souhaitee
      }

      //Verification si le changement dans le compte banciares c'est bien passee
      if(verif){
        strcpy(buffer, "OK");
      }else{
        strcpy(buffer, "KO");
      }

    }else if(!strcmp(requete, "SOLDE")){
      operation* op=solde(id_client, id_compte, password); //Recuperation des informations souhaitée (date, somme dans le compte)
      Ecriture_Solde(buffer, op);                          //Ecriture dans le buffer pour envoyer les informations demandee

    }else if(!strcmp(requete, "OPERATIONS")){
      Account* compte=operations(id_client, id_compte, password); //Recuperation de toutes les operations passee sur le compte
      Ecriture_Operation(buffer, compte);                         //Ecriture sur le buffer pour envoyee toutes les operations

    }else{ //Si la requte n'est pas comprise
      strcpy(buffer, "KO");
    }
  }
  print_comptes(); //Affichage apres chaque modification des comptes
}


//Fonction permettant de l'envoi et reception de messages selon les requetes du client
void HandleClient(int sock) {
  char buffer[BUFFSIZE]="";
  int received = 1;

  //Tourne en boucle tant que le client envoie des donnees ou ne demande pas de quitter
  while (received > 0 && strcmp(buffer, "EXIT")) {

    //Reception de la requete du client
    if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {
      Die("Failed to receive additional bytes from client");
    }
    buffer[received]='\0';  //Ajout du fin de caractere
    printf("Recu du buffer : %s\n", buffer);

    //Traitement de la requete du client
    Recu_banque(buffer);
    received=strlen(buffer);

    //Envoie de reponse a la requete du client
    if (send(sock, buffer, received, 0) != received) {
      Die("Failed to send bytes to client");
    }
    printf("Envoi du buffer : %s\n", buffer);
  }

  printf("Fermeture de la connexion du client\n");
  close(sock);
}



int main() {

  //Creation des comptes bancaires des clients et affichage des comptes
  init_bank();
  print_comptes();

  int serversock, clientsock;
  struct sockaddr_in echoserver, echoclient;

  //Creation du socket server
  if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    Die("Failed to create socket");
  }

  //Construction du sockaddr_in du server
  memset(&echoserver, 0, sizeof(echoserver));       // Clear struct
  echoserver.sin_family = AF_INET;                  // Internet/IP 
  echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   // Incoming addr
  echoserver.sin_port = htons(PORT);                // server port


  // Bind le socket server
  if (bind(serversock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
    Die("Failed to bind the server socket");
  }

  // Listen d'une connection sur le server socket
  if (listen(serversock, MAXPENDING) < 0) {
    Die("Failed to listen on server socket");
  }

  //Le server tourne en boucle en attendant une connection
  while (1) {
    unsigned int clientlen = sizeof(echoclient);
    //Connection avec le client
    if ((clientsock = accept(serversock, (struct sockaddr *) &echoclient, &clientlen)) < 0) {
      Die("Failed to accept client connection");
    }
    fprintf(stdout, "Client connected: %s\n", inet_ntoa(echoclient.sin_addr));

    //Debut de l'echange avec le client
    HandleClient(clientsock);
  }

  //Desallouer tous les comptes bancaires cree
  freeListClients();
}