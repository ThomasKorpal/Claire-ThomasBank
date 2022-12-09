# Claire-ThomasBank

------------------------------------------------------------------------ (ENG) -----------------------------------------------

This project is a client/server banking application working both on the TCP protocol and on the UDP protocol. It creates 5 clients with 3 accounts each and let you control them. You can add money to an account, take money back, watch the amount that you have on one account and watch the previous 10 operations on each account. 

In order to execute the program, you need to :

  - For the TCP application : 
    - Open a terminal and go to where you stored the files
    - Enter command "make all"
    - Enter command "./server_TCP"
    - Open another instance of your terminal and go again where you stored the files
    - Enter command "./client_TCP"
    
  - For the UDP application
    - Open a terminal and go to where you stored the files
    - Enter command "make all"
    - Enter command "./server_UDP"
    - Open another instance of your terminal and go again where you stored the files
    - Enter command "./client_UDP"

For each, you can view all the details about all the clients, their id, their password and their accounts on the "server" terminal.

------------------------------------------------------------------------ (FR) --------------------------------------------------------------------------------------

Ce projet est une application bancaire de type client/serveur fonctionnant sur le protocole TCP ainsi que sur le protocole UDP. Elle crée 5 clients possédant 3 comptes chacun et vous en laisse le controle. Vous pouvez ajouter de l'argent à un compte, faire un retrait, voir le solde d'un compte et avoir accès aux 10 dernières opérations de chaque compte.

Afin d'executer l'application, vous devez :

  - Pour l'application fonctionnant avec TCP : 
    - Ouvrez un terminal et naviguez dans l'arboresence vers là ou sont stockés les fichiers du projet
    - Entrez la commande "make all"
    - Entrez la commande "./server_TCP"
    - Ouvrez une nouvelle instance du terminal et naviguez dans l'arboresence vers là ou sont stockés les fichiers du projet à nouveau
    - Entrez la commande "./client_TCP"
    
  - Pour l'application fonctionnant avec UDP : 
    - Ouvrez un terminal et naviguez dans l'arboresence vers là ou sont stockés les fichiers du projet
    - Entrez la commande "make all"
    - Entrez la commande "./server_UDP"
    - Ouvrez une nouvelle instance du terminal et naviguez dans l'arboresence vers là ou sont stockés les fichiers du projet à nouveau
    - Entrez la commande "./client_UDP"

Pour chaque application, vous avez accès a toutes les informations concernant les clients, leurs identifiants, leurs mots de passe et leurs comptes sur le terminal "serveur"
