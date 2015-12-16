#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <allegro.h>
#include <math.h>

#define ITERACOES 10000
#define DEBUG 0
#define IMAGENAME "ex12.bmp"
#define CORBRANCO (makecol(255,255,255))
#define CORPRETO 1
#define CORCINZA (makecol(160,160,160))
#define CORAZUL (makecol(0, 0, 255))
#define CORVERDE (makecol(0, 255, 0))
#define CORAMARELO (makecol(255,255,100))
#define CORVERMELHO (makecol(255, 0, 0))
#define X 640
#define Y 480
#define XCentro X/2.0
#define YCentro Y/2.0

typedef struct sestados /*Estrutura para armazenar estados.*/
{
    int ne, nt; /*Numero do estado e numero de tokens, respectivamente.*/
    struct sestados *prox;
}estados;

typedef struct stransicoes /*Estrutura para armazenar transicoes.*/
{
    int ntr; /*Numero da transicao*/
    struct stransicoes *prox;
    struct sarcos *entram;
    struct sarcos *saem;
}transicoes;

typedef struct sarcos /*Estrutura para armazenar arcos*/
{
    int origem, destino, custo; /*ponto de origem, destino e custo para transicao ou quantidade de token que a transicao ira enviar.*/
    struct sarcos *prox;
}arcos;

typedef struct stadt
{
    pthread_t nth;
    struct stransicoes *tr; /*Transicao que a thread esta rodando.*/
    struct sestados *std; /*Ira armazenar a cabeca dos estados.*/
    struct stadt *prox;
}tadt;

void gerar_entrada(estados **p_estados, transicoes **p_transicoes);
void criar_threads(tadt **p_threads, transicoes *p_transicoes, estados *p_estados);
void espera_threads(tadt *p_threads);
void *roda_thread(void *dados);
void criar_estados(estados **p_estados, int num);
void criar_transicoes(transicoes **p_transicoes, arcos **p_arcos, int a1, int a2, int num);
void gerar_imagem(transicoes *p_transicoes);
void desenha_estados(BITMAP *buff);
void desenha_transicoes(BITMAP *buff, transicoes *p_transicoes);
void desenha_arcos(int qo, int qf, BITMAP *buff, int k, int c, int flag);
void enviar_tokens(estados *e1, estados *e2, int num);
arcos *retirar_arco(arcos **p_arco);
void transferir_arco(arcos **p_arco, transicoes *p_transicao, int a1, int a2);
void criar_arcos(arcos **p_arcos, int a1, int a2);
void relacionar_tokens(estados *p_estados, int num);
estados *procurar_estado(estados *p_estados, int num);
transicoes *procurar_transicao(transicoes *p_transicoes, int num);
void debug(estados *p_estados, transicoes *p_transicoes);
float arctan(float x1, float y1, float x2, float y2);

static int est, tr, aet, ate; /*Qtd de estados, qtd de transicoes, qtd de arcos estado->transicoes e qtd de arcos transicoes->estados*/

