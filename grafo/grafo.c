/*
 * grafo.c
 *
 *  Created on: Jul 5, 2016
 *      Author: Renan Augusto Starke
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include "grafo.h"
#include "vertice.h"
#include "../lista_enc/lista_enc.h"
#include "../lista_enc/no.h"
#include "../fila/fila.h"

#include "../pilha/pilha.h"

#define FALSE 0
#define TRUE 1
#define INFINIT -1
#define N 1000
#define M 24

#define INFINITO INT_MAX

struct grafos
{
    int id;                    /*!< Identificação numérica do grafo  */
    lista_enc_t *vertices;     /*!< Lista encadeada dos vértices: conjunto V  */
};

/**
  * @brief  Cria uma novo grafo
  * @param	id: Identificação numérica do grafo
  *
  * @retval grafo_t: ponteiro para um novo grafo
  */
grafo_t *cria_grafo(int id)
{
    grafo_t *p = NULL;

    p = (grafo_t*) malloc(sizeof(grafo_t));

    if (p == NULL)
    {
        perror("cria_grafo:");
        exit(EXIT_FAILURE);
    }

    p->id = id;
    p->vertices = cria_lista_enc();

    return p;
}

/**
  * @brief  Adicionar um vértice no grafo (conjunto V)
  * @param	grafo: ponteiro do grafo que se deseja adicionar um vértice
  * @param  id: identificação da vértice
  *
  * @retval vertice_t: ponteiro do vértice criado e adicionada no grafo
  */
vertice_t* grafo_adicionar_vertice(grafo_t *grafo, int id)
{
    vertice_t *vertice;
    no_t *no;

#ifdef DEBUG
    printf("grafo_adicionar_vertice: %d\n", id);
#endif

    if (grafo == NULL)
    {
        fprintf(stderr,"grafo_adicionar_vertice: grafo invalido!");
        exit(EXIT_FAILURE);
    }

    if (procura_vertice(grafo, id) != NULL)
    {
        fprintf(stderr,"grafo_adicionar_vertice: vertice duplicado!\n");
        exit(EXIT_FAILURE);
    }

    vertice = cria_vertice(id);
    no = cria_no(vertice);

    add_cauda(grafo->vertices, no);

    return vertice;
}

/**
  * @brief  Procura um vértice com id específico no grafo
  * @param	grafo: ponteiro do grafo que se deseja busca o vértice
  * @param  id: identificação da aresta
  *
  * @retval vertice_t: ponteiro do vértice. NULL se não encontrado
  */
vertice_t* procura_vertice(grafo_t *grafo, int id)
{
    no_t *no_lista;
    vertice_t *vertice;
    int meu_id;

    if (grafo == NULL)
    {
        fprintf(stderr,"procura_vertice: grafo invalido!");
        exit(EXIT_FAILURE);
    }

    if (lista_vazia(grafo->vertices) == TRUE)
        return FALSE;

    no_lista = obter_cabeca(grafo->vertices);

    while (no_lista)
    {
        //obtem o endereco da lista
        vertice = obter_dado(no_lista);

        //obterm o id do vertice
        meu_id = vertice_get_id(vertice);

        if (meu_id == id)
        {
            return vertice;
        }

        no_lista = obtem_proximo(no_lista);
    }

    return NULL;
}

/**
  * @brief  Cria adjacências.
  * @param	grafo: ponteiro do grafo que contém o vértice (V pertence a G)
  * @param  vertice: vértice fonte da(s) adjacências
  * @param  n: número de parâmetros após n
  * @param  ...: pares ordenados dos vertices destino e peso da aresta: (id vertice destino, peso aresta)
  *
  * @retval Nenhum
  *
  * Ex: adicionar uma aresta para o vertice 2 e 3 com respectivos pesos 9 e 15
  * adiciona_adjacentes(grafo, vertice, 4(n), 2, 9, 3, 15);
  */

void adiciona_adjacentes(grafo_t *grafo, vertice_t *vertice, int n, ...)
{
    va_list argumentos;
    vertice_t *sucessor;
    arestas_t *aresta, *contra_aresta;

    int id_sucessor;
    double peso;
    int x;

    /* Initializing arguments to store all values after num */
    va_start (argumentos, n);

    for (x = 0; x < n; x=x+2 )
    {
        id_sucessor = va_arg(argumentos, int);
        peso = va_arg(argumentos, double);

        sucessor = procura_vertice(grafo, id_sucessor);

        if (sucessor == NULL)
        {
            fprintf(stderr, "adiciona_adjacentes: sucessor nao encontrado no grafo\n");
            exit(EXIT_FAILURE);
        }

        aresta = cria_aresta(vertice, sucessor,peso);
        contra_aresta = cria_aresta(sucessor, vertice, peso);
        adiciona_aresta(vertice, aresta);
        adiciona_aresta(sucessor, contra_aresta);

#ifdef DEBUG
        printf("\tvertice: %d\n", vertice_get_id(vertice));
        printf("\tsucessor: %d\n", id_sucessor);
        printf("\tpeso: %f\n", peso);
#endif

    }

    va_end (argumentos);
}

