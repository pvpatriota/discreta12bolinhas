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
 * @ingroup MACROS 
 * @brief - Quantidade de INTERAÇÕES 
 */
#define ITERACOES 10000
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Define para ativar o DEBUG do código.
 */
#define DEBUG 0
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Este define foi utilizado para padronizar a saida do arquivo de imagem.
 */
#define IMAGENAME "ex12.bmp" /**Nome do arquivo que sera gerado pelo allegro*/
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Defines para facilitar o uso da cor branca no allegro.
 */
#define CORBRANCO (makecol(255,255,255))
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Defines para facilitar o uso da cor preta no allegro.
 */
#define CORPRETO 1
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Defines para facilitar o uso da cor cinza no allegro.
 */
#define CORCINZA (makecol(160,160,160))
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Defines para facilitar o uso da cor azul no allegro.
 */
#define CORAZUL (makecol(0, 0, 255))
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Defines para facilitar o uso da cor verde no allegro.
 */
#define CORVERDE (makecol(0, 255, 0))
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Defines para facilitar o uso da cor amarela no allegro.
 */
#define CORAMARELO (makecol(255,255,100))
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Defines para facilitar o uso da cor vermelha no allegro.
 */
#define CORVERMELHO (makecol(255, 0, 0))
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Defines que informam qual o tamanho da tela horizontalmente a serutilizada pelo allegro.
 */
#define X 640 /**Definicao do tamanho X da tela */
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Defines que informa qual o tamanho da tela verticalmente a ser utilizada pelo allegro.
 */
#define Y 480 /**Definicao do tamanho Y da tela */
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Defines que dizem qual é o centro da tela horizontal baseado no valor de X.
 */
#define XCentro X/2.0 /**Definicao do centro da tela da coordenada X*/
/* ---------------------------------------------------------------------- */
/**
 * @ingroup MACROS 
 * @brief - Defines que dizem qual é o centro da tela vertical baseado no valor de Y.
 */
#define YCentro Y/2.0 /**Definicao do centro da tela da coordenada Y*/

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Estruturas
 * @brief - Estruturas utilizadas para o correto funcionamento do programa.
 */

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Estruturas
 * @brief  - A estrutura sestados cujo o nome definido foi estados, irá armazenar as informacoes sobre os estados da rede de petri.
 * @param[in] - Ne recebe o número de estados.
 * @param[in] - Nt recebe o numero de tokens de cada estado.
 * @param[out] - O ponteiro prox aponta para o elemento seguinte da lista. Este ponteiro também é uma variável da estrutura sestados.
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
float lcos(float x1, float y1, float x2, float y2);
float lsin(float x1, float y1, float x2, float y2);

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Variaveis Globais
 * @brief - Variável EST, do tipo inteira e que recebe a quantidade de estados, declarada como global para receber os dados de entrada dos arquivos de texto e posteriormente ser utilizada para criar a imagem ex12.bmp com o allegro.
 */
static int est; /**Qtd de Estados */

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Variaveis Globais
 * @brief - Variável TR, do tipo inteira e que recebe a quantidade de transicoes, declara como global para receber os dados de entrada dos arquivos de texto e posteriormente ser utilizada para criar a imagem ex12.bmp com o allegro.
 */

static int tr; /**Quantidade de transicoes */
/* ---------------------------------------------------------------------- */
/**
 * @ingroup Variaveis Globais
 * @brief - Variável AET, do tipo inteira e que recebe a quantidade de arcos que vão dos estados para as transicoes, declarada como global para receber os dados de entrada dos arquivos de texto e posteriormente ser utilizada para criar a imagem ex12.bmp com o allegro.
 */

static int aet; /** Quantidade de arcos que vão dos estados para as transicoes */
/* ---------------------------------------------------------------------- */
/**
 * @ingroup Variaveis Globais
 * @brief - Variável ATE, do tipo inteira e que recebe a quantidade de arcos que vão das transicoes para os estados, declarada como global para receber os dados de entrada dos arquivos de texto e posteriormente ser utilizada para criar a imagem ex12.bmp com o allegro.
 */

