#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bank.h"

void test()
{
    /*ajout(clients[0]->id_client,clients[0]->Comptes[2]->id_compte,clients[0]->password,2500);
    retrait(clients[0]->id_client,clients[0]->Comptes[2]->id_compte,clients[0]->password,500);
    solde(clients[0]->id_client,clients[0]->Comptes[2]->id_compte,clients[0]->password);
    operations(clients[0]->id_client,clients[0]->Comptes[2]->id_compte,clients[0]->password);

    //Tests de toutes les fonctions du projet*/

    //Test condition reelle

    int iden;
    int iden2;
    int act=0;
    char mdp[15];
    printf("!Bonjour, veuillez entrer votre identifiant :\n");
    scanf("%d",&iden);
    while(act != 5)
    {
        printf("Quelle action voulez vous effectuer ?\n- tapez 1 pour ajouter de l'argent à un de vos comptes\n- taper 2 pour faire un retrait\n- taper 3 pour consulter le solde d'un de vos comptes\n- taper 4 pour visualiser les 10 dernières opérations d'un de vos comptes\n- taper 5 pour fermer votre espace personnel\n");
        printf("Numéro d'action : ");
        scanf("%d",&act);  
        switch(act)
        {
            case 1:
                printf("Quel est le compte concerné ? :\n");
                scanf("%d",&iden2);
                printf("Entrez votre mot de passe :\n");
                scanf("%s",mdp);
                printf("Quelle est la somme a ajouter ?\n");
                scanf("%d",&act);
                ajout(iden,iden2,mdp,act);
                break;
            case 2:
                printf("Quel est le compte concerné ? :\n");
                scanf("%d",&iden2);
                printf("Entrez votre mot de passe :\n");
                scanf("%s",mdp);
                printf("Quelle est la somme a retirer ?\n");
                scanf("%d",&act);
                retrait(iden,iden2,mdp,act);
                break;
            case 3:
                printf("Quel est le compte concerné ? :\n");
                scanf("%d",&iden2);
                printf("Entrez votre mot de passe :\n");
                scanf("%s",mdp);
                solde(iden,iden2,mdp);
                break;
            case 4:
                printf("Quel est le compte concerné ? :\n");
                scanf("%d",&iden2);
                printf("Entrez votre mot de passe :\n");
                scanf("%s",mdp);
                operations(iden,iden2,mdp);
                break;
            case 5:
                break;
            default:
                break;
        }
    }
    printf("Merci d'avoir utilisé les services de Claire&Thomas Banque !\n");
}

int main()
{
    init_bank();
    print_comptes();
    test();
    print_comptes();
    freeListClients();
    return 0;
}