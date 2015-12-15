#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <allegro.h>

#define DEBUG 1
#define IMAGENAME "ex12.bmp"
#define CORBRANCO (makecol(255,255,255))
#define CORPRETO 1
#define CORCINZA (makecol(160,160,160))
#define CORAZUL (makecol(0, 0, 255))
#define CORVERDE (makecol(0, 255, 0))
#define CORAMARELO (makecol(255,255,100))
#define CORVERMELHO (makecol(255, 0, 0))
#define X 800
#define Y 800
#define XCentro X/2.0
#define YCentro Y/2.0

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

typedef struct tadt
{
    pthread_t nth;
    struct transicoes *tr; /*Transicao que a thread esta rodando.*/
    struct tadt *prox;
}tadt;

void gerar_entrada(estados **p_estados, transicoes **p_transicoes);
void criar_threads(tadt **p_threads, transicoes *p_transicoes);
void espera_threads(tadt *p_threads);
void *roda_thread(void *dados);
void criar_estados(estados **p_estados, int num);
void criar_transicoes(transicoes **p_transicoes, arcos **p_arcos, int a1, int a2, int num);
void gerar_imagem(transicoes *p_transicoes);
void procurar(void);
void enviar_tokens(estados *e1, estados *e2, int num);
arcos *retirar_arco(arcos **p_arco);
void transferir_arco(arcos **p_arco, transicoes *p_transicao, int a1, int a2);
void criar_arcos(arcos **p_arcos, int a1, int a2);
void relacionar_tokens(estados *p_estados, int num);
estados *procurar_estado(estados *p_estados, int num);
transicoes *procurar_transicao(transicoes *p_transicoes, int num);
void debug(estados *p_estados, transicoes *p_transicoes);

static int est, tr, aet, ate;

int main(void)
{
    transicoes *cabeca_transicoes=NULL;
    estados *cabeca_estados=NULL;
    tadt *cabeca_threads=NULL;
    gerar_entrada(&cabeca_estados, &cabeca_transicoes);
    if(DEBUG)
        debug(cabeca_estados, cabeca_transicoes);
    criar_threads(&cabeca_threads, cabeca_transicoes);
    //espera_threads(cabeca_threads);
    gerar_imagem();
    return 0;
}

void gerar_entrada(estados **p_estados, transicoes **p_transicoes)
{
    int ect; /*Qtd estados, qtd transicoes, qtd estados com token, qtd arcos estado->transicao, qtd arcos transicao->estado*/
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
    criar_transicoes(p_transicoes, &cabeca_arcos, aet, ate, tr);
    if(DEBUG)
        printf("Funcao criar_transicoes funcionando.\n");
}

void criar_threads(tadt **p_threads, transicoes *p_transicoes)
{
    transicoes *pl=p_transicoes;
    tadt *pt=*p_threads, *plant=NULL;
    while(pl!=NULL)
    {
        pt=malloc(sizeof(tadt));
        pt->prox=NULL;
        pt->tr=pl;
        if(plant!=NULL)
            plant->prox=pt;
        else
            *p_threads=pt;
        plant=pt;
        pt=pt->prox;
        pl=pl->prox;
    }
    pt=*p_threads;
    while(pt!=NULL)
    {
        if(DEBUG)
            printf("Thread da transicao %d sendo criada.\n", pt->tr->ntr);
        pthread_create(&(pt->nth), NULL, roda_thread, (void *)&pt);
        pt=pt->prox;
    }
}

void espera_threads(tadt *p_threads)
{
    tadt *pt=p_threads;
    while(pt!=NULL)
    {
        if(DEBUG)
            printf("Esperando thread.\n");
        pthread_join(pt->nth, NULL);
        pt=pt->prox;
    }
}

void *roda_thread(void *dados)
{
    /*TODO: funcao que ira realizar a ativacao da troca de token e suas transferencias.*/
    tadt *pl=(tadt *)dados;
    if(DEBUG)
        printf("Thread da transicao %d reportando.\n", pl->tr->ntr);
    return NULL;
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

void criar_transicoes(transicoes **p_transicoes, arcos **p_arcos, int a1, int a2, int num)
{
    transicoes *pl=*p_transicoes, *plant;
    int aux=num;
    if(DEBUG)
        printf("Inicio da funcao criar_transicoes.\n");
    while(aux)
    {
        pl=malloc(sizeof(transicoes));
        pl->prox=NULL;
        pl->ntr=num-aux;
        pl->entram=NULL;
        pl->saem=NULL;
        if(aux!=num)
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
    BITMAP *buff;
    PALETTE pal;
    
    if(install_allegro(SYSTEM_NONE, &errno, atexit)!=0)
        exit(EXIT_FAILURE);
        
    set_color_depth(16);
    get_palette(pal);
    
    buff = create_bitmap(640,480);
    if(buff == NULL)
    {
       printf("Could not create buffer!\n");
       exit(EXIT_FAILURE);
    }

    save_bitmap(IMAGENAME, buff, pal);
    destroy_bitmap(buff);
    allegro_exit();

    printf("Imagem %s salva com sucesso!\n", IMAGENAME);
}

void enviar_tokens(estados *e1, estados *e2, int num)
{
    e1->nt-=num;
    e2->nt+=num;
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

