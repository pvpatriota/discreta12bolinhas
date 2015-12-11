#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define DEBUG 1

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
void debug(estados *p_estados, transicoes *p_transicoes);

int main(void)
{
    transicoes *cabeca_transicoes = NULL;
    estados *cabeca_estados = NULL;
    gerar_entrada(&cabeca_estados, &cabeca_transicoes);
    if(DEBUG)
        debug(cabeca_estados, cabeca_transicoes);
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
    if(DEBUG) 
        printf("Quantidade de estados: %u\n", est);
    if(DEBUG) 
        printf("Quantidade de transicoes: %u\n",tr);
    if(DEBUG) 
        printf("Quantidade de estados que possuem token: %u\n",ect);
    if(DEBUG) 
        printf("Quantidade de arcos que interligam estados e transicoes: %u\n",aet);
    if(DEBUG) 
        printf("Quantidade de arcos que interligam transicoes com estados: %u\n",ate);
    criar_estados(p_estados, est);
    if(DEBUG)
        printf("Funcao criar_estados funcionando.\n");
    relacionar_tokens(*p_estados, ect);
    if(DEBUG)
        printf("Funcao relacionar_tokens funcionando.\n");
    criar_arcos(&cabeca_arcos, aet, ate);
    if(DEBUG)
        printf("Funcao criar_arcos funcionando.\n");
    criar_transicoes(p_transicoes, &cabeca_arcos, aet, ate);
    if(DEBUG)
        printf("Funcao criar_transicoes funcionando.\n");
}

void criar_threads(void)
{
    /*TODO: criar funcao para criar threads.*/
    ;
}

void criar_estados(estados **p_estados, int num) /*Funcao responsavel pela criacao dos estados.*/
{
    if(DEBUG)
        printf("Iniciando criacao de estados, ira rodar por %d vezes.\n", num);
    int i=0;
    estados *pl, *plant;
    for(; i<num; i++)
    {
        if(DEBUG)
            printf("Criando estado %d\n", i);
        pl=malloc(sizeof(estados));
        pl->ne=i;
        pl->nt=0;
        pl->prox=NULL;
        if(i!=0)
        {
            if(DEBUG)
                printf("i!=0 Criacao do estado %d realizada com sucesso.\n", pl->ne);
            plant->prox=pl;
            plant=pl;
            pl=pl->prox;
        }
        else
        {
            *p_estados=pl;
            if(DEBUG)
                printf("i=0 Criacao do estado %d realizada com sucesso.\n", pl->ne);
            plant=pl;
            pl=pl->prox;
        }
    }
    if(DEBUG)
        printf("Fim da funcao criar_estados.\n");
}

void criar_transicoes(transicoes **p_transicoes, arcos **p_arcos, int a1, int a2)
{
    transicoes *pl=*p_transicoes, *plant;
    int aux=a1;
    if(DEBUG)
        printf("Inicio da funcao criar_transicoes.\n");
    while(aux)
    {
        pl=malloc(sizeof(transicoes));
        pl->prox=NULL;
        pl->ntr=a1-aux;
        pl->entram=NULL;
        pl->saem=NULL;
        if(aux!=a1)
        {
            plant->prox=pl;
            plant=pl;
            pl=pl->prox;
        }
        else
        {
            *p_transicoes=pl;
            plant=pl;
            pl=pl->prox;
        }
        aux--;
    }
    aux=a2;
    while(aux)
    {
        pl=malloc(sizeof(transicoes));
        pl->prox=NULL;
        pl->ntr=a2-aux;
        pl->entram=NULL;
        pl->saem=NULL;
        if(aux!=a2)
        {
            plant->prox=pl;
            plant=pl;
            pl=pl->prox;
        }
        else
        {
            *p_transicoes=pl;
            plant=pl;
            pl=pl->prox;
        }
        aux--;
    }
    if(DEBUG)
        printf("Iniciando transferencia de arcos.\n");
    transferir_arco(p_arcos, *p_transicoes, a1, a2);
    if(DEBUG)
        printf("Fim da funcao criar_transicoes.\n");
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
    if(DEBUG)
        printf("Entrando na funcao retirar_arco.\n");
    arcos *pl=*p_arco;
    *p_arco=pl->prox;
    if(DEBUG)
        if(pl!=NULL)
            printf("Terminando a funcao retirar_arco com sucesso.\nArco encontrado vai de %d a %d.\n", pl->origem, pl->destino);
    pl->prox=NULL;
    return pl;
}

