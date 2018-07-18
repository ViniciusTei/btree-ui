#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define M 8
#define MM  (M * 2)
#define FALSE 0
#define TRUE  1

typedef struct{
	char nome[13];
	long int cpf;
}TipoChave;

typedef struct TipoRegistro {
  TipoChave Chave;
} TipoRegistro;

typedef struct TipoPagina* TipoApontador;
typedef struct TipoPagina {
  short n;
  TipoRegistro r[MM];
  TipoApontador p[MM + 1];
} TipoPagina;

void Inicializa(TipoApontador *Dicionario);
int Altura(int numReg);
int Pesquisa(TipoRegistro *x, TipoApontador Ap, int *compPes);
void InsereNaPagina(TipoApontador Ap, 
                    TipoRegistro Reg, TipoApontador ApDir, int *compIns);
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, 
         TipoRegistro *RegRetorno,  TipoApontador *ApRetorno, int *compIns);
void Insere(TipoRegistro Reg, TipoApontador *Ap, int *compIns);
void Reconstitui(TipoApontador ApPag, TipoApontador ApPai, 
                 int PosPai, short *Diminuiu, int *compRec);
void Antecessor(TipoApontador Ap, int Ind, 
                TipoApontador ApPai, short *Diminuiu, int *compAnt);
void Ret(TipoChave Ch, TipoApontador *Ap, short *Diminuiu, int *compRet);
void Retira(TipoChave Ch, TipoApontador *Ap, int *compRet);
void ImprimeI(TipoApontador p, int nivel);
void Imprime(TipoApontador p);
int StringToInt(char *num);