int main(void)
{
    transicoes *cabeca_transicoes=NULL;
    estados *cabeca_estados=NULL;
    tadt *cabeca_threads=NULL;
    gerar_entrada(&cabeca_estados, &cabeca_transicoes);
    printf("\nANTES DA SIMULACAO");
    debug(cabeca_estados, cabeca_transicoes);
    criar_threads(&cabeca_threads, cabeca_transicoes, cabeca_estados);
    espera_threads(cabeca_threads);
    gerar_imagem(cabeca_transicoes);
    printf("\nDEPOIS DA SIMULACAO");
    debug(cabeca_estados, cabeca_transicoes);
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

void criar_threads(tadt **p_threads, transicoes *p_transicoes, estados *p_estados)
{
    transicoes *pl=p_transicoes;
    tadt *pt=*p_threads, *plant=NULL;
    while(pl!=NULL)
    {
        pt=malloc(sizeof(tadt));
        pt->prox=NULL;
        pt->tr=pl;
        pt->std=p_estados;
        if(plant!=NULL)
            plant->prox=pt;
        else
            *p_threads=pt;
        plant=pt;
        pt=pt->prox;
        pl=pl->prox;
    }
    pt=*p_threads;
    if(DEBUG)
    {
        printf("*****Lista de threads******.\n");
        while(pt!=NULL)
        {
            printf("thread da transicao %d.\n", pt->tr->ntr);
            pt=pt->prox;
        }
        pt=*p_threads;
        printf("*****Lista de threads******.\n");
    }
    while(pt!=NULL)
    {
        if(DEBUG)
            printf("Thread da transicao %d sendo criada.\n", pt->tr->ntr);
        pthread_create(&(pt->nth), NULL, roda_thread, (void *)pt);
        pt=pt->prox;
    }
}

void espera_threads(tadt *p_threads)
{
    tadt *pt=p_threads;
    while(pt!=NULL)
    {
        if(DEBUG)
            printf("Esperando thread %d.\n", pt->tr->ntr);
        pthread_join(pt->nth, NULL);
        pt=pt->prox;
    }
}

void *roda_thread(void *dados)
{
    int cont=1, aux;
    tadt *pl=(tadt *)dados;
    arcos *pa;
    estados *pe;
    while(cont<=ITERACOES)
    {
        aux=1;
        pa=pl->tr->entram;
        while(pa!=NULL)
        {
            pe=procurar_estado(pl->std, pa->origem);
            if(pa->custo>pe->nt)
                aux=0;
            pa=pa->prox;
        }
        if(aux)
        {
            pa=pl->tr->entram;
            while(pa!=NULL)
            {
                pe=procurar_estado(pl->std, pa->origem);
                pe->nt-=pa->custo;
                pa=pa->prox;
            }
            pa=pl->tr->saem;
            while(pa!=NULL)
            {
                pe=procurar_estado(pl->std, pa->destino);
                pe->nt+=pa->custo;
                pa=pa->prox;
            }
        }
        cont++;
    }
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

void gerar_imagem(transicoes *p_transicoes)
{
    BITMAP *buff;
    PALETTE pal;
    arcos *pt = p_transicoes->entram;
    arcos *pl = p_transicoes->saem;
    int k=1,flag;

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
    desenha_estados(buff);
    desenha_transicoes(buff, p_transicoes);

    while(1)
    {
        if(DEBUG)
            printf("Criando arcos\n");
        flag=1;
        while(pt !=NULL )
        {
            desenha_arcos(pt->origem*2,pt->destino+k,buff,est*2,pt->custo,flag);
            if(DEBUG)
                printf("Lugar: %d ->  Transicao: %d\n", pt->origem, pt->destino);
            pt=pt->prox;

        }
        flag=0;
        while(pl !=NULL )
        {
            desenha_arcos(pl->origem+k,pl->destino*2,buff,est*2,pl->custo,flag);
            if(DEBUG)
                printf("Transicao: %d -> Lugar: %d\n", pl->origem, pl->destino);
            pl=pl->prox;

        }
        k++;
        p_transicoes=p_transicoes->prox;
        if(p_transicoes==NULL)
            break;
        pt=p_transicoes->entram;
        pl=p_transicoes->saem;
    }

    save_bitmap(IMAGENAME, buff, pal);
    destroy_bitmap(buff);
    allegro_exit();

    printf("Imagem %s salva com sucesso!\n", IMAGENAME);
}

void desenha_estados(BITMAP *buff)
{
    int i;
    float raio,xi,yi,rc;
    raio = (Y/8)*(M_PI/(M_PI+est));
    rc = YCentro - raio*4;

    if(DEBUG)
        printf("Iniciando a funcao desenha_arcos\n");
    for(i=0;i<est;i++)
    {
        if(DEBUG)
            printf("Criando os estados.\n");
        yi=YCentro+rc*cos((2*M_PI/est)*i);
        xi=XCentro+rc*sin((2*M_PI/est)*i);
        circle(buff, xi, yi, raio, CORBRANCO);
        textprintf_ex(buff, font, (xi-18), (yi-5), CORVERDE, CORPRETO, "Est %d",i);
    }
    if(DEBUG)
        printf("Estados criados com sucesso");
}

void desenha_transicoes(BITMAP *buff, transicoes *p_transicoes)
{
    int i,c=0,k=1;
    float xi,yi,rc,raio;
    transicoes *pl = p_transicoes;
    raio = (Y/8)*(M_PI/(M_PI+est));
    rc = YCentro - raio*4;
    raio = (Y/12)*(M_PI/(M_PI+est));

    while(1)
    {
        if(DEBUG)
            printf("Inicializando o laco para a criacao das transicoes\n");
        for(i=k;i<tr*2;i++)
        {
            if(DEBUG)
                printf("Iniciando a criacao das transicoes e ira rodar %d vezes.\n", tr);
            yi=YCentro+rc*cos((2*M_PI/(est*2))*i);
            xi=XCentro+rc*sin((2*M_PI/(est*2))*i);
            line(buff, (xi), (yi)+raio, (xi), (yi)-raio, CORBRANCO);
            if(M_PI/2<=(2*M_PI/(est*2))*i && (3*M_PI)/2>(2*M_PI/(est*2))*i)
                textprintf_ex(buff, font, xi-10, yi-raio-12, CORVERDE, CORPRETO, "Tr%d",c++);
            else
            {
                textprintf_ex(buff, font, xi-10, yi+raio+5, CORVERDE, CORPRETO, "Tr%d",c++);
                break;
            }
            i++;
        }
        if(DEBUG)
            printf("Criacao das transicoes realizada com sucesso.\n");
        k = i+2;
        if(pl->prox!=NULL)
        {
            pl=pl->prox;
        }
        else
            break;
    }
}

void desenha_arcos(int qo, int qf, BITMAP *buff, int k, int c, int flag)
{
    float delta, alfa, beta, phi, x1, y1, x2, y2, x3, y3, xo, yo, xf, yf, raio, xt1, yt1, xt2, yt2, rc;
    raio=(Y/8)*(M_PI/(M_PI+est));
    rc = YCentro - raio*4;

    y1 = YCentro + rc*cos((2*M_PI/k)*qo);
    x1 = XCentro + rc*sin((2*M_PI/k)*qo);
    y3 = YCentro + rc*cos((2*M_PI/k)*qf);
    x3 = XCentro + rc*sin((2*M_PI/k)*qf);

    alfa=arctan(x1,y1,x3,y3);
    y2=(y3+y1)/2 + raio * cos(alfa);
    x2=(x3+x1)/2 - raio * sin(alfa);

    if(((alfa >= 0) && (alfa <= M_PI/2)) || ((alfa >= M_PI) && (alfa <= 3*M_PI/2)))
    {
        beta=arctan(x3,y3,x2,y2);
        phi=arctan(x1,y1,x2,y2);
        xo = x1 + raio * cos(phi);
        yo = y1 + raio * sin(phi);
        xf = x3 + raio * cos(beta);
        yf = y3 + raio * sin(beta);
    }
    else
    {
        alfa=arctan(x3,y3,x1,y1);
        y2=(y3+y1)/2 + raio * cos(alfa);
        x2=(x3+x1)/2 - raio * sin(alfa);
        beta=arctan(x1,y1,x2,y2);
        phi=arctan(x3,y3,x2,y2);
        if(flag)
        {
            xo = x1 - raio * cos(phi);
            yo = y1 - raio * sin(phi);
            xf = x3;
            yf = y3;
        }
        else
        {
            xo = x1;
            yo = y1;
            xf = x3 - raio * cos(beta);
            yf = y3 - raio * sin(beta);
        }
    }

    int coo[8];
    coo[0] = (int)xo;
    coo[1] = (int)yo;
    coo[2] = (int)x2;
    coo[3] = (int)y2;
    coo[4] = (int)x2;
    coo[5] = (int)y2;
    coo[6] = (int)xf;
    coo[7] = (int)yf;
    spline(buff,coo,CORBRANCO);

    delta=arctan(x2,y2,x3,y3);
    xt2 = xf - (raio / 4) * (sin(delta) + cos(delta));
    yt2 = yf + (raio / 4) * (sin(delta) - cos(delta));
    xt1 = xf + (raio / 4) * (sin(delta) - cos(delta));
    yt1 = yf - (raio / 4) * (sin(delta) + cos(delta));

    triangle(buff, xt1, yt1, xt2, yt2, xf, yf, CORBRANCO);
    textprintf_ex(buff, font, x2, y2, CORVERDE, CORPRETO, "%d", c);

}

float arctan(float x1, float y1, float x2, float y2)
{
    if(x2 == x1)
    {
        if(y2 == y1)
            return 9.0;
        else
            if(y2>y1)
                return M_PI/2.0;
        return 3.0*M_PI/2.0;
    }
    if((y2 == y1) && (x2 < x1))
        return M_PI;
    float a = atan(fabs(y2-y1)/fabs(x2-x1));
    if((x2 < x1) && (y2 > y1)) /* 2º quadrante*/
        return a + M_PI/2.0;
    if((x2 < x1) && (y2 < y1)) /* 3º quadrante*/
        return a + M_PI;
    if((x2 > x1) && (y2 < y1)) /* 4º quadrante*/
        return a + 3.0*M_PI/2.0;
    return a; /* 1º quadrante*/
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
    /*if(DEBUG)
      printf("Iniciando funcao procurar_estado.\n");*/
    estados *pl=p_estados;
    /*if(DEBUG)
      printf("Estamos no estado %d.\n", pl->ne);*/
    while(pl!=NULL)
    {
        /*if(DEBUG)
          printf("O numero do estado sendo procurado e: %d. Estamos no estado: %d.\n", num, pl->ne);*/
        if(pl->ne==num)
        {
            /*if(DEBUG)
              printf("Estado %d encontrado.\n", pl->ne);*/
            return pl;
        }
        /*if(DEBUG)
          printf("Estamos no estado %d.\n", pl->ne);*/
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
    printf("\n*************************************\n");
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
    if(DEBUG)
    {
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
}

