#include "arvore.h"

void Inicializa(TipoApontador *Dicionario)
{  
  *Dicionario = NULL; 
}

int Altura(int numReg)
{
  int aux, log;

  for (log = 0, aux = 0; aux <= M; ++log, ++aux)
  {
    aux *= numReg;
  }

  return 1 + log;
}

int Pesquisa(TipoRegistro *x, TipoApontador Ap, int *compPes)
{ 
  long i = 1;
  
  if (Ap == NULL) 
  { 
    (*compPes)++;
    //printf("Registro nao esta presente na arvore!\n");
    return 0;
  }
  
  while (i < Ap->n && x->Chave.cpf > Ap->r[i-1].Chave.cpf) {
    (*compPes)++;
    i++;
  }
  
  if (x->Chave.cpf == Ap->r[i-1].Chave.cpf) 
  { 
    (*compPes)++;
    *x = Ap->r[i-1];
    //printf("Registro esta presente!\n");
    return 1;
  }
  
  if (x->Chave.cpf < Ap->r[i-1].Chave.cpf) {
    (*compPes)++;
    return Pesquisa(x, Ap->p[i-1], compPes);
  }
  else 
    return Pesquisa(x, Ap->p[i], compPes);
} 

void InsereNaPagina(TipoApontador Ap, 
                    TipoRegistro Reg, TipoApontador ApDir, int *compIns)
{ 
  short NaoAchouPosicao;
  int k;
  
  k = Ap->n;  
  NaoAchouPosicao = (k > 0);
  
  while (NaoAchouPosicao) 
    { 
      (*compIns)++;
      if (Reg.Chave.cpf >= Ap->r[k-1].Chave.cpf) 
      { 
        (*compIns)++;
        NaoAchouPosicao = FALSE;
        break;
      }
      Ap->r[k] = Ap->r[k-1];
      Ap->p[k+1] = Ap->p[k];
      k--;
      
      if (k < 1){
        (*compIns)++; 
        NaoAchouPosicao = FALSE;
      }
    }
  Ap->r[k] = Reg; 
  Ap->p[k+1] = ApDir;
  Ap->n++;
} 

void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, 
         TipoRegistro *RegRetorno,  TipoApontador *ApRetorno, int *compIns)
{ 
  long i = 1; 
  long j;
  TipoApontador ApTemp;
  
  if (Ap == NULL) 
  { 
    (*compIns)++;
    *Cresceu = TRUE; 
    (*RegRetorno) = Reg; 
    (*ApRetorno) = NULL;
    return;
  }
  
  while (i < Ap->n && Reg.Chave.cpf > Ap->r[i-1].Chave.cpf){
    (*compIns)++; 
    i++;
  }
  
  if (Reg.Chave.cpf == Ap->r[i-1].Chave.cpf)
  { 
    (*compIns)++;
    //printf(" Erro: Registro ja esta presente\n"); 
    *Cresceu = FALSE;
    return;
  }
  
  if (Reg.Chave.cpf < Ap->r[i-1].Chave.cpf){
    (*compIns)++;
    i--;
  }
  
  Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno, compIns);
  
  if (!*Cresceu){
    (*compIns)++;
    return;
  }
  
  if (Ap->n < MM)   /* Pagina tem espaco */
    { 
      (*compIns)++;
      InsereNaPagina(Ap, *RegRetorno, *ApRetorno, compIns);
      *Cresceu = FALSE;
      return;
    }
  
  /* Overflow: Pagina tem que ser dividida */
  ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
  ApTemp->n = 0;  
  ApTemp->p[0] = NULL;
  
  if (i < M + 1) 
  { 
    (*compIns)++;
    InsereNaPagina(ApTemp, Ap->r[MM-1], Ap->p[MM], compIns);
    Ap->n--;
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno, compIns);
  } 
  else 
    InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno, compIns);
  
  for (j = M + 2; j <= MM; j++) 
    InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j], compIns);
  
  Ap->n = M;  
  ApTemp->p[0] = Ap->p[M+1];
  *RegRetorno = Ap->r[M];  
  *ApRetorno = ApTemp;
}

void Insere(TipoRegistro Reg, TipoApontador *Ap, int *compIns)
{ 
  short Cresceu;
  TipoRegistro RegRetorno;
  TipoPagina *ApRetorno, *ApTemp;
  
  Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno, compIns);
  
  if (Cresceu)  /* Arvore cresce na altura pela raiz */
  { 
    (*compIns)++;
    ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
    ApTemp->n = 1; 
    ApTemp->r[0] = RegRetorno;
    ApTemp->p[1] = ApRetorno;
    ApTemp->p[0] = *Ap;  
    *Ap = ApTemp;
  }
}

