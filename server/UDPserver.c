// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../bank.h"
	
#define PORT	 8080
#define MAXLINE 1024
	


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


// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in servaddr, cliaddr;
		
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
		
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
		
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
		
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
		
	int n;
	
	while(1){
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, (socklen_t*) sizeof(cliaddr));
	buffer[n] = '\0';
	printf("Client : %s\n", buffer);


	n=sendto(sockfd, buffer, n, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
	printf("%d\n", n);
	printf("Hello message sent.\n");
	}
	return 0;
}
