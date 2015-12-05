#include <stdio.h>
#include <pthreads.h>
#include <stdlib.h>

typedef struct estados /*Estrutura para armazenar estados.*/
{
    int ne, nt; /*Numero do estado e numero de tokens, respectivamente.*/
    struct estados prox;
}estados;

typedef struct transicoes /*Estrutura para armazenar transicoes.*/
{
    int ntr; /*Numero da transicao*/
    struct transicoes prox;
    struct arcos cabeca;
}transsicoes;

typedef struct arcos /*Estrutura para armazenar arcos*/
{
    int origem, corigem, destino, cdestino; /*ponto de origem, custo da origem, destino e custo do destino, respectivamente.*/
    struct arcos prox;
}arcos;

void gerar_entrada(void);
void criar_threads(void);
void criar_estados(void);
void criar_transicoes(void);
void gerar_imagem(void);
void procurar(void);
void receber_tokens(void);
void enviar_tokens(void);
void retirar_item(void);
void transferir_item(void);

int main(void)
{
    /*TODO: criar bloco base.*/
    return 0;
}

void gerar_entrada(void)
{
    /*TODO: criar funcao para gerar a entrada.*/
    ;
}

void criar_threads(void)
{
    /*TODO: criar funcao para criar threads.*/
    ;
}

void criar_estados(void)
{
    /*TODO: criar funcao para criar os estados da rede de petri.*/
    ;
}

void criar_transicoes(void)
{
    /*TODO: criar funcao para criar as transicoes da rede de petri.*/
    ;
}

void gerar_imagem(void)
{
    /*TODO: criar funcao para gerar imagem da rede de petri.*/
    ;
}

void procurar(void);
{
    /*TODO: criar funcao para procurar determinada posicao.*/
    ;
}

void receber_tokens(void)
{
    /*TODO: criar funcao para transicao receber tokens.*/
    ;
}

void enviar_tokens(void)
{
    /*TODO: criar funcao para transicao enviar tokens.*/
    ;
}

void retirar_item(void)
{
    /*TODO: criar funcao para retirar item de uma lista.*/
    ;
}

void transferir_item(void)
{
    /*TODO: criar funcao para transferir item de uma lista para outra.*/
    ;
}

