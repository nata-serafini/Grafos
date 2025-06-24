#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "grafo.h"



lista_str *nova_lista() {
    lista_str *l = malloc(sizeof(lista_str));
    l->tamanho = 0;
    l->capacidade = 10;
    l->dados = malloc(l->capacidade * sizeof(char *));
    return l;
}

void adiciona_lista(lista_str *l, const char *s) {
    if (l->tamanho == l->capacidade) {
        l->capacidade *= 2;
        l->dados = realloc(l->dados, l->capacidade * sizeof(char *));
    }
    l->dados[l->tamanho++] = strdup(s);
}

char *junta_lista(lista_str *l) {
    int tam = 1;
    for (int i = 0; i < l->tamanho; i++) tam += strlen(l->dados[i]) + 1;
    char *r = malloc(tam);
    r[0] = '\0';
    for (int i = 0; i < l->tamanho; i++) {
        strcat(r, l->dados[i]);
        if (i < l->tamanho - 1) strcat(r, " ");
    }
    return r;
}

void libera_lista(lista_str *l) {
    for (int i = 0; i < l->tamanho; i++) free(l->dados[i]);
    free(l->dados);
    free(l);
}

// Funções auxiliares
vert *busca_vertice(grafo *g, const char *nome) {
    for (vert *v = g->vertices; v; v = v->prox)
        if (strcmp(v->nome, nome) == 0)
            return v;
    return NULL;
}

vert *adiciona_vertice(grafo *g, const char *nome) {
    vert *v = busca_vertice(g, nome);
    if (v) return v;

    v = malloc(sizeof(vert));
    v->nome = strdup(nome);
    v->vizinhos = NULL;
    v->visitado = 0;
    v->cor = -1;
    v->indice = -1;
    v->lowlink = -1;
    v->tempo = -1;
    v->prox = g->vertices;
    g->vertices = v;
    return v;
}

void adiciona_aresta(grafo *g, vert *v1, vert *v2, int peso) {
    viz *nova1 = malloc(sizeof(viz));
    nova1->vertice = v2;
    nova1->peso = peso;
    nova1->prox = v1->vizinhos;
    v1->vizinhos = nova1;

    viz *nova2 = malloc(sizeof(viz));
    nova2->vertice = v1;
    nova2->peso = peso;
    nova2->prox = v2->vizinhos;
    v2->vizinhos = nova2;
}

grafo *le_grafo(FILE *f) {
    char linha[200];

    // Ler o nome do grafo, pulando linhas vazias e comentários
    do {
        if (!fgets(linha, sizeof(linha), f))
            return NULL;
        linha[strcspn(linha, "\n")] = '\0';
    } while (strlen(linha) == 0 || strncmp(linha, "//", 2) == 0);

    grafo *g = malloc(sizeof(grafo));
    g->nome = strdup(linha);
    g->vertices = NULL;

    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = '\0';

        // Ignorar linhas vazias e comentários
        if (strlen(linha) == 0 || strncmp(linha, "//", 2) == 0)
            continue;

        char v1[100], v2[100];
        int peso;

        // Aresta com peso
        if (sscanf(linha, "%s -- %s %d", v1, v2, &peso) == 3) {
            vert *u = busca_vertice(g, v1);
            if (!u) u = adiciona_vertice(g, v1);

            vert *v = busca_vertice(g, v2);
            if (!v) v = adiciona_vertice(g, v2);

            adiciona_aresta(g, u, v, peso);

        // Aresta sem peso (peso = 1)
        } else if (sscanf(linha, "%s -- %s", v1, v2) == 2) {
            vert *u = busca_vertice(g, v1);
            if (!u) u = adiciona_vertice(g, v1);

            vert *v = busca_vertice(g, v2);
            if (!v) v = adiciona_vertice(g, v2);

            adiciona_aresta(g, u, v, 1);

        // Vértice isolado
        } else {
            if (!busca_vertice(g, linha)) {
                adiciona_vertice(g, linha);
            }
        }
    }

    return g;
}

unsigned int destroi_grafo(grafo *g) {
    vert *v = g->vertices;
    while (v) {
        viz *w = v->vizinhos;
        while (w) {
            viz *temp = w;
            w = w->prox;
            free(temp);
        }
        vert *tempv = v;
        v = v->prox;
        free(tempv->nome);
        free(tempv);
    }
    free(g->nome);
    free(g);
    return 0;
}

char *nome(grafo *g) {
    return g->nome;
}

unsigned int n_vertices(grafo *g) {
    unsigned int count = 0;
    for (vert *v = g->vertices; v; v = v->prox)
        count++;
    return count;
}

unsigned int n_arestas(grafo *g) {
    unsigned int count = 0;
    for (vert *v = g->vertices; v; v = v->prox)
        for (viz *w = v->vizinhos; w; w = w->prox)
            count++;
    return count / 2;
}