static int ate; /**Quantidade de arcos vão das transicoes para os estados*/

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcao Main.
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao criar threads é responsável por criar as threads que iram executar o paralelismo do programa..
 * @param[in] - O ponteiro de ponteiro **p_threads, que irá criar as threads.
 * @param[in] - O ponteiro *p_transicoes leva para a funcao o valor da variavel cabeca_transicoes.
 * @param[in] - O ponteiro *p_estados, leva para a funcao criar_threads o valor da variável cabeça_estados.
 */

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
/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao espera_threads tem como finalidade esperar que as threads retornem o status de concluidas..
 * @param[in] - O ponteiro *p_threads, recebe o valor inical da lista de threads.
 */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao roda thread é a funcao responsável por rodar as threas que foram criadas na funcao criar_threads.
 * @param[in] - A variável cont é utilizada para definir um loop com o while.
 * @param[in] - A variável aux é utilizada para controle e esta irá definir a possibilidade ou não de uma transicao ser ativada.
 * @param[in] - O ponteiro *pa do tipo de estrutura arcos, irá receber a quantidade de transicoes que entram.
 * @param[in] - O ponteiro *pe do tipo de estrutura estados, irá receber os dados que a funcao procurar_estados irá lhe retornar.
 */

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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao criar_estados, tem como finalidade criar a quantidade de estados que foram lidos a partir da entrada dos arquivos de textos que foram disponibilizados.
 * @param[in] - O ponteiro de ponteiro p_estados, vai levar a lista de estados que serão criados com esta funcao.
 * @param[in] - A variável NUM, do tipo int, tem armazenada dentro de si a quantidade de estados que deveram ser criados nesta funcao e que serao organizados pela lista que utiliza o ponteiro p_estados.
 * @param[in] - A variável I, do tipo inteiro é utilizada apenas como uma variável de controle para a funcao FOR.
 */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao criar transicoes é a responsável por criar todas as transicoes necessárias para o correto funcionamento do programa.
 * @param[in] - O ponteiro de ponteiro p_transicoes recebe a lista transicoes que é enviada na chamada da funcao criar_transicoes dentro da funcao main.
 * @param[in] - O ponteiro de ponteiro p_arcos, vai receber a lista de arcos que também é enviada na chamada da funcao criar_transicoes dentro da funcao main.
 * @param[in] - A Variável A1, do tipo inteira, recebe a quantidade de arcos que vão de estados para as transicoes.
 * @param[in] - A Variável A2, do tipo inteira, recebe a quantidade de arcos que vão das transicoes para os estados.
 * @param[in] - A variável NUM, também do tipo inteira, irá receber a quantidade de transicoes que foram armazenadas na variável TR.
 * @param[in] - A variável AUX, do tipo inteiro, é utilizada para auxliar o funcionamento da funcao criar_transicoes.
 */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao gerar_imagem será a funcao responsavel por criar a imagem ex12.bmp.
 * @param[in] - O ponteiro do tipo de estrutura transicoes, traz a lista de transicoes para a funcao gerar_imagem, pois é nessa lista que se encontram as informacoes que precisamos para criar a imagem.
 * @param[in] - O ponteiro buff, do tipo de estrutura BITMAP, faz parte da biblioteca do allegro e este ponteiro é que irá realizar a criação da imagem.
 * @param[in] - O pal, é uma variável do tipo de estrutura PALETTE e esta estrutura é usada pela biblioteca allegro.h para poder criar a imagem.
 * @param[in] - O ponteiro pt, do tipo de estrutura arcos, receberá a quantidade de arcos que entram em uma respectiva transicao.
 * @param[in] - O ponteiro pl, do tipo de estrutura arcos, receberá a quantidade de arcos que saem de uma respectiva transicao.
 * @param[in] - A variável K, do tipo inteira, foi criada para auxiliar na funcao e está está sendo usada como um contador dentro da funcao FOR.
 * @param[in] - A Variável flag, também de tipo inteira, foi criada para garantir que o programa irá alterar entra a criação dos arcos que vão dos estados para as transicoes e dos arcos que vão das transicoes para o estados.
 */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A função desenha_estados, é a função responsável por pegar a quantidade de estados que foram lidos dos arquivos de textos fornecidos e através de um código matemático, distribuir os estados pelo tamanho da tela. Os estados possuem formato de cículo.
 * @param[out] - O ponteiro buff, do tipo BITMAP, é um ponteiro que se caracteriza como um sentido de saída, já que ele vai ser instanciado para criar um arquivo que será entregue e posteriormente visualizado pelo usuário..
 */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A função desenha_transicoes, tem como finalidade desenhar a quantidade de transicoes que foram armazenadas na variavel TR após ter sido lida pelos arquivos de texto e assim como a funcao desenha_estados, ao final retornará para a funcao gerar_imagem, onde será criada a imagem ex12.bmp com as transições.
 * @param[out] - O ponteiro buff, do tipo BITMAP, é um ponteiro que se caracteriza como um sentido de saída, já que ele vai ser instanciado para criar um arquivo que será entregue e posteriormente visualizado pelo usuário..
 * @param[in] - O ponteiro *p_transicoes do tipo de estrutura transicoes, recebe a lista transicoes, onde ele encontrará os respectivos valores para realizar o desenho das transicoes..
 */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao desenha_arcos tem como finalidade desenhar os arcos que saem dos estados e tem destino nas transicoes e desenhar os arcos que saem das transicoes e tem destino dos estados.
 * @param[in] - A variável qo, do tipo inteira, indica a origem do arco.
 * @param[in] - A variável qf, do tipo inteira, indica o destino do arco que teve origem em qo.
 * @param[in] - O ponteiro buff, do tipo BITMAP, irá criar a imagem.
 * @param[in] - A variável K, do tipo inteira, receberá a quantidade de estados multiplicado por dois, para poder realizar a alternação das transicoes de forma correta.
 * @param[in] - A varíável C, do tipo inteira, irá receber o custo de cada arco.
 * @param[in] - A variável flag, do intpo inteira também, será uma variável de auxilo, para fazer com que o programa alterne entra os arcos que saem dos estados e vão para as transicoes e dos arcos que saem das transicoes e vão para os estados.
 */