void transferir_arco(arcos **p_arco, transicoes *p_transicao, int a1, int a2)
{
    if(DEBUG)
        printf("Iniciando funcao transferir_arco.\n");
    arcos *r, *aux, *plant;
    transicoes *pl;
    while(a1)
    {
        r=retirar_arco(p_arco);
        if(DEBUG)
            printf("Arco com origem em %d e destino em %d.\n", r->origem, r->destino);
        pl=procurar_transicao(p_transicao, r->destino);
        if(DEBUG)
            if(pl->ntr==r->destino)
                printf("Transicao encontrada.\n");
        if(DEBUG)
            printf("Inciando tranferencia do arco com origem no estado %d e detino na transicao %d.\n", r->origem, r->destino);
        if(pl->entram!=NULL)
        {
            aux=pl->entram;
            while(aux!=NULL)
            {
                plant=aux;
                aux=aux->prox;
            }
            plant->prox=r;
        }
        else
            pl->entram=r;
        a1--;
    }
    while(a2)
    {
        r=retirar_arco(p_arco);
        if(DEBUG)
            printf("Arco com origem em %d e destino em %d.\n", r->origem, r->destino);
        pl=procurar_transicao(p_transicao, r->origem);
        if(DEBUG)
            if(pl->ntr==r->origem)
                printf("Transicao encontrada.\n");
        if(DEBUG)
            printf("Inciando tranferencia do arco com origem na transicao %d e destino no estado %d.\n", r->origem, r->destino);
        if(pl->saem!=NULL)
        {
            aux=pl->saem;
            while(aux!=NULL)
            {
                plant=aux;
                aux=aux->prox;
            }
            plant->prox=r;
        }
        else
            pl->saem=r;
        a2--;
    }
    if(DEBUG)
        printf("Fim da funcao transferir_arco.\n");
}

void criar_arcos(arcos **p_arcos, int a1, int a2)
{
    if(DEBUG)
        printf("Iniciando funcao criar_arcos.\n");
    arcos *pl=*p_arcos, *plant;
    int x, y, z, aux=a1;
    while(a1+a2)
    {
        if(DEBUG)
            printf("A1+A2=%d.\n", a1+a2);
        pl=malloc(sizeof(arcos));
        scanf("%d %d %d", &x, &y, &z);
        pl->origem=x;
        pl->destino=z;
        pl->custo=y;
        pl->prox=NULL;
        if(aux!=a1)
        {
            plant->prox=pl;
            plant=pl;
            pl=pl->prox;
        }
        else
        {
            *p_arcos=pl;
            plant=*p_arcos;
            pl=pl->prox;
        }
        a1--;
    }
    if(DEBUG)
        printf("Fim da funcao criar_arcos.\n");
}

estados *procurar_estado(estados *p_estados, int num)
{
    if(DEBUG)
        printf("Iniciando funcao procurar_estado.\n");
    estados *pl=p_estados;
    if(DEBUG)
        printf("Estamos no estado %d.\n", pl->ne);
    while(pl!=NULL)
    {
        if(DEBUG)
            printf("O numero do estado sendo procurado e: %d. Estamos no estado: %d.\n", num, pl->ne);
        if(pl->ne==num)
        {
            if(DEBUG)
                printf("Estado %d encontrado.\n", pl->ne);
            return pl;
        }
        if(DEBUG)
            printf("Estamos no estado %d.\n", pl->ne);
        pl=pl->prox;
    }
    if(DEBUG)
        printf("Funcao procurar_estado retornando nulo.\n");
    return pl;
}

transicoes *procurar_transicao(transicoes *p_transicoes, int num)
{
    if(DEBUG)
        printf("Iniciando funcao procurar_transicao.\n");
    transicoes *pl=p_transicoes;
    if(DEBUG)
        if(pl==NULL)
            printf("procurar_transicao: pl = NULL.\n");
    while(pl!=NULL)
    {
        printf("1\n");
        if(pl->ntr==num)
            return pl;
        pl=pl->prox;
    }
    return pl;
}

void relacionar_tokens(estados *p_estados, int num)
{
    int est, tok; /*Estado e token.*/
    estados *pl;
    while(num)
    {
        if(DEBUG)
            printf("Entrando na funcao relacionar_tokens.\n");
        scanf("%d %d", &est, &tok);
        if(DEBUG)
            printf("O estado %d tem %d tokens.\n", est, tok);
        pl=procurar_estado(p_estados, est);
        if(DEBUG)
            printf("O resultado da funcao procurar_estado encontrou: %d\n", pl->ne);
        pl->nt=tok;
        if(DEBUG)
            printf("Colocando %d tokens no estado %d.\n", pl->nt, pl->ne);
        num--;
    }
}

void debug(estados *p_estados, transicoes *p_transicoes)
{
    estados *pl=p_estados, *plant;
    transicoes *plt=p_transicoes, *plantt;
    arcos *pla;
    printf("\n********** FUNCAO DEBUG!! **********\n");
    while(pl!=NULL)
    {
        plant=pl;
        pl=pl->prox;
    }
    printf("Numero de estados: %d.\nEstados com token:\n", plant->ne+1);
    pl=p_estados;
    while(pl!=NULL)
    {
        if(pl->nt!=0)
            printf("Estado %d com %d tokens.\n", pl->ne, pl->nt);
        pl=pl->prox;
    }
    while(plt!=NULL)
    {
        plantt=plt;
        plt=plt->prox;
    }
    printf("Numero de transicoes: %d.\n", plantt->ntr+1);
    plt=p_transicoes;
    while(plt!=NULL)
    {
        pla=plt->entram;
        while(pla!=NULL)
        {
            printf("Arco com origem no estado %d e com destino na transicao %d com custo de %d tokens.\n", pla->origem, pla->destino, pla->custo);
            pla=pla->prox;
        }
        plt=plt->prox;
    }
    plt=p_transicoes;
    while(plt!=NULL)
    {
        pla=plt->saem;
        while(pla!=NULL)
        {
            printf("Arco com origem na transicao %d e com destino no estado %d que entrega %d tokens.\n", pla->origem, pla->destino, pla->custo);
            pla=pla->prox;
        }
        plt=plt->prox;
    }
}