void marca_componente(vert *v) {
    if (v->visitado) return;
    v->visitado = 1;
    for (viz *w = v->vizinhos; w; w = w->prox)
        marca_componente(w->vertice);
}

unsigned int n_componentes(grafo *g) {
    for (vert *v = g->vertices; v; v = v->prox)
        v->visitado = 0;

    unsigned int comp = 0;
    for (vert *v = g->vertices; v; v = v->prox) {
        if (!v->visitado) {
            marca_componente(v);
            comp++;
        }
    }
    return comp;
}

int dfs_bipartido(vert *v, int cor) {
    v->visitado = 1;
    v->cor = cor;
    for (viz *w = v->vizinhos; w; w = w->prox) {
        if (!w->vertice->visitado) {
            if (!dfs_bipartido(w->vertice, 1 - cor)) return 0;
        } else if (w->vertice->cor == v->cor) {
            return 0;
        }
    }
    return 1;
}

unsigned int bipartido(grafo *g) {
    for (vert *v = g->vertices; v; v = v->prox) {
        v->visitado = 0;
        v->cor = -1;
    }

    for (vert *v = g->vertices; v; v = v->prox) {
        if (!v->visitado) {
            if (!dfs_bipartido(v, 0)) return 0;
        }
    }
    return 1;
}

int dfs_dist(vert *origem, vert *atual, int *dist, int d) {
    if (dist[atual->tempo] <= d) return 0;
    dist[atual->tempo] = d;
    int max = d;
    for (viz *v = atual->vizinhos; v; v = v->prox) {
        max = (dfs_dist(origem, v->vertice, dist, d + v->peso) > max) ? d + v->peso : max;
    }
    return max;
}


int dijkstra_dist(vert *origem, vert **componente, int tam_comp, int *idmap) {
    int *dist = malloc(tam_comp * sizeof(int));
    int *visitado = calloc(tam_comp, sizeof(int));
    for (int i = 0; i < tam_comp; i++) dist[i] = INF;

    int id_orig = idmap[origem->tempo];
    dist[id_orig] = 0;

    for (int count = 0; count < tam_comp; count++) {
        int u = -1;
        for (int i = 0; i < tam_comp; i++) {
            if (!visitado[i] && (u == -1 || dist[i] < dist[u])) {
                u = i;
            }
        }
        if (u == -1 || dist[u] == INF) break;
        visitado[u] = 1;

        for (viz *v = componente[u]->vizinhos; v; v = v->prox) {
            int j = idmap[v->vertice->tempo];
            if (dist[j] > dist[u] + v->peso) {
                dist[j] = dist[u] + v->peso;
            }
        }
    }

    int max = 0;
    for (int i = 0; i < tam_comp; i++) {
        if (dist[i] < INF && dist[i] > max) {
            max = dist[i];
        }
    }

    free(dist);
    free(visitado);
    return max;
}

char *diametros(grafo *g) {
    for (vert *v = g->vertices; v; v = v->prox)
        v->visitado = 0;

    lista_str *resultado = nova_lista();

    for (vert *v = g->vertices; v; v = v->prox) {
        if (v->visitado) continue;

        // Obter todos os vértices do componente usando DFS
        vert **componente = malloc(sizeof(vert*) * 1000);
        int n = 0;

        void dfs(vert *u) {
            if (u->visitado) return;
            u->visitado = 1;
            componente[n++] = u;
            for (viz *w = u->vizinhos; w; w = w->prox)
                dfs(w->vertice);
        }

        dfs(v);

        if (n == 1) {
            adiciona_lista(resultado, "0");
            free(componente);
            continue;
        }

        // Calcular diâmetro com Dijkstra (sem usar heap, pois n é pequeno)
        int max_dist = 0;
        for (int i = 0; i < n; i++) {
            int dist[1000];
            int visitado[1000] = {0};

            for (int j = 0; j < n; j++) dist[j] = INF;
            dist[i] = 0;

            for (int k = 0; k < n; k++) {
                int u = -1;
                for (int j = 0; j < n; j++)
                    if (!visitado[j] && (u == -1 || dist[j] < dist[u]))
                        u = j;

                if (u == -1 || dist[u] == INF) break;
                visitado[u] = 1;

                for (viz *w = componente[u]->vizinhos; w; w = w->prox) {
                    for (int j = 0; j < n; j++) {
                        if (componente[j] == w->vertice && dist[j] > dist[u] + w->peso)
                            dist[j] = dist[u] + w->peso;
                    }
                }
            }

            for (int j = 0; j < n; j++)
                if (dist[j] < INF && dist[j] > max_dist)
                    max_dist = dist[j];
        }

        char buf[32];
        sprintf(buf, "%d", max_dist);
        adiciona_lista(resultado, buf);
        free(componente);
    }

    // Ordenar numericamente os diâmetros
    for (int i = 0; i < resultado->tamanho - 1; i++) {
        for (int j = i + 1; j < resultado->tamanho; j++) {
            if (atoi(resultado->dados[i]) > atoi(resultado->dados[j])) {
                char *tmp = resultado->dados[i];
                resultado->dados[i] = resultado->dados[j];
                resultado->dados[j] = tmp;
            }
        }
    }

    char *res = junta_lista(resultado);
    libera_lista(resultado);
    return res;
}