void read_table(grafo_t *grafo, char *table)
{
    char buffer[N], *city[N];
    int i, j, cod, linha = 0, id[M];
    float *time;
    vertice_t *vertices;
    FILE *fp;

    fp = fopen(table, "r");

    if(!fp)
    {
        perror("Erro ao ler tabela");
        exit(-1);
    }

    for (i=0; i < N; i++)
    {
        city[i] = malloc(sizeof(char)*32);
        if(!city[i])
        {
            perror("Erro ao alocar cidades");
            fclose(fp);
            exit(-1);
        }
    }

    time = malloc(sizeof(float)*M);
    if(!time)
    {
        perror("Erro ao alocar time");
        fclose(fp);
        exit(-1);
    }

    while(fgets(buffer, N, fp)!=NULL)
        linha++;
    rewind(fp);

    fgets(buffer, N, fp);
    sscanf(buffer, "%50[^,], %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d,", city[0], &id[0], &id[1], &id[2], &id[3], &id[4], &id[5], &id[6], &id[7], &id[8], &id[9], &id[10], &id[11], &id[12], &id[13], &id[14], &id[15], &id[16], &id[17],&id[18], &id[19], &id[20], &id[21], &id[22]);
    for(i = 0; i < M-1; i++)
    {
        vertices = grafo_adicionar_vertice(grafo, id[i]);

#ifdef DEBUG
        printf("id: %d \n", id[i]);
#endif

    }

    fgets(buffer, N, fp);
    int ret = sscanf(buffer, "%50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,], %50[^,],", city[0], city[1], city[2], city[3], city[4], city[5], city[6], city[7], city[8], city[9], city[10], city[11], city[12], city[13], city[14], city[15], city[16], city[17], city[18], city[19], city[20], city[21], city[22],city[23]);

    if (ret != 24)
    {
        fprintf(stderr, "Erro processado cidade %s - linha: %d.\n", __FILE__, __LINE__);
        exit(-1);
    }

    for(i = 1; i < M; i++)
    {
        vertices = procura_vertice(grafo, id[i-1]);
        vertice_set_nome(vertices, city[i]);

#ifdef DEBUG
        printf("%s\n", city[i]);
#endif // DEBUG
    }

    for(i = 2; i < linha; i++)
    {
        fgets(buffer, N, fp);
        int ret = sscanf(buffer, "%d, %50[^,], %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f,", &cod, city[22+i], &time[0], &time[1], &time[2], &time[3], &time[4], &time[5], &time[6], &time[7], &time[8], &time[9], &time[10], &time[11], &time[12], &time[13], &time[14], &time[15], &time[16], &time[17], &time[18], &time[19], &time[20], &time[21], &time[22]);

        if (ret != 25)
        {
            fprintf(stderr, "Erro processado cidade %s - linha: %d.\n", __FILE__, __LINE__);
            exit(-1);
        }

        vertices = procura_vertice(grafo, cod);
        if(!vertices)
        {
            vertices = grafo_adicionar_vertice(grafo, cod);
            vertice_set_nome(vertices, city[22+i]);
#ifdef DEBUG
            printf("%d - %s\n", cod, city[0]);
#endif // DEBUG
        }
        for(j = 0; j < M-1; j++)
        {
            if(time[j]!=INFINIT)
            {
                adiciona_adjacentes(grafo, vertices, 2, id[j], time[j]);
            }
        }
    }
}

vertice_t *buscar_vertice(lista_enc_t *lista, int id)
{
    no_t* no;
    vertice_t *vertice;

    no = obter_cabeca(lista);

    while(no)
    {
        vertice = obter_dado(no);
        if(vertice_get_id(vertice) == id)
            return vertice;
        no = obtem_proximo(no);
    }
    return 0;
}

/**
  * @brief  Busca em largura
  * @param	grafo: ponteiro do grafo que se deseja executar a busca
  * @param  inicial: ponteiro do vértice inicial (fonte) da busca
  *
  * @retval Nenhum: Vértices são marcados internamente
  */
