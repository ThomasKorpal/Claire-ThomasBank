#include "bank.h"
#include <stdlib.h>
#include <stdio.h>

void test()
{
    ajout(clients[0]->id_client,clients[0]->Comptes[2]->id_compte,clients[0]->password,2500);
    retrait(clients[0]->id_client,clients[0]->Comptes[2]->id_compte,clients[0]->password,500);
    solde(clients[0]->id_client,clients[0]->Comptes[2]->id_compte,clients[0]->password);
    operations(clients[0]->id_client,clients[0]->Comptes[2]->id_compte,clients[0]->password);
}

int main()
{
    init_bank();
    for(int i=0; i <MAX_CLIENT_NUMBER; i++)
    {
        printf("%d : %s (%d)\n",clients[i]->id_client,clients[i]->password,clients[i]->index_compte);
        for(int j=0; j<MAX_ACCOUNT_NUMBER; j++)
        {
            printf("    Compte n°%d : %lld€ (%d)\n",clients[i]->Comptes[j]->id_compte,clients[i]->Comptes[j]->solde,clients[i]->Comptes[j]->index_archive);
        }
    }
    test();
    freeListClients();
    return 0;
}