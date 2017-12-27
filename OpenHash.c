/*
 * File:   OpenHash.c
 * Author: Renata Caroline Cunha
 *
 * Data: 10/10/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "OpenHash.h"

//Estruturas
struct open
{
    char palavra[33];
    struct linhas *indice; //Ponteiro para lista com indices das linhas em que elas aparecem
    int vazio; //Para saber se a posição está vazia
    int numColisoes;
};

struct linhas
{
    int linha;
    struct linhas *prox;
};

//Cabeçalho das funções
OpenHash OHcriaHash(int tam); //Cria a Open Hash
int OHverificaTamPalavra(char *palavra);  //Verifica se é uma palavra válida
void OHinserePalavraChave(OpenHash Hash,char *palavra, unsigned int *pesos, int tam,int tipoHash); //tipoHash=Tipo de hash que deseja utilizar
unsigned int OHfuncaoHash1(unsigned int *pesos, char *palavra, int tam, int peso);
int OHfuncaoHash2(int tam, int numPalavra);
int OHfuncaoHash3(int tam, int numPalavra);
int OHconverteString(char *palavra);
void  OHpegaPalavra(int inicio, int fim, char *frase, char *palavra);
OpenHash OHinsereIndice(OpenHash Hash, int posHash, int linha);
int OHconsultaPalavraChave(OpenHash Hash, char *palavra,  unsigned int *pesos, int tam, int tipoHash);
int OHparticao(OpenHash *Vet, int inicio, int fim);
void OHordenaVetor(OpenHash *Vetor, int inicio, int fim);
OpenHash* OHcopiaVetor(OpenHash Hash, int tam);
void OHapagaLinha(OpenHash Hash, int pos);


int OHconsultaPalavraChave(OpenHash Hash, char *palavra, unsigned int *pesos, int tam, int tipoHash) //Verifica se a palavra chave já está na Hash
{
   int h,i=0;
   int numPalavra=OHconverteString(palavra);

   if(tipoHash==1)
   {
        h=OHfuncaoHash1(pesos, palavra,tam,i);


        while(((strcmp(Hash[h].palavra, palavra))!=0) && (i<tam)) //Enquanto as palavras não forem iguais e enquanto não tiver percorrido a hash inteira
        {
            i++;
            h=OHfuncaoHash1(pesos, palavra,tam,i);
        }

        if(i==tam)//Se tiver percorrido toda a hash e não encontrar a palavra retorna 0
        {
          return (-1);
        }
   }

      if(tipoHash==2)
   {
        h=OHfuncaoHash2(tam, numPalavra);


        while(((strcmp(Hash[h].palavra, palavra))!=0) && (i<tam)) //Enquanto as palavras não forem iguais e enquanto não tiver percorrido a hash inteira
        {
            i++;
            h=OHfuncaoHash2(tam, numPalavra+i);
        }

        if(i==tam)//Se tiver percorrido toda a hash e não encontrar a palavra retorna 0
        {
          return (-1);
        }
   }

      if(tipoHash==3)
   {
        h=OHfuncaoHash3(tam, numPalavra);


        while(((strcmp(Hash[h].palavra, palavra))!=0) && (i<tam)) //Enquanto as palavras não forem iguais e enquanto não tiver percorrido a hash inteira
        {
            i++;
            h=OHfuncaoHash3(tam, numPalavra+i);
        }

        if(i==tam)//Se tiver percorrido toda a hash e não encontrar a palavra retorna 0
        {
          return (-1);
        }
   }

   return(h); //Encontrou a palavra - Retorna a posição na Hash que a palavra está
}

int OHqtdPalavrasChave(char EndArquivo[]) //Tamanho da Open Hash
{
   FILE *arq;
   char frase[33];
   int qtdPalavras=0;
   arq=fopen(EndArquivo,"r");

   if(arq)
   {
        while((fgets(frase,sizeof(frase),arq))!=NULL)
        {
            if(OHverificaTamPalavra(frase))
            {
                qtdPalavras++;
            }
        }
   }

   fclose(arq);
   return(qtdPalavras);
}

OpenHash OHcriaHash(int tam) //Cria a Open Hash
{
   struct open *novo=(struct open*)malloc(tam*sizeof(struct open));
   int i;

   for(i=0; i<tam;i++)
   {
       novo[i].vazio=0;
       strcpy(novo[i].palavra,"---"); //Considerar que "---" está vazio
       novo[i].indice=NULL;
       novo[i].numColisoes=0;
   }

   return(novo);
}

int OHverificaTamPalavra(char *palavra)  //Verifica se é uma palavra válida
{
    if(strlen(palavra)>3)
    {
       return(1);
    }

    return(0);
}

void OHinserePalavraChave(OpenHash Hash,char *palavra,unsigned int *pesos,int tam,int tipoHash)
{
   int h,i,numPalavra;

   numPalavra=OHconverteString(palavra);

   if(tipoHash==1)
   {
       i=0;
       h=OHfuncaoHash1(pesos, palavra,tam,i);

       while(Hash[h].vazio==1)
       {
           Hash[h].numColisoes++;
           h=OHfuncaoHash1(pesos, palavra,tam,i);
           i++;
       }
   }

   if(tipoHash==2)
   {
       h=OHfuncaoHash2(tam, numPalavra);
       i=0;

       while(Hash[h].vazio==1)
       {
           Hash[h].numColisoes++;
           h=OHfuncaoHash2(tam, numPalavra+i);
           i++;
       }
   }

   if(tipoHash==3)
   {

       h=OHfuncaoHash3(tam, numPalavra);
       i=0;

       while(Hash[h].vazio==1)
       {
           Hash[h].numColisoes++;
           h=OHfuncaoHash3(tam, numPalavra+i);
           i++;
       }
   }
   strcpy(Hash[h].palavra,palavra);
   Hash[h].vazio=1;
   Hash[h].indice=NULL;

   return;
//Chama a Open, olha se ta vazio. Se tiver vazio insere,
//se não tiver vazio, chama a open de novo com outro valor.
}

OpenHash OHlePalavrasChave(char arquivo[], unsigned int *pesos,int tam, int tipoHash) //tipoHash=Tipo de hash que deseja utilizar
{
    FILE *arq;
    arq=fopen(arquivo,"r");

    OpenHash Hash=OHcriaHash(tam);

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
                 OHpegaPalavra(inicio,i,linha,palavra); //Pega palavra na linha
                 if(OHverificaTamPalavra(palavra))
                 {
                     if(OHconsultaPalavraChave(Hash,palavra,pesos,tam,tipoHash)==-1)
                     {
                         OHinserePalavraChave(Hash,palavra,pesos,tam,tipoHash);
                     }
                 }

                 inicio=i+1;
            }
       }
   }

    return(Hash);
}

unsigned int OHfuncaoHash1(unsigned int *pesos, char *palavra, int tam, int peso)
{
    int i,valor=0;

    for(i=0; i<strlen(palavra);i++)
    {
        valor=valor+pesos[i]*palavra[i];
    }
    valor = valor +peso;
    return(valor % tam);
}

int OHfuncaoHash2(int tam, int numPalavra)
{
    float X=0.6180339887;  //0 < X < 1
    float valor=numPalavra*X;
    valor=valor-(int)valor;

    return(int)(tam * valor);
}

int OHfuncaoHash3(int tam, int numPalavra)
{
    int valorRetorno=numPalavra % tam;

    return(valorRetorno);
}

int OHconverteString(char *palavra)
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

void  OHpegaPalavra(int inicio, int fim, char *frase, char *palavra) //"Pega" palavra no texto
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

OpenHash OHinsereIndice(OpenHash Hash, int posHash, int linha) //Insere o indice da linha em que aparece as palavras chave no texto
{
    struct linhas *novo=(struct linhas*)malloc(sizeof(struct linhas));

    novo->linha=linha;
    novo->prox=NULL;

    //Se a lista estiver vazia..
    if(Hash[posHash].indice==NULL)
    {
        Hash[posHash].indice=novo;
        return(Hash);
    }

    //Outros casos
    struct linhas *aux=Hash[posHash].indice;
    while(aux->prox!=NULL)
    {
        aux=aux->prox;
    }

    if(aux->linha==linha)
    {
        return(Hash);
    }

    aux->prox=novo;
    return(Hash);
}

OpenHash OHleTexto(OpenHash Hash, unsigned int *pesos,int tam,char arquivo[], int tipoHash)
{
   FILE *arq;
   arq=fopen(arquivo,"r");

   char linha[100000];
   char palavra[33];
   int inicio,i;
   int contLinha=0;
   int posHash;

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
                 OHpegaPalavra(inicio,i,linha,palavra);
                 if(OHverificaTamPalavra(palavra))
                 {
                    posHash=OHconsultaPalavraChave(Hash,palavra,pesos,tam, tipoHash);

                    if(posHash>-1) //Se a palavra for uma palavra chave, então coloca sua linha
                    {
                        Hash=OHinsereIndice(Hash,posHash,contLinha);
                    }
                 }

                 inicio=i+1;
            }
       }
   }

   return(Hash);
}

int OHsalvaHash(OpenHash Hash, char* arquivo, int tam)
{
    FILE *arq;

    arq=fopen(arquivo,"w");

    struct linhas *aux;
    int i;

    OpenHash *Vetor=OHcopiaVetor(Hash,tam);
    OHordenaVetor(Vetor,0,tam-1);

    for(i=0; i<tam; i++)
    {
        if(OHverificaTamPalavra(Vetor[i]->palavra))
        {
            fprintf(arq,"%s ",Vetor[i]->palavra);
            aux=Vetor[i]->indice;

            if(aux!=NULL)
            {
                while(aux->prox!=NULL)
                {
                    fprintf(arq,"%d ",aux->linha);
                    aux=aux->prox;
                }

                fprintf(arq,"%d\n",aux->linha);
            }
        }
    }
    fclose(arq);

    return(1);
}

void OHordenaVetor(OpenHash *Vetor, int inicio, int fim)
{
  if(inicio < fim)
  {
    int q = OHparticao(Vetor, inicio, fim);
    OHordenaVetor(Vetor, inicio, q - 1);
    OHordenaVetor(Vetor, q + 1, fim);
  }
}

int OHparticao(OpenHash *Vet, int inicio, int fim)
{
     OpenHash aux1, aux2;
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

OpenHash* OHcopiaVetor(OpenHash Hash, int tam)
{
    OpenHash *vetor=(struct open**)malloc(tam*sizeof(struct open*));
    int i;

    for(i=0; i<tam; i++)
    {
        vetor[i] = &Hash[i];
    }

    return(vetor);
}

void OHdestroiHash(OpenHash Hash, int tam)
{
    int i;

    for(i=0; i<tam; i++)
    {
        if(Hash[i].indice!=NULL)
        {
            while((Hash[i].indice)->prox!=NULL)
            {
                OHapagaLinha(Hash,i);
            }
            OHapagaLinha(Hash,i);
        }
    }
    free(Hash);

    return;
}

void OHapagaLinha(OpenHash Hash, int pos)
{
    struct linhas *auxLinha;

    auxLinha=Hash[pos].indice;
    Hash[pos].indice=auxLinha->prox;

    free(auxLinha);
    auxLinha=NULL;

    return;
}

int OHqtdColisoes(OpenHash Hash, int tam)
{
    int i, numColisoes=0;

    for(i=0;i<tam;i++)
    {
        numColisoes=numColisoes+Hash[i].numColisoes;
    }

    return(numColisoes);
}

unsigned int * OHcriaVetorPesos(int tam)
{
    int i;
   // srand(time(NULL));

   unsigned int *pesos=(unsigned int*)malloc(tam*sizeof(unsigned int));

    for(i=0;i<tam; i++)
    {
            pesos[i]=1000*(rand()/(double)RAND_MAX);
    }

    return(pesos);
}
