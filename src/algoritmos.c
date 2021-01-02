#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "fila.h"
#include "algoritimos.h"

#define N 1000
#define M 22

#define FALSE 0
#define TRUE 1
#define INFINIT -1

typedef struct priority_queue
{
  int queue_size;
  int queue_index;
  arestas_t **pri_queue;
} priority_queue_t;

void read_table(grafo_t *grafo, char *table)
{
    char buffer[N], temp_char[100], **city;
    int i, j, linha = 0, cod[M], *id, temp_int;
    float time[M];
    vertice_t *vertices;
    FILE *fp;

    fp = fopen(table, "r");

    if(!fp)
    {
        perror("Erro ao ler tabela");
        exit(EXIT_FAILURE);
    }

    while(fgets(buffer, N, fp) != NULL)
        linha++;

    city = malloc((linha-2)*sizeof(char*));
    if(city == NULL)
    {
        perror("Erro ao alocar cidades");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    id = malloc((linha-2)*sizeof(int));
    if(id == NULL)
    {
        perror("Erro ao alocar ids das cidades");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    rewind(fp);

    fgets(buffer, N, fp); // Ignorando primeira e segunda linha
    fgets(buffer, N, fp);

    for (i = 0; i < linha - 2; ++i)
    {
        fgets(buffer, N, fp);

        sscanf(buffer, "%d,%50[^,],", &id[i], temp_char);

        city[i] = malloc((strlen(temp_char)+1)*sizeof(char));
        if(!city[i])
        {
          perror("Erro ao alocar memória para o nome da cidade");
          fclose(fp);
          exit(EXIT_FAILURE);
        }
        strcpy(city[i], temp_char);

        #ifdef DEBUG
                printf("%d - %s\n", id[i], city[i]);
        #endif

        vertices = grafo_adicionar_vertice(grafo, id[i]);
        vertice_set_nome(vertices, city[i]);
    }
    rewind(fp);

    // Pegando as cidades das colunas
    fgets(buffer, N, fp);
    int ret = sscanf(buffer, "%50[^,],,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\
    %d,%d,%d,%d,%d,%d,%d,%d,",
    temp_char, &cod[0], &cod[1], &cod[2], &cod[3], &cod[4], &cod[5], &cod[6], &cod[7],
    &cod[8], &cod[9], &cod[10], &cod[11], &cod[12], &cod[13], &cod[14], &cod[15],
    &cod[16], &cod[17], &cod[18], &cod[19], &cod[20], &cod[21], &cod[22]);

    if (ret != 24)
    {
        fprintf(stderr, "Erro processando cidade %s - linha: %d.\n", __FILE__, __LINE__);
        exit(-1);
    }
    // Ignorando a segunda linha
    fgets(buffer, N, fp);

    for(i = 2; i < linha; i++)
    {
        fgets(buffer, N, fp);
        int ret = sscanf(buffer, "%d,%50[^,],%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,\
        %f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,", &temp_int, temp_char, &time[0], &time[1],
        &time[2], &time[3], &time[4], &time[5], &time[6], &time[7], &time[8],
        &time[9], &time[10], &time[11], &time[12], &time[13], &time[14], &time[15],
        &time[16], &time[17], &time[18], &time[19], &time[20], &time[21], &time[22]);

        if (ret != 25)
        {
            fprintf(stderr, "Erro processado cidade %s - linha: %d.\n", __FILE__, __LINE__);
            exit(-1);
        }

        for(j = 0; j < M; ++j)
        {
          vertices = procura_vertice(grafo, cod[j]);

          if(time[j]!=INFINIT)
          {
              adiciona_adjacentes(grafo, vertices, 2, id[i-2], time[j]);
          }
        }
    }
    free(id);
    free(city);
    fclose(fp);
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

    no = obter_cabeca(componentes_conexos(grafo));

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

    no = obter_cabeca(componentes_conexos(grafo));

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

void add_edges(vertice_t *vertice, priority_queue_t *pri_queue) {
    lista_enc_t *arestas = vertice_get_arestas(vertice);
    no_t *no;
    arestas_t *aresta;

    vertice_visitado(vertice, TRUE);

    no = obter_cabeca(arestas);
    while(no) {
      aresta = obter_dado(no);
      if(vertice_get_visit(aresta_get_adjacente(aresta)) == FALSE)
        pri_queue->pri_queue[pri_queue->queue_index++] = aresta;

      no = obtem_proximo(no);
    }
}

arestas_t* get_lightest(priority_queue_t *pri_queue) {
    arestas_t *aresta;
    int index_remov;

    aresta = pri_queue->pri_queue[0];
    for (int i = 1; i < pri_queue->queue_index; ++i) {
      if(pri_queue->pri_queue[i]) {
        if (aresta_get_peso(aresta) > aresta_get_peso(pri_queue->pri_queue[i]))
        {
          aresta = pri_queue->pri_queue[i];
          index_remov = i;
        }
      }
    }

    pri_queue->pri_queue[index_remov] = NULL;

    return aresta;
}

grafo_t* prim_algorithm(grafo_t* grafo, int id)
{
    int count_aresta = 0, m = numero_vertices(grafo) - 1;
    arestas_t *aresta;
    vertice_t *v, *destino;
    grafo_t *prims_graph;
    priority_queue_t queue;

    queue.queue_index = 0;
    queue.queue_size = numero_vertices(grafo)*numero_vertices(grafo)*sizeof(arestas_t*);
    queue.pri_queue = malloc(queue.queue_size);
    prims_graph = cria_grafo(id);

    v = procura_vertice(grafo, id);
    add_edges(v, &queue);

    while((queue.queue_index != 0) && (count_aresta != m))
    {
        aresta = get_lightest(&queue);
        destino = aresta_get_adjacente(aresta);

        if(vertice_get_visit(destino) == TRUE)
          continue;

        count_aresta++;
        adiciona_aresta_grafo(prims_graph, aresta);

        add_edges(destino, &queue);
    }

    free(queue.pri_queue);

    return prims_graph;
}
