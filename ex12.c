#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct estados /*Estrutura para armazenar estados.*/
{
    int ne, nt; /*Numero do estado e numero de tokens, respectivamente.*/
    struct estados *prox;
}estados;

typedef struct transicoes /*Estrutura para armazenar transicoes.*/
{
    int ntr; /*Numero da transicao*/
    struct transicoes *prox;
    struct arcos *entram;
    struct arcos *saem;
}transicoes;

typedef struct arcos /*Estrutura para armazenar arcos*/
{
    int origem, destino, custo; /*ponto de origem, destino e custo para transicao ou quantidade de token que a transicao ira enviar.*/
    struct arcos *prox;
}arcos;

void gerar_entrada(estados **p_estados, transicoes **p_transicoes);
void criar_threads(void);
void criar_estados(estados **p_estados, int num);
void criar_transicoes(transicoes **p_transicoes, arcos **p_arcos, int a1, int a2);
void gerar_imagem(void);
void procurar(void);
void enviar_tokens(void);
arcos *retirar_arco(arcos **p_arco);
void transferir_arco(arcos **p_arco, transicoes *p_transicao, int a1, int a2);
void criar_arcos(arcos **p_arcos, int a1, int a2);
void relacionar_tokens(estados *p_estados, int num);
estados *procurar_estado(estados *p_estados, int num);
transicoes *procurar_transicao(transicoes *p_transicoes, int num);

int main(void)
{
    transicoes *cabeca_transicoes = NULL;
    estados *cabeca_estados = NULL;
    gerar_entrada(&cabeca_estados, &cabeca_transicoes);
    criar_threads();
    /*TODO: funcao para esperar todas as threads retornarem.*/
    gerar_imagem();
    return 0;
}

void gerar_entrada(estados **p_estados, transicoes **p_transicoes)
{
    int est, tr, ect, aet, ate; /*Qtd estados, qtd transicoes, qtd estados com token, qtd arcos estado->transicao, qtd arcos transicao->estado*/
    arcos *cabeca_arcos=NULL;
    scanf("%d", &est); /*Lendo cinco linhas iniciais do arquivo de entrada.*/
    scanf("%d", &tr);
    scanf("%d", &ect);
    scanf("%d", &aet);
    scanf("%d", &ate);
    criar_estados(p_estados, tr);
    relacionar_tokens(*p_estados, ect);
    criar_arcos(&cabeca_arcos, est, tr);
    criar_transicoes(p_transicoes, &cabeca_arcos, aet, ate);
}

void criar_threads(void)
{
    /*TODO: criar funcao para criar threads.*/
    ;
}

void criar_estados(estados **p_estados, int num) /*Funcao responsavel pela criacao dos estados.*/
{
    int i=0;
    estados *pl=NULL;
    estados *plant=NULL;
    for(; i<num; i++)
    {
        pl=malloc(sizeof(estados));
        pl->ne=i;
        pl->nt=0;
        pl->prox=NULL;
        if(plant!=NULL)  /*Pode ser otimizado! Variavel plant nao e necessaria.*/
        {
            plant->prox=pl;
            pl=pl->prox;
        }
        else
        {
            *p_estados=pl;
            pl=pl->prox;
        }
    }
}

void criar_transicoes(transicoes **p_transicoes, arcos **p_arcos, int a1, int a2)
{
    transicoes *p2=*p_transicoes;
    int aux=a1;
    while(aux)
    {
        p2=malloc(sizeof(transicoes));
        p2->prox=NULL;
        p2->ntr=a1-aux;
        p2->entram=NULL;
        p2->saem=NULL;
        if(aux!=a1)
            p2=p2->prox;
        else
        {
            *p_transicoes=p2;
            p2=p2->prox;
        }
        aux--;
    }
    aux=a2;
    while(aux)
    {
        p2=malloc(sizeof(transicoes));
        p2->prox=NULL;
        p2->ntr=a1-aux;
        p2->entram=NULL;
        p2->saem=NULL;
        if(aux!=a2)
            p2=p2->prox;
        else
        {
            *p_transicoes=p2;
            p2=p2->prox;
        }
        aux--;
    }
    transferir_arco(p_arcos, *p_transicoes, a1, a2);
}

void gerar_imagem(void)
{
    /*TODO: criar funcao para gerar imagem da rede de petri.*/
    ;
}

void enviar_tokens(void)
{
    /*TODO: criar funcao para transicao enviar tokens.*/
    ;
}

arcos *retirar_arco(arcos **p_arco)
{
    arcos *pl=*p_arco;
    if(pl==NULL)
        return pl;
    *p_arco=pl->prox;
    pl->prox=NULL;
    return pl;
}

void transferir_arco(arcos **p_arco, transicoes *p_transicao, int a1, int a2)
{
    arcos *r, *aux;
    transicoes *pl;
    while(a1)
    {
        *pl=*p_transicao;
        r=retirar_arco(p_arco);
        while(r->origem!=pl->ntr)
            pl=pl->prox;
        aux=pl->entram;
        while(aux->prox!=NULL)
            aux=aux->prox;
        aux=r;
        a1--;
    }
    while(a2)
    {
        *pl=*p_transicao;
        r=retirar_arco(p_arco);
        while(r->destino!=pl->ntr)
            pl=pl->prox;
        aux=pl->saem;
        while(aux->prox!=NULL)
            aux=aux->prox;
        aux=r;
        a2--;
    }
}

void criar_arcos(arcos **p_arcos, int a1, int a2)
{
    arcos *pl=*p_arcos;
    int x, y, z, aux=a1;
    while((a1+a2)>0)
    {
        pl=malloc(sizeof(arcos));
        scanf("%d %d %d", &x, &y, &z);
        pl->origem=x;
        pl->destino=z;
        pl->custo=y;
        if(aux==a1)
            *p_arcos=pl;
        pl=pl->prox=NULL;
        a1--;
    }
}

estados *procurar_estado(estados *p_estados, int num)
{
   estados *pl=p_estados;
   while(pl!=NULL)
   {
       if(pl->ne==num)
           return pl;
       pl=pl->prox;
   }
   return pl;
}

transicoes *procurar_transicao(transicoes *p_transicoes, int num)
{
    transicoes *pl=p_transicoes;
    while(pl!=NULL)
    {
        if(pl->ntr==num)
            return pl;
        pl=pl->prox;
    }
    return pl;
}

void relacionar_tokens(estados *p_estados, int num)
{
    int est, tok; /*Estado e token.*/
    estados *pl=NULL;
    while(num)
    {
        scanf("%d %d", &est, &tok);
        pl=procurar_estado(p_estados, est);
        pl->nt=tok;
        num--;
    }
}

