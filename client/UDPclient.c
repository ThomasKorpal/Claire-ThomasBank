// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
	
#define PORT	 8080
#define BUFFSIZE 1024

void Die(char *mess) { perror(mess); exit(1); }


//Fonction permettant de traiter l'action du client
void choix(char* buffer, int action){
  switch(action){
    case 1:
      strcpy(buffer, "AJOUT");
      break;
    case 2:
      strcpy(buffer, "RETRAIT");  
      break;
    case 3:
      strcpy(buffer, "SOLDE");  
      break;
    case 4:
      strcpy(buffer, "OPERATIONS");
      break;
    case 5:
      strcpy(buffer, "EXIT");
      break;
  }
}

//Fonction permettant l'ecriture dans le buffer
void Ecriture_buffer(char* buffer, int action, char* id, char* compte, char* mdp){
  if(action!=5){                    //Si le client demande "EXIT" alors pas besoin d'ecrire les informations bancaires du client
    strcat(buffer, " <");
    strcat(buffer, id);
    strcat(buffer, " ");
    strcat(buffer, compte);
    strcat(buffer, " ");
    strcat(buffer, mdp);
    strcat(buffer, " ");

    if(action==1 || action==2){     //Seul les AJOUT et RETRAIT ont besoin de rensigner les sommes souhaitées
      char somme[5];
      printf("Quelle est la somme concernée ?\n");
      scanf("%s",somme); 
      strcat(buffer, somme);
    }
    strcat(buffer, ">");
  }
}

//Fonction permettant de traiter les requetes du client
void Envoi_banque(char* buffer){
  char id[15];
  char compte[15];
  char mdp[15];
  int act;


  //Demandant les informations necessaire afin de traiter la demande du client
  printf("Bonjour, veuillez entrer votre identifiant :\n");
  scanf("%s",id);
  printf("Quelle action voulez vous effectuer ?\n- tapez 1 pour ajouter de l'argent à un de vos comptes\n- taper 2 pour faire un retrait\n- taper 3 pour consulter le solde d'un de vos comptes\n- taper 4 pour visualiser les 10 dernières opérations d'un de vos comptes\n- taper 5 pour fermer votre espace personnel\n");
  printf("Numéro d'action : ");
  scanf("%d",&act);

  if(act!=5){           //Si le client demande de quitter alors pas besoin de connaitre les informations liée à son compte
    printf("Quel est le compte concerné ? :\n");
    scanf("%s",compte);
    printf("Entrez votre mot de passe :\n");
    scanf("%s",mdp);
  }

  choix(buffer, act);
  Ecriture_buffer(buffer, act, id, compte, mdp);
}



int main() {
	int sock;
	char buffer[BUFFSIZE];
	struct sockaddr_in	 servaddr, cliaddr;
	int bufferlen, addrlen;
	
	//Creation du socket client
	if ( (sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
		Die("Failed to create socket");
	}

	//Construction du sockaddr_in du server
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	//Construction du sockaddr_in du client
	memset(&cliaddr, 0, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(PORT+1);
	cliaddr.sin_addr.s_addr = INADDR_ANY;

	//Bind le socket client
  	if (bind(sock, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0) {
    	Die("Failed to bind the server socket");
  	}
	
	//Tourne en boucle tant que le client ne veut pas quitter
	while(strcmp(buffer, "EXIT")){

		//Traitement de la demande du client
		Envoi_banque(buffer);
		bufferlen=strlen(buffer);

		//Envoi du buffer(contemant la demande du client) au server
		if(sendto(sock, buffer, bufferlen, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr))!=bufferlen){
			Die("Mismatch in number of sent bytes");
		}
		printf("Client envoi : %s\n", buffer);
				
		//Reception du buffer envoyée par le server
		if((bufferlen = recvfrom(sock, (char *)buffer, BUFFSIZE, MSG_WAITALL, (struct sockaddr *) &servaddr, (socklen_t*) &addrlen))<1){
			Die("Failed to receive bytes from server");
		}
		buffer[bufferlen] = '\0';   //Ajout du fin de caractere
		printf("Recu de la part du server : %s\n", buffer);
	}

	printf("Fermeture du socket client\n");
	close(sock);
	return 0;
}
