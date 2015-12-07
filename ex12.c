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
    struct arcos *cabeca;
}transicoes;

typedef struct arcos /*Estrutura para armazenar arcos*/
{
    int origem, corigem, destino, cdestino; /*ponto de origem, custo da origem, destino e custo do destino, respectivamente.*/
    struct arcos *prox;
}arcos;

void gerar_entrada(estados **p_estados, transicoes **p_transicoes);
void criar_threads(void);
void criar_estados(estados **p_estados, int num);
void criar_transicoes(transicoes **p_transicoes, arcos **p_arcos);
void gerar_imagem(void);
void procurar(void);
void enviar_tokens(void);
arcos *retirar_arco(arcos **p_arco, arcos *r);
void transferir_item(void);
void criar_arcos(void);
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
    for(i=0; i<(aet+ate); i++)
    {
        /*Este for deve estar dentro da funcao criar_arcos.*/
        criar_arcos();
    }
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

void procurar_arco(void)
{
    /*TODO: criar funcao para procurar determinada posicao.*/
    ;
}

void enviar_tokens(void)
{
    /*TODO: criar funcao para transicao enviar tokens.*/
    ;
}

arcos *retirar_arco(arcos **p_arco, arcos *r)
{
    arcos *pl=*p_arco;
    arcos *plant=NULL;
    if(r==NULL || pl==NULL)
        return NULL;
    while(pl!=r && p_arco!=NULL)
    {
        plant=pl;
        pl=pl->prox;
    }
    if(plant!=NULL)
    {
        pl->prox=plant->prox;
        plant->prox=pl;
    }
    else
        *p_arco=pl->prox;
    return pl;
}

void transferir_arco(void)
{
    /*TODO: adicionar item retirado na lista de destino..*/
    ;
}

void criar_arcos(void)
{
    /*TODO: criar funcao para gerar lista de arcos.*/
    /*Deve retornar o ponteiro para os arcos.*/
    ;
}

void relacionar_tokens(void)
{
    /*TODO: criar funcao para relacionar os tokens da entrada com os seus respectivos estados.*/
    ;
}