int tempo_global, raiz_children;

void dfs_articulacao(vert *v, vert *pai, lista_str *cortes, lista_str *pontes) {
    v->visitado = 1;
    v->indice = v->lowlink = tempo_global++;
    for (viz *w = v->vizinhos; w; w = w->prox) {
        vert *u = w->vertice;
        if (!u->visitado) {
            if (pai == NULL) raiz_children++;
            dfs_articulacao(u, v, cortes, pontes);
            if (u->lowlink >= v->indice && pai != NULL) adiciona_lista(cortes, v->nome);
            if (u->lowlink > v->indice) {
                char buf[200];
                snprintf(buf, sizeof(buf), "%s-%s", v->nome, u->nome);
                adiciona_lista(pontes, buf);
            }
            if (u->lowlink < v->lowlink) v->lowlink = u->lowlink;
        } else if (u != pai && u->indice < v->lowlink) {
            v->lowlink = u->indice;
        }
    }
}

char *vertices_corte(grafo *g) {
    for (vert *v = g->vertices; v; v = v->prox) {
        v->visitado = 0;
        v->indice = -1;
        v->lowlink = -1;
    }

    int tempo = 0;
    lista_str *cortes = nova_lista();

    void dfs(vert *v, vert *pai) {
        v->visitado = 1;
        v->indice = v->lowlink = tempo++;
        int filhos = 0;
        int ponto_articulacao = 0;

        for (viz *w = v->vizinhos; w; w = w->prox) {
            vert *u = w->vertice;
            if (!u->visitado) {
                filhos++;
                dfs(u, v);
                if (u->lowlink >= v->indice)
                    ponto_articulacao = 1;
                if (u->lowlink < v->lowlink)
                    v->lowlink = u->lowlink;
            } else if (u != pai && u->indice < v->lowlink) {
                v->lowlink = u->indice;
            }
        }

        if ((pai == NULL && filhos > 1) || (pai != NULL && ponto_articulacao)) {
            adiciona_lista(cortes, v->nome);
        }
    }

    for (vert *v = g->vertices; v; v = v->prox) {
        if (!v->visitado)
            dfs(v, NULL);
    }

    // Ordenar alfabeticamente
    for (int i = 0; i < cortes->tamanho - 1; i++) {
        for (int j = i + 1; j < cortes->tamanho; j++) {
            if (strcmp(cortes->dados[i], cortes->dados[j]) > 0) {
                char *tmp = cortes->dados[i];
                cortes->dados[i] = cortes->dados[j];
                cortes->dados[j] = tmp;
            }
        }
    }

    char *res = junta_lista(cortes);
    libera_lista(cortes);
    return res;

}

char *arestas_corte(grafo *g) {
    for (vert *v = g->vertices; v; v = v->prox) {
        v->visitado = 0;
        v->indice = -1;
        v->lowlink = -1;
    }

    int tempo = 0;
    lista_str *pontes = nova_lista();

    void dfs(vert *v, vert *pai) {
        v->visitado = 1;
        v->indice = v->lowlink = tempo++;

        for (viz *w = v->vizinhos; w; w = w->prox) {
            vert *u = w->vertice;
            if (!u->visitado) {
                dfs(u, v);
                if (u->lowlink > v->indice) {
                    // Ordenar nomes na aresta
                    char aresta[256];
                    if (strcmp(v->nome, u->nome) < 0)
                        sprintf(aresta, "%s %s", v->nome, u->nome);
                    else
                        sprintf(aresta, "%s %s", u->nome, v->nome);
                    adiciona_lista(pontes, aresta);
                }
                if (u->lowlink < v->lowlink)
                    v->lowlink = u->lowlink;
            } else if (u != pai && u->indice < v->lowlink) {
                v->lowlink = u->indice;
            }
        }
    }

    for (vert *v = g->vertices; v; v = v->prox)
        if (!v->visitado)
            dfs(v, NULL);

    // Ordenar alfabeticamente
    for (int i = 0; i < pontes->tamanho - 1; i++) {
        for (int j = i + 1; j < pontes->tamanho; j++) {
            if (strcmp(pontes->dados[i], pontes->dados[j]) > 0) {
                char *tmp = pontes->dados[i];
                pontes->dados[i] = pontes->dados[j];
                pontes->dados[j] = tmp;
            }
        }
    }

    char *res = junta_lista(pontes);
    libera_lista(pontes);
    return res;
}
