/*
 * File:   main.c
 * Author: Renata Caroline Cunha
 *
 * Data: 10/10/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "OpenHash.h"
#include "HashExterno.h"


int main(int argc, char **argv)
{
    if(argc==4)
    {
        int tam,i,numColisoes;
        unsigned int *HEpesos, *OHpesos;
        double inicio, fim, tempo;
        OpenHash HashO;
        Externa HashE;
        srand(time(NULL));

        for(i=1;i<4;i++)
        {
            printf("||========= Hash Externa %d ========|| \n\n", i);

            inicio=(double)clock()/CLOCKS_PER_SEC;

            if(i==1)
            {
                HEpesos=HEcriaVetorPesos();
            }

            HashE=HElePalavrasChave(argv[1],HEpesos,i);
            HashE=HEleTexto(HashE,argv[2],HEpesos,i);
            HEsalvaHash(HashE,argv[3]);
            numColisoes=HEqtdColisoes(HashE);
            HEdestroiHash(HashE);

            fim=(double)clock()/CLOCKS_PER_SEC;
            tempo=fim-inicio;

            printf("\tNúmero de colisões: %d\n\n",numColisoes);
            printf("\tTempo Gasto: %f s\n\n",tempo);
            printf("||=================================|| \n\n");

            if(i==1)
            {
                free(HEpesos);
            }
        }

        for(i=1;i<4;i++)
        {
            printf("||========== Open Hash %d ==========|| \n\n", i);

            inicio=(double)clock()/CLOCKS_PER_SEC;

            tam=OHqtdPalavrasChave(argv[1]);

            if(i==1)
            {
                OHpesos=OHcriaVetorPesos(tam);
            }

            HashO=OHlePalavrasChave(argv[1],OHpesos,tam, i);
            HashO=OHleTexto(HashO,OHpesos,tam,argv[2],i);
            OHsalvaHash(HashO,argv[3],tam);
            numColisoes=OHqtdColisoes(HashO,tam);
            OHdestroiHash(HashO,tam);

            fim=(double)clock()/CLOCKS_PER_SEC;
            tempo=fim-inicio;

            printf("\tNúmero de colisões: %d\n\n",numColisoes);
            printf("\tTempo Gasto: %f s\n\n",tempo);
            printf("||=================================|| \n\n");

            if(i==1)
            {
                free(OHpesos);
            }
        }

        return 0;
    }
    else
    {
        printf("Parametros incorretos");

        return(-1);
    }

}
