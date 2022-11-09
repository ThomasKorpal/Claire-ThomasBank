#include "bank.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void init_bank()
{
    srand(time(NULL));
    
    for(int i = 0; i<MAX_CLIENT_NUMBER; i++)
    {
        int id_client=rand()%99999;
        clients[i]=add_client(id_client, password[i]);
        for(int i=0; i<MAX_ACCOUNT_NUMBER; i++){
            add_account(clients[i]);
        }
    }
}

Client* add_client(int id_client, char* password)
{
    Client* nouv_client = malloc(sizeof(Client));
    nouv_client->id_client = id_client;
    nouv_client->password = password;
    for(int i = 0; i < MAX_ACCOUNT_NUMBER; i++)
    {
        nouv_client->Comptes[i]=malloc(sizeof(Account));
    }
    nouv_client->index_compte=0;
    return nouv_client;
}

void add_account(Client* client)
{
    Account* nouv_compte = malloc(sizeof(Account));
    nouv_compte->solde = 0;
    nouv_compte->id_compte = nouv_id;
    nouv_id++;
    for(int i = 0; i < MAX_ACCOUNT_NUMBER; i++)
    {
        nouv_compte->archive[i]=malloc(sizeof(operation));
    }
    nouv_compte->index_archive=0;
    
    if(client->index_compte < MAX_ACCOUNT_NUMBER)
    {
        client->Comptes[client->index_compte]=nouv_compte;
        client->index_compte++;
    }
}

Client* find_client(int id_client)
{
    for(int i = 0; i < MAX_CLIENT_NUMBER; i++)
    {
        if((clients[i]).id_client == id_client)
        {
            return &clients[i];
        }
    }

    return NULL;
}

Account* find_account(Client* client, int id_compte)
{
    for(int i = 0; i < MAX_ACCOUNT_NUMBER; i++)
    {
        if((client->Comptes[i])->id_compte == id_compte)
        {
            return &(client->Comptes[i]);
        }
    }

    return NULL;
}

Client* identification(int id_client, char* password)
{
    Client* temp = find_client(id_client);
    char* verif;
    printf("Entrez votre mot de passe :\n");
    scanf("%s", &verif);
    while(strcmp(verif, temp->password) != 0)
    {
        printf("Mot de passe incorrect, veuillez re-taper votre mot de passe :\n");
        scanf("%s", &verif);
    }
    printf("Mot de passe correct");
    return temp;
}

void ecriture_archive(Account* compte, TypeOP type_ope, time_t date, int montant)
{
    operation* op = malloc(sizeof(operation));
    op->type=type_ope;
    op->montant=montant;
    op->date=date;

     if(compte->index_archive>=MAX_ARCHIVE_NUMBER)
    {
        for(int i=1; i<MAX_ARCHIVE_NUMBER; i++)
        {
            compte->archive[i-1] = compte->archive[i];
            compte->index_archive=MAX_ARCHIVE_NUMBER-1;
        }
    }
    
    if(compte->index_archive<MAX_ARCHIVE_NUMBER)
    {
        compte->archive[compte->index_archive]=op;
        compte->index_archive++; 
    }
}

void ajout(int id_client, int id_compte, char* password, int somme)
{
    Client* client_courrant = identification(id_client, password);
    if(client_courrant != NULL)
    {
        Account* compte_courrant = find_account(client_courrant, id_compte);
        compte_courrant->solde+=somme;
        time_t now;
        ecriture_archive(compte_courrant, AJOUT, time(&now), somme);
    }
    
}

void retrait(int id_client, int id_compte, char* password, int somme)
{
    Client* client_courrant = identification(id_client, password);
    if(client_courrant != NULL)
    {
        Account* compte_courrant = find_account(client_courrant, id_compte);
        compte_courrant->solde-=somme;
        time_t now;
        ecriture_archive(compte_courrant,RETRAIT, time(&now), somme);
    }
}

void solde(int id_client, int id_compte, char* password)
{
    Client* client_courrant = identification(id_client, password);
    if(client_courrant != NULL)
    {
        Account* compte_courrant = find_account(client_courrant, id_compte);
        printf("Votre solde est de: %d euros\n",compte_courrant->solde);
        time_t now;
        ecriture_archive(compte_courrant,SOLDE,time(&now),0);
    }
}

void operations(int id_client, int id_compte, char* password)
{
    Client* client_courrant = identification(id_client, password);
    if(client_courrant != NULL){
        Account* compte_courrant = find_account(client_courrant, id_compte);
        for(int i = 0; i < 10; i++)
        {
         printf("Date : %s\n Type : %s\n Montant : %d euros\n\n",(compte_courrant->archive[i])->date, to_string((compte_courrant->archive[i])->type), (compte_courrant->archive[i])->montant);
        }
        time_t now;
        ecriture_archive(compte_courrant, SOLDE, time(&now), 0);
    }
}

char* to_string(TypeOP op)
{
    char* res;
    switch(op)
    {
        case 0:
            res="AJOUT";
            break;
        case 1:
            res="RETRAIT";
            break;
        case 2:
            res="SOLDE";
            break;
        default:
            res="";
    }
    return res;
}
