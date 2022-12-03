#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFFSIZE 450
void Die(char *mess) { perror(mess); exit(1); }


void Envoi_banque(char* buffer){
  char id[15];
  char compte[15];
  char mdp[15];
  int act;
  char somme[5];

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

  switch(act)
  {
    case 1:
      strcpy(buffer, "AJOUT");
      printf("Quelle est la somme a ajouter ?\n");
      scanf("%s",somme); 
      break;
    case 2:
      strcpy(buffer, "RETRAIT");
      printf("Quelle est la somme a retirer ?\n");
      scanf("%s",somme);  
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
    default:
        break;
  }

  if(act!=5){
    strcat(buffer, " <");
    strcat(buffer, id);
    strcat(buffer, " ");
    strcat(buffer, compte);
    strcat(buffer, " ");
    strcat(buffer, mdp);
    strcat(buffer, " ");

    if(act!=4){
      strcat(buffer, somme);
    }
    strcat(buffer, ">");
  }
}


int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in echoserver;
  char buffer[BUFFSIZE];
  unsigned int echolen;
  //int received = 0;

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

/*
  // Send the word to the server
  echolen = strlen(argv[2]);
  if (send(sock, argv[2], echolen, 0) != echolen) {
    Die("Mismatch in number of sent bytes");
  }
  printf("Send : %s\n", argv[2]);
  
  // Receive the word back from the server
  fprintf(stdout, "Received: ");
  int bytes = 0;
  if ((bytes = recv(sock, buffer, BUFFSIZE-1, 0)) < 1) {
    Die("Failed to receive bytes from server");
  }
  buffer[bytes] = '\0';        // Assure null terminated string
  fprintf(stdout,"%s\n",buffer);
*/

  while (strcmp(buffer, "EXIT")){
    Envoi_banque(buffer);
    echolen=strlen(buffer);
    if (send(sock, buffer, echolen, 0) != echolen) {
      Die("Mismatch in number of sent bytes");
    }
    printf("Envoi : %s\n", buffer);
    int bytes = 0;
    if ((bytes = recv(sock, buffer, BUFFSIZE-1, 0)) < 1) {
      Die("Failed to receive bytes from server");
    }
    buffer[bytes] = '\0';        // Assure null terminated string
    printf("Recu : %s\n", buffer);
  }

  /*
  while (received < echolen) {
    int bytes = 0;
    if ((bytes = recv(sock, buffer, BUFFSIZE-1, 0)) < 1) {
      Die("Failed to receive bytes from server");
    }
    received += bytes;
    buffer[bytes] = '\0';        // Assure null terminated string
    fprintf(stdout,"%s",buffer);
  }*/

  fprintf(stdout, "\n");
  printf("Fermeture cl\n");
  close(sock);
  exit(0);
}

