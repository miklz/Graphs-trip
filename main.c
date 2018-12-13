#include <stdio.h>
#include <stdlib.h>
#include "grafo/grafo.h"

int main()
{
    grafo_t *g;
    char table[] = "tempo.txt";
    lista_enc_t *spanning_tree;

    g = cria_grafo(100);
    read_table(g, table);
    spanning_tree = prim_algorithm(g, 4205407);
    exportar_grafo_prim(spanning_tree);
    //exportar_grafo_dot("grafo_table.txt", g);

    return 0;
}
