#ifndef ALGORITIMOS_
#define ALGORITIMOS_

#include "grafo.h"
#include "pilha.h"

/* Le tabela para compor o grafo */
void read_table(grafo_t *grafo, char *table);

/* Menor camingo entre todos os nos:
 * retorna um pilha do caminho entre fonte e destino  */
pilha_t* Dijkstra(grafo_t *grafo, vertice_t *fonte, vertice_t *destino);

/**
  * @brief  Busca em largura
  * @param	grafo: ponteiro do grafo que se deseja executar a busca
  * @param  inicial: ponteiro do vértice inicial (fonte) da busca
  *
  * @retval Nenhum: Vértices são marcados internamente
  */
void bfs(grafo_t *grafo, vertice_t* inicial);

/**
  * @brief  Busca em profundidade
  * @param	grafo: ponteiro do grafo que se deseja executar a busca
  * @param  inicial: ponteiro do vértice inicial (fonte) da busca
  *
  * @retval Nenhum: Vértices são marcados internamente
  */
void dfs(grafo_t *grafo, vertice_t* inicial);

grafo_t* prim_algorithm(grafo_t *grafo, int id);

#endif