void bfs(grafo_t *grafo, vertice_t* inicial)
{
    no_t *no;
    fila_t* fila;
    arestas_t *aresta;
    vertice_t *u, *v;
    lista_enc_t * lista_aresta;

    fila = cria_fila();

    no = obter_cabeca(grafo->vertices);

    while(no)
    {
        v = obter_dado(no);
        vertice_set_pai(v,NULL);
        vertice_set_dist(v, -1);
        no = obtem_proximo(no);
    }

    vertice_set_dist(inicial, 0);
    enqueue(inicial,fila);

    while(!fila_vazia(fila))
    {
        v = dequeue(fila);
        lista_aresta = vertice_get_arestas(v);
        no = obter_cabeca(lista_aresta);

        while(no)
        {
            aresta = obter_dado(no);
            u = aresta_get_adjacente(aresta);
            if(vertice_get_dist(u)!=-1)
            {
                enqueue(u, fila);
                vertice_set_pai(u,v);
                vertice_set_dist(u,vertice_get_dist(v)+1);
            }
            no = obtem_proximo(no);
        }
    }
}

/**
  * @brief  Busca em profundidade
  * @param	grafo: ponteiro do grafo que se deseja executar a busca
  * @param  inicial: ponteiro do vértice inicial (fonte) da busca
  *
  * @retval Nenhum: Vértices são marcados internamente
  */
void dfs(grafo_t *grafo, vertice_t* inicial)
{
    no_t *no;
    pilha_t* pilha;
    arestas_t *aresta;
    vertice_t *u, *v;
    lista_enc_t * lista_aresta;

    pilha = cria_pilha();

    no = obter_cabeca(grafo->vertices);

    while(no)
    {
        v = obter_dado(no);
        vertice_visitado(v, FALSE);
        no = obtem_proximo(no);
    }

    push(inicial, pilha);

    while(!pilha_vazia(pilha))
    {
        u = pop(pilha);
        lista_aresta = vertice_get_arestas(u);
        no = obter_cabeca(lista_aresta);

        while(no)
        {
            aresta = obter_dado(no);
            v = aresta_get_adjacente(aresta);
            if(!vertice_get_visit(v))
            {
                vertice_visitado(v,TRUE);
                push(v, pilha);
            }
        }
    }
}

lista_enc_t* prim_algorithm(grafo_t* grafo, int id)
{

    no_t *no, *no_prim, *no_aux;
    arestas_t *aresta, *aresta_aux;
    vertice_t *u, *v;
    lista_enc_t *prim_arestas;

    prim_arestas = cria_lista_enc();
    no = obter_cabeca(grafo->vertices);

    while(no)
    {
        v = obter_dado(no);
        vertice_set_dist(v, INFINIT);
        vertice_set_pai(v, NULL);
        if(vertice_get_id(v)==id)
            vertice_set_dist(v, FALSE);
        no = obtem_proximo(no);
    }

    no = obter_cabeca(grafo->vertices);
    u = procura_vertice(grafo, id);
    while(no)
    {
        no_aux = obter_cabeca(vertice_get_arestas(u));
        while(no_aux)
        {
            aresta = obter_dado(no_aux);
            v = aresta_get_adjacente(aresta);
            if(vertice_get_dist(v)==INFINIT)
            {
                vertice_set_pai(v,u);
                vertice_set_dist(v,aresta_get_peso(aresta));
                no_prim = cria_no(aresta);
                add_cauda(prim_arestas, no_prim);
            }
            else
            {
                if(vertice_get_dist(v) > aresta_get_peso(aresta))
                {
                    aresta_aux = procurar_adjacente(vertice_get_pai(v), v);
                    no_prim = cria_no(aresta_aux);
                    remover_no(prim_arestas, no_prim);
                    vertice_set_pai(v,u);
                    vertice_set_dist(v,aresta_get_peso(aresta));
                    no_prim = cria_no(aresta);
                    add_cauda(prim_arestas, no_prim);
                }
            }

            no_aux = obtem_proximo(no_aux);
        }
        no = obtem_proximo(no);
        if(no)
        {
            u = obter_dado(no);
            if(vertice_get_id(u)==id)
            {
                no = obtem_proximo(no);
                u = obter_dado(no);
            }
            printf("proximo %s->%d\n", vertice_get_nome(u),vertice_get_id(u));
        }
    }

    return prim_arestas;
}


