#ifndef __BUFFERPRINTER_H__
#define __BUFFERPRINTER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void bufferPrinter(char* buffer)
{   
    char* delim=" ";
    int taille=strlen(buffer);
    char* typeRep=strtok(buffer, delim);
    if(taille>14){
        if(!strcmp(typeRep, "RES_SOLDE")){
            int montant=atoi(strtok(NULL, delim));
            char* date=strtok(NULL, "\n");
            printf("Votre solde au %s est de %d euros.\n", date, montant);
        }else if(!strcmp(typeRep, "RES_OPERATIONS")){
            char* typeOP;
            char* date;
            int montant;
            taille-=strlen(typeRep);

            while(taille>1){
                typeOP=strtok(NULL, delim);
                date=strtok(NULL, "\n");
                montant=atoi(strtok(NULL, delim));
                printf("%s : %d €\n",typeOP,montant);
                printf("Date : %s\n", date);
                taille=taille-strlen(typeOP)-strlen(date)-sizeof(montant)/sizeof(4)-5;
            }
        }
    }
}

#endif