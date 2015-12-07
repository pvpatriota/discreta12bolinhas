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
void criar_transicoes(transicoes **p_transicoes, arcos **p_arcos);
void gerar_imagem(void);
void procurar(void);
void enviar_tokens(void);
arcos *retirar_arco(arcos **p_arco);
void transferir_item(void);
void criar_arcos(arcos **p_arcos, int a1, int a2);
void relacionar_tokens(void);

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
    /*Lembrete! Funcao deve receber ponteiro com cabeca dos estados e ponteiro com cabeca das transicoes.*/
    int est, tr, ect, aet, ate, i; /*Qtd estados, qtd transicoes, qtd estados com token, qtd arcos estado->transicao, qtd arcos transicao->estado*/
    arcos *cabeca_arcos=NULL;
    /*TODO: ler linhas estaticas*/
    criar_estados(p_estados, tr);
    for(i=0; i<ect; i++)
    {
        /*Este for deve estar dentro da funcao relacionar_tokens.*/
        relacionar_tokens();
    }
    criar_arcos(&cabeca_arcos, est, tr);
    criar_transicoes(p_transicoes, &cabeca_arcos);
}

void criar_threads(void)
{
    /*TODO: criar funcao para criar threads.*/
    ;
}

void criar_estados(estados **p_estados, int num)
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
        if(plant!=NULL)
        {
            plant->prox=pl;
            pl=pl->prox;
        }
        else
        {
            *p_estados=pl;
        }
    }
}

void criar_transicoes(transicoes **p_transicoes, arcos **p_arcos)
{
    /*TODO: criar funcao para criar as transicoes da rede de petri.*/
    /*TODO: relacionar arcos com as transicoes.*/
    ;
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

void transferir_arco(arcos **p_arco, transicoes *p_transicao)
{
    arcos *r;
    while(p_arco!=NULL)
    {
        r=retirar_arco(p_arco);
        /*TODO: colocar r na transicao que ele esta ligado.*/
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

void relacionar_tokens(void)
{
    /*TODO: criar funcao para relacionar os tokens da entrada com os seus respectivos estados.*/
    ;
}

