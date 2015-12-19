/***************************************************************************
 *   Programa, versão                                                      *
 *                                                                         *
 *   Programa que simula a rede de petri utilizando threads e listas.      *
 *                                                                         *
 *   Copyright (C) 2015  Joao Pedro e Paulo Vitor                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, read it online                       *
 *   at <http://www.gnu.org/licenses/>, or write to the                    *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 *   To contact the author, please write to:                               *
 *   João Pedro Pacheco Rodrigues Almeida                                  *
 *   Paulo Vitor Alves Patriota                                            *
 *   Email: paulovitorpatriota@gmail.com                                   *
 *   Webpage: http://www.upe.br                                            *
 *   Phone: +55 (81) 9-9966-1154                                           *
 *          +55 (81) 9-9337-5011                                           *
 ***************************************************************************/

/**
 * \file ex12.c
 * \brief Programa que simula a rede de petri utilizando threads e listas.
 * \author Joao Pedro Pacheco Rodrigues Almeida e Paulo Vitor Alves Patriota.
 * \date 2015-12-19
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <allegro.h>
#include <math.h>

/* ---------------------------------------------------------------------- */
/**
* \file ex12.c
* \ingroup DEFINES
* \brief - Os defines utilizados para o funcionamento correto do programa, sem utilizar numeros dentro do código.
*/
#define ITERACOES 10000
#define DEBUG 0
#define IMAGENAME "ex12.bmp" /**Nome do arquivo que sera gerado pelo allegro*/
#define CORBRANCO (makecol(255,255,255))
#define CORPRETO 1
#define CORCINZA (makecol(160,160,160))
#define CORAZUL (makecol(0, 0, 255))
#define CORVERDE (makecol(0, 255, 0))
#define CORAMARELO (makecol(255,255,100))
#define CORVERMELHO (makecol(255, 0, 0))
#define X 640 /**Definicao do tamanho X da tela */
#define Y 480 /**Definicao do tamanho Y da tela */
#define XCentro X/2.0 /**Definicao do centro da tela da coordenada X*/
#define YCentro Y/2.0 /**Definicao do centro da tela da coordenada Y*/

/* ---------------------------------------------------------------------- */
/**
 * \file ex12.c
 * @ingroup Estruturas
 * @brief - Estruturas utilizadas para o correto funcionamento do programa.
 */

/* ---------------------------------------------------------------------- */
/**
 * \file ex12.c
 * \ingroup Estruturas
 * \brief  - A estrutura sestados cujo o nome definido foi estados, irá armazenar as informacoes sobre os estados da rede de petri.
 * \param[in] - Ne recebe o número de estados.
 * \param[in] - Nt recebe o numero de tokens de cada estado.
 * \param - O ponteiro prox aponta para o elemento seguinte da lista. Este ponteiro também é uma variável da estrutura sestados.
 */
typedef struct sestados /*Estrutura para armazenar estados.*/
{
    int ne, nt; /*Numero do estado e numero de tokens, respectivamente.*/
    struct sestados *prox;
}estados;

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Estruturas
 * @brief - A estrutura stransicoes cujo o nome definido foi transicoes, irá armazenar as informacoes sobre as transicoes da rede de petri.
 * @param[in] - NTR recebe o número de transicoes do arquivo de leitura.
 * @param[out] -  O ponteiro prox aponta para o elemento seguinte da lista. Este ponteiro também é uma variável da estrutura stransicoes.
 * @param[out] -  O ponteiro entram é um ponteiro de estrutura sarcos e este ponteiro é responsavel por armazenar a quantidade de arcos que entram na respectiva transicao.
 * @param[out] - O ponteiro saem é um ponteiro de estrutura sarcos e este ponteiro é responsável por armazenar a quantidade de arcos que saem da respectiva transicao.
 */
typedef struct stransicoes /*Estrutura para armazenar transicoes.*/
{
    int ntr; /*Numero da transicao*/
    struct stransicoes *prox;
    struct sarcos *entram;
    struct sarcos *saem;
}transicoes;

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Estruturas
 * @brief - A estrutura sarcos cujo o nome definido foi arcos, irá armazenar as informacoes sobre os arcos da rede de petri.
 * @param[in] - A variavel ORIGEM do tipo int, irá indicar o ponto de origem da transicao, ou seja, de qual transicao o token irá sair.
 * @param[in] - A variável DESTINO do tipo int, irá indicar o ponto de chegada de uma transicao, ou seja, em qual transicao o token irá chegar.
 * @param[in] - A variável CUSTO do tipo int, irá indicar o custo da transicao, ou seja, a quantidade de tokens que uma determinada transicao irá enviar.
 * @param[out] - O ponteiro prox aponta para o elemento seguinte da lista. Este ponteiro é uma variável da estrutura stadt, que é a estrutura responsável pelo auxilo na hora de criar as threads.
 */

typedef struct sarcos /*Estrutura para armazenar arcos*/
{
    int origem, destino, custo; /*ponto de origem, destino e custo para transicao ou quantidade de token que a transicao ira enviar.*/
    struct sarcos *prox;
}arcos;

/* ---------------------------------------------------------------------- */
/**
 * \ingroup Estruturas
 * \brief - A estrutura stadt cujo o nome definido foi tadt, irá auxiliar o programa na criação das threads.
 */
typedef struct stadt /*Estrutura para auxiliar na criacao das threads.*/
{
    pthread_t nth;
    struct stransicoes *tr; /*Transicao que a thread esta rodando.*/
    struct sestados *std; /*Ira armazenar a cabeca dos estados.*/
    struct stadt *prox;
}tadt;

