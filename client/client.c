#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFFSIZE 450


void Die(char *mess) { perror(mess); exit(1); }


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

void Ecriture_buffer(char* buffer, int action, char* id, char* compte, char* mdp){
  if(action!=5){
    strcat(buffer, " <");
    strcat(buffer, id);
    strcat(buffer, " ");
    strcat(buffer, compte);
    strcat(buffer, " ");
    strcat(buffer, mdp);
    strcat(buffer, " ");

    if(action==1 || action==2){
      char somme[5];
      printf("Quelle est la somme concernée ?\n");
      scanf("%s",somme); 
      strcat(buffer, somme);
    }
    strcat(buffer, ">");
  }
}

void Envoi_banque(char* buffer){
  char id[15];
  char compte[15];
  char mdp[15];
  int act;

  printf("Bonjour, veuillez entrer votre identifiant :\n");
  scanf("%s",id);
  printf("Quelle action voulez vous effectuer ?\n- tapez 1 pour ajouter de l'argent à un de vos comptes\n- taper 2 pour faire un retrait\n- taper 3 pour consulter le solde d'un de vos comptes\n- taper 4 pour visualiser les 10 dernières opérations d'un de vos comptes\n- taper 5 pour fermer votre espace personnel\n");
  printf("Numéro d'action : ");
  scanf("%d",&act);

  if(act!=5){
    printf("Quel est le compte concerné ? :\n");
    scanf("%s",compte);
    printf("Entrez votre mot de passe :\n");
    scanf("%s",mdp);
  }

  choix(buffer, act);
  Ecriture_buffer(buffer, act, id, compte, mdp);
}



int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in echoserver;
  char buffer[BUFFSIZE];
  unsigned int echolen;

  if (argc != 4) {
    fprintf(stderr, "USAGE: TCPecho <server_ip> <word> <port>\n");
    exit(1);
  }

  /* Create the TCP socket */
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    Die("Failed to create socket");
  }

  /* Construct the server sockaddr_in structure */
  memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
  echoserver.sin_family = AF_INET;                  /* Internet/IP */
  echoserver.sin_addr.s_addr = inet_addr(argv[1]);  /* IP address */
  echoserver.sin_port = htons(atoi(argv[3]));       /* server port */
  
  /* Establish connection */
  if (connect(sock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {
    Die("Failed to connect with server");
  }

  while (strcmp(buffer, "EXIT")){
    Envoi_banque(buffer);
    echolen=strlen(buffer);

    if (send(sock, buffer, echolen, 0) != echolen) {
      Die("Mismatch in number of sent bytes");
    }
    printf("Envoi : %s\n", buffer);
    
    if ((echolen = recv(sock, buffer, BUFFSIZE-1, 0)) < 1) {
      Die("Failed to receive bytes from server");
    }
    buffer[echolen] = '\0';        // Assure null terminated string
    printf("Recu : %s\n", buffer);
  }

  printf("Fermeture cl\n");
  close(sock);
  exit(0);
}

