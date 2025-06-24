# Grafos
Bibliotéca sobre grafos em C.
# Projeto: Análise de Grafos em C

Este programa lê a definição de um grafo a partir de um arquivo de entrada e realiza diversas análises estruturais, como número de vértices, número de arestas, componentes conexas, verificação de bipartição, diâmetro, vértices de corte e arestas de corte.

---

## Estrutura do Projeto

- `grafos.c` — Código principal que implementa a leitura, construção e análise do grafo.
- `grafo.h` — Cabeçalho com as estruturas de dados e protótipos das funções.
- `teste.c` — Arquivo que chama a lib grafo.h
- `Makefile` — Para facilitar a compilação.
- Arquivos de entrada: `teste1.in`, `teste2.in`, ..., etc.

---

## Compilação

make

```bash
gcc -o grafos grafos.c

## Execução

./testagrafo < entrada.in


E digite/passe os dados na entrada padrão.
Formato da Entrada

A entrada segue o seguinte padrão:

nome_do_grafo
vértice1 -- vértice2 peso
vértice3 -- vértice4
vértice5
...
    Linhas que começam com '//' devem ser ignoradas.

    A primeira linha é o nome do grafo.

    Cada linha seguinte representa:

        Uma aresta com peso: v1 -- v2 peso

        Uma aresta sem peso (peso zero será assumido): v1 -- v2

        Um vértice isolado: v1

## Saída

O programa imprime as seguintes informações sobre o grafo:

    Nome do grafo

    Número de vértices

    Número de arestas

    Número de componentes conexas

    Se o grafo é bipartido

    Diâmetro (maior distância entre dois vértices do mesmo componente)

    Vértices de corte (articulações)

    Arestas de corte (pontes)

## Exemplo de Entrada

triângulo_com_vértice
um -- dois 12
dois -- quatro 24
quatro

## Exemplo de Saída

grafo: triângulo_com_vértice
4 vértices
2 arestas
2 componentes
não bipartido
diâmetros: 36
vértices de corte: dois
arestas de corte: (um, dois), (dois, quatro)

Funcionalidades implementadas

    Leitura dinâmica do grafo

    Inserção de vértices e arestas com verificação de duplicatas

    Identificação de componentes conexas

    Verificação de bipartição via BFS

    Cálculo do diâmetro por BFS múltiplo

    Detecção de vértices e arestas de corte via DFS + lowpoints


    O grafo é não-direcionado e pode conter pesos.

    Todos os nomes de vértices são tratados como strings.

    O programa ignora linhas vazias ou comentadas com //.