void desenha_arcos(int qo, int qf, BITMAP *buff, int k, int c, int flag)
{
    float si,co, alfa,beta, phi, x1, y1, x2, y2, x3, y3, xo, yo, xf, yf, raio, xt1, yt1, xt2, yt2, rc;
    raio=(Y/8)*(M_PI/(M_PI+(k/2)));
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
        if(flag)
        {
            xo = x1 + raio * cos(phi);
            yo = y1 + raio * sin(phi);
            xf = x3;
            yf = y3;
        }
        else
        {
            xo = x1;
            yo = y1;
            xf = x3 + raio * cos(beta);
            yf = y3 + raio * sin(beta);
        }
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

    si=lsin(x2,y2,xf,yf);
    co=lcos(x2,y2,xf,yf);
    xt1 = xf - (raio / 4) * (si + co);
    yt1 = yf + (raio / 4) * (co - si);
    xt2 = xf + (raio / 4) * (si - co);
    yt2 = yf - (raio / 4) * (si + co);
    triangle(buff, xf, yf, xt1, yt1, xt2, yt2, CORBRANCO);
    textprintf_ex(buff, font, x2, y2, CORVERDE, CORPRETO, "%d", c);

}

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao lcos é a responsavel por garantir o posicionamento correto dos arcos com os respectivos centros das transicoes e dos estados. Com essa funcao, os triangulos, antes originados de formas incorretas, agora passaram a ter os formatos corretos atraves dos calculos dos senos e cossenos.
 * @param[in] - A variavel X1 do tipo float, indica a posicação inicial do arco que sai do estado ou da transicao no eixo horizontal.
 * @param[in] - A variável Y1 do tipo float, indica a posicao inicial do arco que sai do estado ou da transico no eixo vertical.
 * @param[in] - A variável X2 do tipo float, indica a posicao final do arco que chega no estado ou transicao do eixo horizontal.
 * @param[in] - A variável Y2 do tipo float, indica a posicao final do arco que chega no estado ou transicao do eixo vertical.
 * @return O valor do centro.
 * */
