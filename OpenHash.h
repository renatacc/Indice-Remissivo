/*
 * File:   OpenHash.h
 * Author: Renata Caroline Cunha
 *
 * Data: 10/10/2016
 */

#ifndef OpenHash_H
#define OpenHash_H

typedef struct open *OpenHash;

int OHqtdPalavrasChave(char EndArquivo[]); //Tamanho da Open Hash
OpenHash OHlePalavrasChave(char arquivo[], unsigned int *pesos,int tam, int tipoHash); //tipoHash=Tipo de hash que deseja utilizar
OpenHash OHleTexto(OpenHash Hash, unsigned int *pesos,int tam,char arquivo[], int tipoHash);
int OHsalvaHash(OpenHash Hash, char* arquivo, int tam);
void OHdestroiHash(OpenHash Hash, int tam);
int OHqtdColisoes(OpenHash Hash, int tam);
unsigned int * OHcriaVetorPesos(int tam);

#endif
