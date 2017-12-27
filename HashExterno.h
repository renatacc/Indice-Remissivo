/* 
 * File:   HashExterno.h
 * Author: Renata Caroline Cunha
 *
 * Data: 10/10/2016
 */

#ifndef HASHEXTERNO_H
#define	HASHEXTERNO_H

typedef struct tabela *Externa;

Externa HElePalavrasChave(char arquivo[], unsigned int *pesos,int tipoHash);
Externa HEleTexto(Externa Hash,char texto[], unsigned int *pesos,int tipoHash);
int HEsalvaHash(Externa Hash, char* arquivoSaida);
void HEdestroiHash(Externa Hash);
int HEqtdColisoes(Externa Hash);
unsigned int * HEcriaVetorPesos(void);

#endif	/* HASHEXTERNO_H */

