/*
 * grafo.c
 *
 *  Created on: Jul 5, 2016
 *      Author: Renan Augusto Starke
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "grafo.h"
#include "fila.h"

#define FALSE 0
#define TRUE 1

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

int numero_vertices(grafo_t *grafo) {
  return tamanho_lista(grafo->vertices);
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

void adiciona_aresta_grafo(grafo_t *grafo, arestas_t *aresta) {
    vertice_t *fonte, *destino;
    int id_fonte, id_destino;

    id_fonte = vertice_get_id(aresta_get_fonte(aresta));
    fonte = procura_vertice(grafo, id_fonte);
    if (fonte == NULL) {
      fonte = grafo_adicionar_vertice(grafo, id_fonte);
      vertice_visitado(fonte, TRUE);
    }

    id_destino = vertice_get_id(aresta_get_adjacente(aresta));
    destino = procura_vertice(grafo, id_destino);
    if (destino == NULL) {
      destino = grafo_adicionar_vertice(grafo, id_destino);
      vertice_visitado(destino, FALSE);
    }

    vertice_set_nome(fonte, vertice_get_nome(aresta_get_fonte(aresta)));
    vertice_set_nome(destino, vertice_get_nome(aresta_get_adjacente(aresta)));

    adiciona_aresta(fonte, aresta);

#ifdef DEBUG
    printf("\tfonte: %d\n", id_fonte);
    printf("\tdestino: %d\n", id_destino);
    printf("\tpeso: %f\n", aresta_get_peso(aresta));
#endif
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
    //arestas_t *contra_aresta;
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
            //contra_aresta = procurar_adjacente(adjacente, vertice);
            //if (contra_aresta)
            //  aresta_set_status(contra_aresta, EXPORTADA);

            //obtem peso
            peso = aresta_get_peso(aresta);

            fprintf(file, "\t%s -- %s [label = %f];\n",
                    vertice_get_nome(vertice),
                    vertice_get_nome(adjacente),
                    peso);

            no_arest = obtem_proximo(no_arest);
        }
        no_vert = obtem_proximo(no_vert);
    }
    fprintf(file, "}\n");
    fclose(file);
}

lista_enc_t* componentes_conexos(grafo_t *grafo)
{
  return grafo->vertices;
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
        if(vertice_get_nome(vertice))
          free(vertice_get_nome(vertice));
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
