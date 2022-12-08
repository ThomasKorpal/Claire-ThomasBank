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
#define BUFFSIZE 1024


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
  }else if(strcmp(buffer, "EXIT") && strcmp(buffer, "")){
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


int main() {
	int sock;
	char buffer[BUFFSIZE];
	struct sockaddr_in servaddr, cliaddr;
	int addrlen, bufferlen;
		

	init_bank();
	print_comptes();	

	// Creating socket file descriptor
	if ( (sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
		Die("Failed to create socket");
	}
		
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
		
	// Bind the socket with the server address
	if ( bind(sock, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
		Die("Failed to bind the server socket");
	}
		
	addrlen = sizeof(cliaddr); //len is value/result
	

	while(1){
		if((bufferlen = recvfrom(sock, (char *)buffer, BUFFSIZE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, (socklen_t *) &addrlen))<0){
			Die("Failed to receive additional bytes from client");
		}
		buffer[bufferlen] = '\0';
		printf("Client : %s\n", buffer);

		Recu_banque(buffer);
		bufferlen=strlen(buffer);

		if(sendto(sock, buffer, bufferlen, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, addrlen)!=bufferlen){
			Die("Failed to send bytes to client");
		}
		printf("Server envoi : %s\n", buffer);
	}
		
	return 0;
}