float lcos(float x1, float y1, float x2, float y2)
{
    if(x1==0 && x2 == 0 && y1 == 0 && y2 == 0)
        return ~0;
    return ((x2-x1)/sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
}

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao lsin tem a mesma finalidade da funcao lcos, que é garantir o posicionamento corretos dos arcos com os respectivos centros das transicoes e dos estados. Corrigindo também os triangulos formados, devido aos calculos dos senos e cossenos.
 * @param[in] - A variavel X1 do tipo float, indica a posicação inicial do arco que sai do estado ou da transicao no eixo horizontal.
 * @param[in] - A variável Y1 do tipo float, indica a posicao inicial do arco que sai do estado ou da transico no eixo vertical.
 * @param[in] - A variável X2 do tipo float, indica a posicao final do arco que chega no estado ou transicao do eixo horizontal.
 * @param[in] - A variável Y2 do tipo float, indica a posicao final do arco que chega no estado ou transicao do eixo vertical.
 * @return O valor do centro.
 * */
float lsin(float x1, float y1, float x2, float y2)
{
    if(x1==0 && x2 == 0 && y1 == 0 && y2 == 0)
        return ~0;
    return ((y2-y1)/sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
}
/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao arctan é a responsavel por fazer as ligacoes corretas dos arcos para os estados ou transicoes, fazendo com que saiam dos centros dos estados e possam chegar nos centros das transicoes ou saindo dos centro das transicoes para chegarem ao centro dos estados.
 * @param[in] - A variavel X1 do tipo float, indica a posicação inicial do arco que sai do estado ou da transicao no eixo horizontal.
 * @param[in] - A variável Y1 do tipo float, indica a posicao inicial do arco que sai do estado ou da transico no eixo vertical.
 * @param[in] - A variável X2 do tipo float, indica a posicao final do arco que chega no estado ou transicao do eixo horizontal.
 * @param[in] - A variável Y2 do tipo float, indica a posicao final do arco que chega no estado ou transicao do eixo vertical.
 * @return Valor retornado a indicando qual quadrante se encontra.
 * */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao *retirar_arco é uma função que só é utilizada caso se tenha a intenção ou seja necessário remover um determinado arco.
 * @param[in] - O ponteiro de ponteiro p_arcos, da estrutura arcos, vai trazer a lista dos arcos para que possa ser procurado o que deseja e removido.
 * @return Ele retorna a lista com o arco desejado removido da mesma.
 * */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao transferir arcos tem como finalidade fazer a ligação correta entre os arcos e as transicoes..
 * @param[in] - O ponteiro de ponteiro p_transicao recebe a lista transicoes que é enviada na chamada da funcao.
 * @param[in] - O ponteiro de ponteiro p_arcos, vai receber a lista de arcos que também é enviada na chamada da funcao.
 * @param[in] - A Variável A1, do tipo inteira, recebe a quantidade de arcos que vão de estados para as transicoes.
 * @param[in] - A Variável A2, do tipo inteira, recebe a quantidade de arcos que vão das transicoes para os estados.
 */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao criar arcos é a responsável por criar todos os arcos necessárias para o correto funcionamento do programa.
 * @param[in] - O ponteiro de ponteiro p_arcos recebe a lista arcos que é enviada na chamada da funcao criar_arcos dentro da funcao main.
 * @param[in] - A Variável A1, do tipo inteira, recebe a quantidade de arcos que vão de estados para as transicoes.
 * @param[in] - A Variável A2, do tipo inteira, recebe a quantidade de arcos que vão das transicoes para os estados.
 * @param[in] - A variavel X, do tipo inteiro será usada para ler os dados da lista referente ao campo de origem do arco.
 * @param[in] - A variável Z, do tipo inteiro, será usada para ler os dados da lista referente ao campo destino do arco.
 * @param[in] - A variável Y, do tipo inteiro, será usada para ler os dados da lista referente ao campo custo do arco.
 * @param[in] - A variável AUX, do tipo inteiro, é utilizada para auxliar o funcionamento da funcao criar_arcos.
 */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao procurar estados vai procurar um determinado estado dentro da lista de estados.
 * @param[in] - O ponteiro p_estados recebe a lista estados que é enviada na chamada da funcao procurar_estados.
 * @param[in] - A Variável NUM, do tipo inteira, recebe a quantidade de estados existentes.
 * @return Retorna o estado desejado como posicao atual da lista.
 */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao procurar transicoes vai procurar uma determinada transicao dentro da lista de transicoes.
 * @param[in] - O ponteiro p_transicoes recebe a lista de transicoes que é enviada na chamada da funcao procurar_transicoes.
 * @param[in] - A Variável NUM, do tipo inteira, recebe a quantidade de transicoes existentes.
 * @return Retorna a transicao desejada como posicao atual da lista.
 */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao relacionar tokens tem como finalidade relacionar a quantidade de estados com os seus respectivos tokens, ou seja, essa funcao vai inicializar os estados, colocando a quantidade de tokens necessárias a partir da leitura dos arquivos de textos disponibilizados..
 * @param[in] - O ponteiro p_estados recebe a lista estados que é enviada na chamada da funcao procurar_estados.
 * @param[in] - A Variável NUM, do tipo inteira, recebe a quantidade de estados existentes que possuem tokens.
 * 
 */
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

/* ---------------------------------------------------------------------- */
/**
 * @ingroup Funcoes
 * @brief - A funcao DEBUG tem como finalidade mostrar um status atual do funcionamento do programa. Com isso é possível que o programador do mesmo saiba o que está acontecendo durante o funcionamento do programa e possa corrigir qualquer tipo de bugs que possam aparecer.
 * @param[in] - O ponteiro p_estados recebe a lista de estados completa que é enviada na chamada da função, onde será utilizada para mostrar as estatisticas dos estados.
 * @param[in] - O ponteiro p_transicoes recebe a lista de transicoes completa que é enviada na chamada da função, onde será utilizada para mostrar as estatísticas das transicoes.
 * @param[in] - O ponteiro pl, do tipo de estrutura estados, vai receber a quantidade de elementos da lista de estados.
 * @param[in] - O ponteiro plt, do tipo de estrutura transicoes, vai receber a quantidade de transicoes existentes.
 * @param[in] - O ponteiro pl, tanto do tipo estrutura de estados como do tipo estrutura de transicoes, sera usada para apontar imediatamente para o elemento anterior da lista.
 * @param[in] - O ponteiro pla, to tipo de estrutura arcos, vai ser utilizado para receber os valores dos arcos, onde entram, onde saem, a origem, o destino e o custo dos mesmos.
 * 
 */
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