/* ---------------------------------------------------------------------- */
/** 
 * @ingroup Protótipos das funcoes
 * @brief - Declaração dos protótipos das funcoes que serão utilizadas para simular a rede de petri.
 */
void gerar_entrada(estados **p_estados, transicoes **p_transicoes); /*Funcao respostavel por ler e administrar entrada do programa.*/
void criar_threads(tadt **p_threads, transicoes *p_transicoes, estados *p_estados); /*Funcao responsavel por criar as threads.*/
void espera_threads(tadt *p_threads);/*Funcao responsavel por esperar todas as threads terminarem seus trabalhos.*/
void *roda_thread(void *dados); /*Funcao que sera rodada pelas threads.*/
void criar_estados(estados **p_estados, int num); /*Funcao que, dentro da funcao gerar_entrada, ira criar os estados.*/
void criar_transicoes(transicoes **p_transicoes, arcos **p_arcos, int a1, int a2, int num); /*Ira criar as transicoes.*/
void gerar_imagem(transicoes *p_transicoes);
void desenha_estados(BITMAP *buff);
void desenha_transicoes(BITMAP *buff, transicoes *p_transicoes);
void desenha_arcos(int qo, int qf, BITMAP *buff, int k, int c, int flag);
arcos *retirar_arco(arcos **p_arco); /*Funcao que ira retirar um arco da lista de arcos, funciona dentro da funcao transferir_arco.*/
void transferir_arco(arcos **p_arco, transicoes *p_transicao, int a1, int a2); /*Funcao que transefere arcos da lista principal para as transicoes que eles pertencem.*/
void criar_arcos(arcos **p_arcos, int a1, int a2); /*Funcao que, dentro da funcao gerar_entrada, ira criar a lista de arcos.*/
void relacionar_tokens(estados *p_estados, int num); /*Funcao que ira relacionar os tokens com seus estados.*/
estados *procurar_estado(estados *p_estados, int num); /*Funcao auxiliar para buscar um estado da lista.*/
transicoes *procurar_transicao(transicoes *p_transicoes, int num); /*Funcao auxiliar para procurar transicao.*/
void debug(estados *p_estados, transicoes *p_transicoes); /*Funcao para mostrar dados do programa na tela.*/
float arctan(float x1, float y1, float x2, float y2);

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Variaveis Globais
 * @brief - Variáveis globais declaradas para receber os dados de entrada dos arquivos de texto e para serem utilizadas principalmente no momento de gerar a imagem com o allego.
 */
static int est, tr, aet, ate; /*Qtd de estados, qtd de transicoes, qtd de arcos estado->transicoes e qtd de arcos transicoes->estados*/

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Main.
 * @brief - Funcao Main do código, que será a responsável por chamar as funcoes nas devidas ordens para o correto funcionamento.
 *
 * A funcao main, chamara as seguintes funcoes respectivamente:
 * Gerar_entrada que ira receber todos os dados dos arquivos de texto disponibilizados.
 * Debug que tem como funcao, realizar o DEBUG do código mostrando possiveis erros.
 * Criar_threads criará as threads para executar o paralelismo das acoes,
 * Espera_threads é a funcao que irá esperar as threads terminarem de executar suas funcoes.
 * Gerar_imagem cria uma imagem do tipo .bmp utilizando o allegro e salva um arquivo com o nome ex12.bmp.
 * Para finalizar, chama novamente a funcao debug para mostrar possiveis erros.
 */
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
/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao gerar_entrada é responsável por receber os dados dos arquivos de texto que foram disponibilizados, realizam a leitura dos mesmos e armazenam os seus dados nas variavéis globais.
 * @param[in] - Ponteiro que aponta pra ponteiro da estrutura estados chamado p_estados.
 * @param[in] - Ponteiro que aponta para outro ponteiro da estrutura transicoes chamado p_transicoes.
 * @param[in] - Variável ECT do tipo inteiro que irá armazenar a quantidade de estados que possuem tokens.
 */
void gerar_entrada(estados **p_estados, transicoes **p_transicoes)
{
    int ect; /*Estados com tokens.*/
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
    tadt *pt, *plant=NULL;
    while(pl!=NULL) /*Criando lista que ira ser relacionada com as threads.*/
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
    pt=*p_threads; /*colocando ponteiro auxiliar como cabeca da lista em questao.*/
    if(DEBUG)
    {
        printf("*****Lista de threads******.\n");
        while(pt!=NULL)
        {
            printf("thread da transicao %d.\n", pt->tr->ntr);
            pt=pt->prox;
        }
        pt=*p_threads; /*Colocando o ponteiro auxiliar como cabeca novamente caso o debug seja utilizado.*/
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
    while(cont<=ITERACOES) /*A quantidade de iteracoes pode ser modificada pela alteracao do define.*/
    {
        aux=1; /*Variavel que vai ser utilizada como forma de checar a possibilidade, ou nao, da ativacao da transicao.*/
        pa=pl->tr->entram;
        while(pa!=NULL)
        {
            pe=procurar_estado(pl->std, pa->origem);
            if(pa->custo>pe->nt)
                aux=0; /*A ativacao vai ser considerada sempre verdadeira, sendo considerada falsa apenas caso esteja faltando algum token.*/
            pa=pa->prox;
        }
        if(aux) /*Sera ativada apenas se todos os estados de origem da transicao estiverem com mais tokens que o necessario.*/
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

arcos *retirar_arco(arcos **p_arco) /*Esta funcao so pode ser utilizada caso se a intencao seja retirar a cabeca da lista.*/
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

