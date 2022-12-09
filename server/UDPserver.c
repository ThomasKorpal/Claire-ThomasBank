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
      buffer[strlen(buffer)-1]=' ';

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


int main() {
	int sock;
	char buffer[BUFFSIZE];
	struct sockaddr_in servaddr, cliaddr;
	int addrlen, bufferlen;
		
	
  	//Creation des comptes bancaires des clients et affichage des comptes
	init_bank();
	print_comptes();	

  	//Creation du socket server
	if ( (sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
		Die("Failed to create socket");
	}

	//initialisation du sockaddr_in du server
	memset(&servaddr, 0, sizeof(servaddr));

	//Construction du sockaddr_in du client
	memset(&cliaddr, 0, sizeof(cliaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
		
	// Bind le socket server
	if ( bind(sock, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
		Die("Failed to bind the server socket");
	}
		
	addrlen = sizeof(cliaddr);

	//Le server tourne en boucle en atteandant une reception de requete
	while(1){
		//Reception de la requete du client
		if((bufferlen = recvfrom(sock, (char *)buffer, BUFFSIZE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, (socklen_t *) &addrlen))<0){
			Die("Failed to receive additional bytes from client");
		}
		buffer[bufferlen] = '\0';
		printf("Client : %s\n", buffer);

    	//Traitement de la requete du client
		Recu_banque(buffer);
		bufferlen=strlen(buffer);

    	//Envoie de reponse a la requet du client
		if(sendto(sock, buffer, bufferlen, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, addrlen)!=bufferlen){
			Die("Failed to send bytes to client");
		}
		printf("Server envoi : %s\n", buffer);
	}
	
	//Desallouer tous les comptes bancaires cree
	freeListClients();
	return 0;
}
