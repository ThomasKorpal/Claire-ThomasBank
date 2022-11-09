#ifndef __BANK_H__
#define __BANK_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARCHIVE_NUMBER  10
#define MAX_ACCOUNT_NUMBER 3
#define MAX_CLIENT_NUMBER 5


char* password[MAX_CLIENT_NUMBER]={"John2222", "290Anna", "1001Dalmatiens", "CPCets", "J'ai faim"};

int nouv_id = 1;

Client clients[MAX_ACCOUNT_NUMBER];
//int i = 0;

typedef enum TypeOP {AJOUT, RETRAIT,SOLDE} TypeOP;

typedef struct operation
{
    TypeOP type;
    int montant;
    time_t date;
}operation;

typedef struct Account
{
    int id_compte;
    long long int solde;
    operation* archive[MAX_ARCHIVE_NUMBER];
    int index_archive;
}Account;

typedef struct Client
{
    int id_client;
    char* password;
    Account* Comptes[MAX_ACCOUNT_NUMBER];
    int index_compte;
}Client;

void init_bank();
Client* add_client(int id_client, char* password);
void add_account(Client* client);
void ajout(int id_client, int id_compte, char* password, int somme);
void retrait(int id_client, int id_compte, char* password, int somme);
void solde(int id_client, int id_compte, char* password);
void operations(int id_client, int id_compte, char* password);
Client* find_client(int id_client);
void ecriture_archive(Account* compte, TypeOP operation, time_t date, int montant);
Account* find_account(Client* client, int id_compte);
char* to_string(TypeOP op);

#endif