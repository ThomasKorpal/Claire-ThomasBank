#ifndef __BUFFERPRINTER_H__
#define __BUFFERPRINTER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bufferPrinter(char* buffer)
{
    char* typeRep;
    char* reste;
    int montant;
    char caracCourant = buffer[0];
    sscanf(buffer,"%s %s",typeRep,reste);
    caracCourant=buffer[strlen(typeRep)+1];
    switch(strcmp(typeRep,"RES_SOLDE"))
    {
        case 1:
            sscanf(buffer,"%s %d %s",typeRep,&montant,reste);
            printf("Votre solde au %s est de %d euros.\n",reste,montant);
            break;
        default:
            break;
    }
    switch(strcmp(typeRep,"RES_OPERATIONS"))
    {
        case 1:
            printf("");
            char* typeOP;
            char* jour;
            char* mois;
            int journb;
            char* heure;
            int annee; 
            int montant;
            for(int i = 0; i < 10; i++)
            {
                sscanf(reste,"%s %s %s  %d %s %d %d %s",typeOP,jour,mois,&journb,heure,&annee,&montant,reste);
                printf("%s : %d €\n",typeOP,montant);
                printf("Date : %d %s %d : %s\n",journb,mois,annee,heure);
            }
            break;
        default:
            break;
    }
}

#endif