void exportar_grafo_prim(lista_enc_t *spanning_tree)
{
    FILE *fp;

    no_t *no;
    arestas_t *aresta;
    vertice_t *vertice, *pai;

    fp = fopen("grafo_prim's.txt", "w");

    if (fp == NULL || spanning_tree == NULL)
    {
        fprintf(stderr, "exportar_grafp_dot: ponteiros invalidos\n");
        exit(EXIT_FAILURE);
    }

    no = obter_cabeca(spanning_tree);
    while(no)
    {
        aresta = obter_dado(no);
        vertice = aresta_get_adjacente(aresta);
        pai = aresta_get_fonte(aresta);
        printf("\t\"%s\"\t -> \t\"%s\" \t[ label = \"%.2f\" ];\n", vertice_get_nome(pai), vertice_get_nome(vertice), aresta_get_peso(aresta));
        fprintf(fp,"\t\"%s\"\t -> \t\"%s\" \t[ label = \"%.2f\" ];\n", vertice_get_nome(pai), vertice_get_nome(vertice), aresta_get_peso(aresta));

        no = obtem_proximo(no);
    }

    fclose(fp);

}
/**
  * @brief  Exporta o grafo em formato dot.
  * @param	filename: nome do arquivo dot gerado
  * @param  grafo: ponteiro do grafo a ser exportado
  *
  * @retval Nenhum
  */
void exportar_grafo_dot(const char *filename, grafo_t *grafo)
{
    FILE *file;

    no_t *no_vert;
    no_t *no_arest;
    vertice_t *vertice;
    vertice_t *adjacente;
    arestas_t *aresta;
    arestas_t *contra_aresta;
    lista_enc_t *lista_arestas;

    float peso;

    if (filename == NULL || grafo == NULL)
    {
        fprintf(stderr, "exportar_grafp_dot: ponteiros invalidos\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(filename, "w");

    if (file == NULL)
    {
        perror("exportar_grafp_dot:");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "graph {\n");

    //obtem todos os nos da lista
    no_vert = obter_cabeca(grafo->vertices);
    while (no_vert)
    {
        vertice = obter_dado(no_vert);

        //obtem todos as arestas
        lista_arestas = vertice_get_arestas(vertice);

        no_arest = obter_cabeca(lista_arestas);
        while (no_arest)
        {
            aresta = obter_dado(no_arest);

            //ignora caso já exportada
            if (aresta_get_status(aresta) == EXPORTADA)
            {
                no_arest = obtem_proximo(no_arest);
                continue;
            }

            //marca como exportada esta aresta
            aresta_set_status(aresta, EXPORTADA);
            adjacente = aresta_get_adjacente(aresta);


            //marca contra-aresta também como exporta no caso de grafo não direcionados
            contra_aresta = procurar_adjacente(adjacente, vertice);
            aresta_set_status(contra_aresta, EXPORTADA);

            //obtem peso
            peso = aresta_get_peso(aresta);

            fprintf(file, "\t%d -- %d [label = %f];\n",
                    vertice_get_id(vertice),
                    vertice_get_id(adjacente),
                    peso);

            no_arest = obtem_proximo(no_arest);
        }
        no_vert = obtem_proximo(no_vert);
    }
    fprintf(file, "}\n");
    fclose(file);
}

/**
  * @brief  Libera a memória utilizada pelo grafo
  * @param  grafo: ponteiro do grafo a ser exportado
  *
  * @retval Nenhum
  */
void libera_grafo (grafo_t *grafo)
{
    no_t *no_vert;
    no_t *no_arest;
    no_t *no_liberado;
    vertice_t *vertice;
    arestas_t *aresta;
    lista_enc_t *lista_arestas;

    if (grafo == NULL)
    {
        fprintf(stderr, "libera_grafo: grafo invalido\n");
        exit(EXIT_FAILURE);
    }

    //varre todos os vertices
    no_vert = obter_cabeca(grafo->vertices);
    while (no_vert)
    {
        vertice = obter_dado(no_vert);

        //libera todas as arestas
        lista_arestas = vertice_get_arestas(vertice);
        no_arest = obter_cabeca(lista_arestas);
        while (no_arest)
        {
            aresta = obter_dado(no_arest);

            //libera aresta
            free(aresta);

            //libera no da lsita
            no_liberado = no_arest;
            no_arest = obtem_proximo(no_arest);
            free(no_liberado);
        }

        //libera lista de arestas e vertice
        free(lista_arestas);
        free(vertice);

        //libera no da lista
        no_liberado = no_vert;
        no_vert = obtem_proximo(no_vert);
        free(no_liberado);
    }

    //libera grafo e vertice
    free(grafo->vertices);
    free(grafo);
}
