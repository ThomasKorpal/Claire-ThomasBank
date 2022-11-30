#include "bank.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

char* password_tab[MAX_CLIENT_NUMBER]={"John2222", "290Anna", "1001Dalmatiens", "CPCets", "JaiFaim"};
int nouv_id = 1;

void init_bank()
{
    srand(time(NULL));
    
    for(int i = 0; i<MAX_CLIENT_NUMBER; i++)
    {
        int id_client=rand()%99999;
        clients[i]=add_client(id_client, password_tab[i]);
        for(int j=0; j<MAX_ACCOUNT_NUMBER; j++){
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
    for(int i = 0; i < MAX_ARCHIVE_NUMBER; i++)
    {
        nouv_compte->archive[i]=malloc(sizeof(operation));
    }
    nouv_compte->index_archive=0;
    
    if(client->index_compte < MAX_ACCOUNT_NUMBER)
    {
        client->Comptes[client->index_compte]=nouv_compte;
    (client->index_compte)++;
    }
    else
    {
        printf("La limite de comptes a été atteinte!");
    }
}

Client* find_client(int id_client)
{
    for(int i = 0; i < MAX_CLIENT_NUMBER; i++)
    {
        if((clients[i])->id_client == id_client)
        {
            return clients[i];
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
            return (client->Comptes[i]);
        }
    }

    return NULL;
}

Client* identification(int id_client, char* password)
{
    Client* temp = find_client(id_client);
    int test = strcmp(password, temp->password);
    if(test != 0)
    {
        printf("Mot de passe incorrect, veuillez reessayer.\n");
        return NULL;
    }
    printf("Mot de passe correct, accès autorisé\n");
    return temp;
}

void ecriture_archive(Account* compte, TypeOP type_ope, time_t date, int montant)
{
    operation* op = malloc(sizeof(operation));
    op->type=type_ope;
    op->montant=montant;
    op->date=malloc(sizeof(100));
    op->date=ctime(&date);

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
        printf("La somme de %d€ a bien été ajoutée du compte n°%d appartenant au client n°%d\n",somme,id_compte,id_client);
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
        printf("La somme de %d€ a bien été retirée du compte n°%d appartenant au client n°%d\n",somme,id_compte,id_client);
    }
    
}

operation* solde(int id_client, int id_compte, char* password)
{
    Client* client_courrant = identification(id_client, password);
    if(client_courrant != NULL)
    {
        Account* compte_courrant = find_account(client_courrant, id_compte);
        printf("Votre solde est de: %lld euros\n",compte_courrant->solde);
        time_t now;
        ecriture_archive(compte_courrant,SOLDE,time(&now), compte_courrant->solde);

        return compte_courrant->archive[compte_courrant->index_archive];
    }
    printf("L'identification est incorrect\n");
    return 0;
    
}

operation** operations(int id_client, int id_compte, char* password)
{
    Client* client_courrant = identification(id_client, password);
    if(client_courrant != NULL){
        Account* compte_courrant = find_account(client_courrant, id_compte);
        printf("Visualisation des 10 dernières opérations :\n");
        for(int i = 0; i < 10; i++)
        {
            printf("Date : %s\nType : %s\nMontant : %d euros\n\n",(compte_courrant->archive[i])->date, to_string((compte_courrant->archive[i])->type), (compte_courrant->archive[i])->montant);
        }

        return compte_courrant->archive;
    }

    printf("L'identification est incorrect\n");
    return NULL;
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

void freeListClients()
{
    for(int i = 0; i<MAX_CLIENT_NUMBER; i++)
    {
        for(int j = 0; j<MAX_ACCOUNT_NUMBER; j++)
        {
            for(int k = 0; k<MAX_ARCHIVE_NUMBER; k++)
            {
                free(clients[i]->Comptes[j]->archive[k]);
            }
            free(clients[i]->Comptes[j]);
        }
        free(clients[i]);
    }
}
