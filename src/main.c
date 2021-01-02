#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"
#include "algoritimos.h"

int main()
{
    grafo_t *g, *spanning_tree;
    char table[] = "tempo.csv";

    g = cria_grafo(100);
    read_table(g, table);
    spanning_tree = prim_algorithm(g, 4205407);
    exportar_grafo_dot("grafo_prim's.txt", spanning_tree);
    libera_grafo(g);
    libera_grafo(spanning_tree);

    return 0;
}
