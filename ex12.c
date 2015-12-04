#include <stdio.h>
#include <pthreads.h>
#include <stdlib.h>

typedef struct estados /*Estrutura para definir os estados.*/
{
    int ne, nt; /*Numero do estado e numero de tokens, respectivamente.*/
    struct estados prox;
}estados;

void gerar_entrada(void);
void criar_threads(void);
void criar_estados(void);
void criar_transicoes(void);
void gerar_imagem(void);
void procurar(void);

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