void Reconstitui(TipoApontador ApPag, TipoApontador ApPai, 
                 int PosPai, short *Diminuiu, int *compRec)
{ 
  TipoPagina *Aux;  
  long DispAux, j;
  
  if (PosPai < ApPai->n)  /* Aux = TipoPagina a direita de ApPag */
  { 
    (*compRec)++;
    Aux = ApPai->p[PosPai+1];  
    DispAux = (Aux->n - M + 1) / 2;
    ApPag->r[ApPag->n] = ApPai->r[PosPai];
    ApPag->p[ApPag->n + 1] = Aux->p[0];  
    ApPag->n++;
    
    if (DispAux > 0)  /* Existe folga: transfere de Aux para ApPag */
    { 
      (*compRec)++;
      for (j = 1; j < DispAux; j++)
        InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j], compRec);
      
      ApPai->r[PosPai] = Aux->r[DispAux-1];  Aux->n -= DispAux;
      
      for (j = 0; j < Aux->n; j++) 
        Aux->r[j] = Aux->r[j + DispAux];
      for (j = 0; j <= Aux->n; j++) 
        Aux->p[j] = Aux->p[j + DispAux];
      
      *Diminuiu = FALSE;
    }
    else /* Fusao: intercala Aux em ApPag e libera Aux */
      { 
        for (j = 1; j <= M; j++)
          InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j], compRec);
        
        free(Aux);
        
        for (j = PosPai + 1; j < ApPai->n; j++) 
          { ApPai->r[j-1] = ApPai->r[j]; 
	    
        ApPai->p[j] = ApPai->p[j+1]; 
	  }
        ApPai->n--;
        if (ApPai->n >= M){
          (*compRec)++; 
          *Diminuiu = FALSE;
        }
      }
  }
  else /* Aux = TipoPagina a esquerda de ApPag */
    { 
      Aux = ApPai->p[PosPai-1]; 
      DispAux = (Aux->n - M + 1) / 2;
      
      for (j = ApPag->n; j >= 1; j--) 
        ApPag->r[j] = ApPag->r[j-1];
      ApPag->r[0] = ApPai->r[PosPai-1];
      
      for (j = ApPag->n; j >= 0; j--) 
        ApPag->p[j+1] = ApPag->p[j];
      ApPag->n++;
      
      if (DispAux > 0) /* Existe folga: transf. de Aux para ApPag */
      { 
        (*compRec)++;
        for (j = 1; j < DispAux; j++)
          InsereNaPagina(ApPag, Aux->r[Aux->n - j], 
	                 Aux->p[Aux->n - j + 1], compRec);
        ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
        ApPai->r[PosPai-1] = Aux->r[Aux->n - DispAux];
        Aux->n -= DispAux;  
        *Diminuiu = FALSE;
      }
      else /* Fusao: intercala ApPag em Aux e libera ApPag */
        { 
          for (j = 1; j <= M; j++)
            InsereNaPagina(Aux, ApPag->r[j-1], ApPag->p[j], compRec);
          
          free(ApPag);  ApPai->n--;
          
          if (ApPai->n >= M) {
          (*compRec)++; 
            *Diminuiu = FALSE;
          }
        }
    }
}

void Antecessor(TipoApontador Ap, int Ind, 
                TipoApontador ApPai, short *Diminuiu, int *compAnt)
{ 
  if (ApPai->p[ApPai->n] != NULL) 
  { 
    (*compAnt)++;
    Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu, compAnt);
    
    if (*Diminuiu){ 
      (*compAnt)++;
      Reconstitui(ApPai->p[ApPai->n], ApPai, (long)ApPai->n, Diminuiu, compAnt);
    }
    
    return;
  }
  
  Ap->r[Ind-1] = ApPai->r[ApPai->n - 1]; 
  ApPai->n--;  *Diminuiu = (ApPai->n < M);
} 

void Ret(TipoChave Ch, TipoApontador *Ap, short *Diminuiu, int *compRet)
{ 
  long j, Ind = 1;
  TipoApontador Pag;
  
  if (*Ap == NULL) 
  { 
    (*compRet)++;
    //printf("Erro: registro nao esta na arvore\n"); 
    *Diminuiu = FALSE;
    
    return;
  }
  
  Pag = *Ap;
  
  while (Ind < Pag->n && Ch.cpf > Pag->r[Ind-1].Chave.cpf){
    (*compRet)++; 
    Ind++;
  }
  
  if (Ch.cpf == Pag->r[Ind-1].Chave.cpf) 
  { 
    (*compRet)++;
    if (Pag->p[Ind-1] == NULL)   /* TipoPagina folha */
    { 
      (*compRet)++;
      Pag->n--;
      *Diminuiu = (Pag->n < M);
      
      for (j = Ind; j <= Pag->n; j++) 
        { 
          Pag->r[j-1] = Pag->r[j];  
          Pag->p[j] = Pag->p[j+1]; 
        }
      
      return;
    }
    /* TipoPagina nao e folha: trocar com antecessor */
    Antecessor(*Ap, Ind, Pag->p[Ind-1], Diminuiu, compRet);
    
    if (*Diminuiu) {
      (*compRet)++;
      Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu, compRet);
    }
    
    return;
  }
  
  if (Ch.cpf > Pag->r[Ind-1].Chave.cpf) {
    (*compRet)++;
    Ind++;
  }
    Ret(Ch, &Pag->p[Ind-1], Diminuiu, compRet);
  
  if (*Diminuiu) {
    (*compRet)++;
    Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu, compRet);
  }
}  

void Retira(TipoChave Ch, TipoApontador *Ap, int *compRet)
{ 
  short Diminuiu;
  TipoApontador Aux;
  
  Ret(Ch, Ap, &Diminuiu, compRet);
  
  if (Diminuiu && (*Ap)->n == 0)  /* Arvore diminui na altura */
  { 
    (*compRet)++;
    Aux = *Ap;   
    *Ap = Aux->p[0]; 
    
    free(Aux);
  }
}  

/* Código para transformar uma string em um inteiro
 * retirado do site http://blockofcodes.blogspot.com/2013/07/how-to-convert-string-to-integer-in-c.html
 */
int StringToInt(char *num)
{
  int len;
  int dec = 0, i;

  len = strlen(num);
  
  for(i=0; i<len; i++){
    dec = dec * 10 + ( num[i] - '0' );
  }

  return dec;
}


void Imprime(TipoApontador p) {
  if (p == NULL) {
    return;
  }

  for (int i = 0; i < p->n; i++) {
    Imprime(p->p[i]);
    printf("%s - %s ", p->r[i].Chave.nome, p->r[i].Chave.cpf);
  }

  printf("\n");
  Imprime(p->p[p->n]);
}
