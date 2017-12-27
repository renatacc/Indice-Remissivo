/*
 * File:   HashExterno.c
 * Author: Renata Caroline Cunha
 *
 * Data: 10/10/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "HashExterno.h"

// #define TAMANHO 100

struct tabela
{
    struct externa *inicio;
    int numColisoes;
};

struct externa
{
    char palavra[33];
    struct linhas *indice; //Ponteiro para lista com indices das linhas em que elas aparecem
    struct externa *prox; //Para proxima caixinha com palavra chave
};

struct linhas
{
    int linha;
    struct linhas *prox;
};

typedef struct externa *structExterna;

//Cabeçalho das funções
Externa HEcriaHash(void); //Cria a Open Externa
int HEverificaTamPalavra(char *palavra);
int HEconsultaPalavraChave(Externa Hash, char *palavra, unsigned int *pesos,int tipoHash);
void  HEpegaPalavra(int inicio, int fim, char *frase, char *palavra);
void HEinserePalavraChave(Externa Hash,char *palavra,unsigned int *pesos,int tipoHash);
int HEconverteString(char *palavra);
int HEfuncaoHash1(unsigned int *pesos, char *palavra);
int HEfuncaoHash2(int numPalavra);
int HEfuncaoHash3(int numPalavra);
Externa HEinsereIndice(Externa Hash, char *palavra,unsigned int *pesos, int linha, int tipoHash);
int HEparticao(structExterna *Vet, int inicio, int fim);
void HEordenaVetor(structExterna *Vet, int inicio, int fim);
int HEqtdPalavrasChave(Externa Hash); //Quantidade de palavras chave na Hash
structExterna* HEcopiaVetor(Externa Hash, int tam);
void HEordenaVetor(structExterna *Vet, int inicio, int fim);
int HEparticao(structExterna *Vet, int inicio, int fim);
void HEapagaNodo(Externa Hash,int pos);
void HEapagaLinha(Externa Hash, int pos);

Externa HEcriaHash(void) //Cria a Open Externa
{
   Externa novo=(Externa)malloc(TAMANHO*(sizeof(struct tabela)));

   int i;
   for(i=0;i<TAMANHO;i++)
   {
       novo[i].inicio=NULL;
       novo[i].numColisoes=0;
   }

   return(novo);
}

Externa HElePalavrasChave(char arquivo[], unsigned int *pesos,int tipoHash) //tipoHash=Tipo de hash que deseja utilizar
{
    FILE *arq;
    arq=fopen(arquivo,"r");

    Externa Hash=HEcriaHash();

    char palavra[33], linha[33];
    int inicio,i;

    if(arq==NULL)
    {
        return(Hash);
    }

   while((fgets(linha,sizeof(linha),arq))!=NULL)
   {
       inicio=0;

       for(i=0;i<strlen(linha);i++) //Vai procurar até achar um caracter que não seja uma letra
       {
            if(!isalpha(linha[i])) //Se não é uma letra...
            {
                 HEpegaPalavra(inicio,i,linha,palavra); //Pega palavra na linha
                 if(HEverificaTamPalavra(palavra))
                 {
                     if(HEconsultaPalavraChave(Hash,palavra,pesos,tipoHash)==-1)
                     {
                         HEinserePalavraChave(Hash,palavra,pesos,tipoHash);
                     }
                 }

                 inicio=i+1;
            }
       }
   }

    return(Hash);
}

int HEverificaTamPalavra(char *palavra)  //Verifica se é uma palavra válida
{
    if(strlen(palavra)>3)
    {
       return(1);
    }

    return(0);
}

int HEconsultaPalavraChave(Externa Hash, char *palavra, unsigned int *pesos,int tipoHash) //Verifica se a palavra chave já está na Hash
{
   int numPalavra=HEconverteString(palavra);

   if(tipoHash==1)
   {
        int h=HEfuncaoHash1(pesos,palavra);

        if(Hash[h].inicio==NULL)
        {
            return(-1);
        }

        struct externa *aux=Hash[h].inicio;

        while(aux->prox!=NULL)
        {
            if((strcmp(aux->palavra, palavra))==0)
            {
                return(h);
            }
            aux=aux->prox;
        }

        if((strcmp(aux->palavra, palavra))==0)
        {
            return(h);
        }
   }

      if(tipoHash==2)
   {
        int h=HEfuncaoHash2(numPalavra);

         if(Hash[h].inicio==NULL)
        {
            return(-1);
        }

        struct externa *aux=Hash[h].inicio;

        while(aux->prox!=NULL)
        {
            if((strcmp(aux->palavra, palavra))==0)
            {
                return(h);
            }

            aux=aux->prox;
        }

        if((strcmp(aux->palavra, palavra))==0)
        {
            return(h);
        }
   }

      if(tipoHash==3)
   {
        int h=HEfuncaoHash3(numPalavra);

         if(Hash[h].inicio==NULL)
        {
            return(-1);
        }

        struct externa *aux=Hash[h].inicio;

        while(aux->prox!=NULL)
        {
            if((strcmp(aux->palavra, palavra))==0)
            {
                return(h);
            }

            aux=aux->prox;
        }

        if((strcmp(aux->palavra, palavra))==0)
        {
            return(h);
        }
   }
   return(-1);
}

void  HEpegaPalavra(int inicio, int fim, char *frase, char *palavra) //"Pega" palavra no texto
{
    int i,j=0;

    for(i=inicio;i<fim;i++)
    {
        palavra[j]=tolower(frase[i]);
        j++;
    }

    palavra[j]='\0';

    return;
}

void HEinserePalavraChave(Externa Hash,char *palavra,unsigned int *pesos,int tipoHash)
{
   int numPalavra=HEconverteString(palavra);
   int h;

   if(tipoHash==1)
   {
       h=HEfuncaoHash1(pesos,palavra);
   }

   if(tipoHash==2)
   {
       h=HEfuncaoHash2(numPalavra);
   }

   if(tipoHash==3)
   {
       h=HEfuncaoHash3(numPalavra);
   }

   struct externa *novo=(struct externa*)malloc(sizeof(struct externa));

   novo->indice=NULL;
   novo->prox=NULL;
   strcpy(novo->palavra,palavra);

   if(Hash[h].inicio==NULL) //Lista vazia
   {
       Hash[h].inicio=novo;
       return;
   }

   struct externa *aux=Hash[h].inicio;

   while(aux->prox!=NULL)
   {
       aux=aux->prox;
       Hash[h].numColisoes++;
   }

   aux->prox=novo;
   Hash[h].numColisoes++;

   return;
}

int HEconverteString(char *palavra)
{
    int i;
    unsigned valorRetorno=0;
    int tam=strlen(palavra);

    for(i=0;i<tam;i++)
    {
       valorRetorno=(1.7 * valorRetorno)+(int)palavra[i];
    }
    return(valorRetorno);
}

int HEfuncaoHash1(unsigned int *pesos, char *palavra)
{
    int i,valor=0;

    for(i=0; i<strlen(palavra);i++)
    {
        valor=valor+(pesos[i]*palavra[i]);
    }

    return(valor % TAMANHO);
}

int HEfuncaoHash2(int numPalavra)
{
    float X=0.6180339887;  //0 < X < 1
    float valor=numPalavra*X;
    valor=valor-(int)valor;

    return(int)(TAMANHO * valor);
}

int HEfuncaoHash3(int numPalavra)
{
   // int valorRetorno=numPalavra % TAMANHO;

    return(numPalavra % TAMANHO);
}


Externa HEleTexto(Externa Hash,char texto[], unsigned int *pesos,int tipoHash)
{
   FILE *arq;
   arq=fopen(texto,"r");

   char linha[100000];
   char palavra[33];
   int inicio,i;
   int contLinha=0;

   if(arq==NULL)
   {
      return(Hash);
   }

   while((fgets(linha,sizeof(linha),arq))!=NULL)
   {
       inicio=0;
       contLinha++;

       for(i=0;i<strlen(linha);i++) //Vai procurar até achar um caracter que não seja uma letra
       {
            if(!isalpha(linha[i])) //Se não é uma letra...
            {
                 HEpegaPalavra(inicio,i,linha,palavra);

                 if(HEverificaTamPalavra(palavra))
                 {
                    if(HEconsultaPalavraChave(Hash,palavra,pesos,tipoHash)>-1) //Se a palavra for uma palavra chave, então coloca sua linha
                    {
                        Hash=HEinsereIndice(Hash, palavra, pesos, contLinha, tipoHash);
                    }
                 }

                 inicio=i+1;
            }
       }
   }

   return(Hash);
}

Externa HEinsereIndice(Externa Hash, char *palavra,unsigned int *pesos, int linha, int tipoHash)
{
    int h,numPalavra=HEconverteString(palavra);

   if(tipoHash==1)
   {
       h=HEfuncaoHash1(pesos, palavra);
   }

   if(tipoHash==2)
   {
       h=HEfuncaoHash2(numPalavra);
   }

   if(tipoHash==3)
   {
       h=HEfuncaoHash3(numPalavra);
   }

    struct externa *auxExterna=Hash[h].inicio;
    struct linhas *novo=(struct linhas*)malloc(sizeof(struct linhas));

    novo->linha=linha;
    novo->prox=NULL;

    while(((strcmp(auxExterna->palavra,palavra))!=0) && (auxExterna->prox!=NULL))
    {
        auxExterna=auxExterna->prox;
    }

    if((strcmp(auxExterna->palavra, palavra))==0) //Encontrou a palavra
    {
        if(auxExterna->indice==NULL) //Se a lista de linhas estiver vazia...
        {
            auxExterna->indice=novo;
            return(Hash);
        }

        struct linhas *auxLinha=(struct linhas*)malloc(sizeof(struct linhas));

        auxLinha=auxExterna->indice;

        while(auxLinha->prox!=NULL) //Insere no final da lista
        {
            auxLinha=auxLinha->prox;
        }
        if(auxLinha->linha==linha)
        {
            return(Hash);
        }

        auxLinha->prox=novo;
        return(Hash);
    }

    return(Hash);
}

int HEsalvaHash(Externa Hash, char* arquivoSaida)
{
    FILE *arq;

    arq=fopen(arquivoSaida,"w");

    int i,tam;

    struct linhas *auxLinha;

    tam=HEqtdPalavrasChave(Hash);

    structExterna *Vet=HEcopiaVetor(Hash,tam);
    HEordenaVetor(Vet,0,tam-1);

    for(i=0; i<tam; i++)
    {
        if(HEverificaTamPalavra(Vet[i]->palavra))
        {
            fprintf(arq,"%s ",Vet[i]->palavra);
            auxLinha=Vet[i]->indice;

            if(auxLinha!=NULL)
            {
                while(auxLinha->prox!=NULL)
                {
                    fprintf(arq,"%d ",auxLinha->linha);
                    auxLinha=auxLinha->prox;
                }

                fprintf(arq,"%d\n",auxLinha->linha);
            }
        }
    }
    fclose(arq);

    return(1);
}

int HEparticao(structExterna *Vet, int inicio, int fim)
{
     structExterna aux1, aux2;
     int i, j;

     aux2 = Vet[fim]; //pivo
     j = inicio-1;

     for(i=inicio; i < fim; i++)
     {
	 if((strcmp(Vet[i]->palavra, aux2->palavra))<0)
         {
            j++;
            aux1 = Vet[j];
            Vet[j] = Vet[i];
            Vet[i] = aux1;
         }
     }

     aux1 = Vet[j+1];
     Vet[j+1] = aux2;
     Vet[fim] = aux1;

     return(j+1);
}

void HEordenaVetor(structExterna *Vet, int inicio, int fim)
{
    if(inicio < fim)
    {
      int q = HEparticao(Vet, inicio, fim);
      HEordenaVetor(Vet, inicio, q - 1);
      HEordenaVetor(Vet, q + 1, fim);
    }
}

structExterna* HEcopiaVetor(Externa Hash, int tam)
{
    int i,j=0;

    struct externa *NodoPalavra=NULL;
    struct externa **VetPalavras=(struct externa**)malloc(tam*sizeof(struct externa*));

    for(i=0; i<TAMANHO; i++)
    {
        NodoPalavra = Hash[i].inicio;

        while(NodoPalavra!=NULL)
        {
            VetPalavras[j] = NodoPalavra;
            j++;
            NodoPalavra = NodoPalavra->prox;
        }
    }

    return(VetPalavras);
}

int HEqtdPalavrasChave(Externa Hash) //Quantidade de palavra chave na Hash
{
    struct externa *auxExterna;
    int i, cont=0;


    for(i=0; i<TAMANHO; i++)
    {
        auxExterna=Hash[i].inicio;

        while(auxExterna!=NULL)
        {
            cont++;
            auxExterna=auxExterna->prox;
        }
    }

    return(cont);
}

void HEdestroiHash(Externa Hash)
{
    int i;

    for(i=0; i<TAMANHO; i++)
    {
        if(Hash[i].inicio!=NULL)
        {
            while((Hash[i].inicio)->prox!=NULL)
            {
                HEapagaNodo(Hash, i);
            }

            HEapagaNodo(Hash,i);
        }
    }

    free(Hash);
    return;
}

void HEapagaNodo(Externa Hash, int pos) //Apaga nodo palavra
{
    struct externa *auxExterna;

    if((Hash[pos].inicio)->indice!=NULL)
    {
        while(((Hash[pos].inicio)->indice)->prox!=NULL)
        {
            HEapagaLinha(Hash,pos);
        }

        HEapagaLinha(Hash,pos);
    }

    auxExterna=Hash[pos].inicio;
    Hash[pos].inicio=auxExterna->prox;

    free(auxExterna);
    auxExterna=NULL;

    return;
}

void HEapagaLinha(Externa Hash, int pos)
{
    struct linhas *auxLinha;

    auxLinha=(Hash[pos].inicio)->indice;
    (Hash[pos].inicio)->indice=auxLinha->prox;

    free(auxLinha);
    auxLinha=NULL;

    return;
}

int HEqtdColisoes(Externa Hash)
{
    int i, numColisoes=0;

    for(i=0;i<TAMANHO;i++)
    {
        numColisoes=numColisoes+Hash[i].numColisoes;
    }

    return(numColisoes);
}

unsigned int * HEcriaVetorPesos(void)
{
    int i;
   // srand(time(NULL));

    unsigned int *pesos=(unsigned int*)malloc(TAMANHO*sizeof(unsigned int));

    for(i=0;i<TAMANHO; i++)
    {
            pesos[i]=1000*(rand()/(double)RAND_MAX);
    }

    return(pesos);
}
