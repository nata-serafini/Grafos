# Biblioteca de Grafos em C

## Projeto: Análise de Grafos

Este programa lê a definição de um grafo a partir de um arquivo de entrada e realiza diversas análises estruturais, como:

- Número de vértices  
- Número de arestas  
- Componentes conexas  
- Verificação de bipartição  
- Cálculo do diâmetro  
- Identificação de vértices de corte  
- Identificação de arestas de corte  

---

## Estrutura do Projeto

- `grafos.c` — Código principal que implementa a leitura, construção e análise do grafo.  
- `grafo.h` — Cabeçalho com as estruturas de dados e protótipos das funções.  
- `teste.c` — Arquivo de teste que utiliza a biblioteca `grafo.h`.  
- `Makefile` — Facilita a compilação.  
- Arquivos de entrada: `teste1.in`, `teste2.in`, etc.

---

## Compilação

```bash
make
```

---

## Execução

```bash
./testagrafo < entrada.in
```

Os dados devem ser passados pela entrada padrão.

---

## Formato da Entrada

A entrada segue o seguinte padrão:

- A primeira linha contém o **nome do grafo**.
- Linhas seguintes representam:
  - Uma aresta com peso: `v1 -- v2 peso`
  - Uma aresta sem peso (assume-se peso zero): `v1 -- v2`
  - Um vértice isolado: `v1`
- Linhas iniciadas com `//` são ignoradas.

---

## Saída

O programa imprime as seguintes informações:

- Nome do grafo  
- Número de vértices  
- Número de arestas  
- Número de componentes conexas  
- Se o grafo é bipartido  
- Diâmetro (maior distância entre dois vértices no mesmo componente)  
- Vértices de corte (articulações)  
- Arestas de corte (pontes)

---

## Exemplo de Entrada

```
triângulo_com_vértice  
um -- dois 12  
dois -- quatro 24  
quatro
```

## Exemplo de Saída

```
grafo: triângulo_com_vértice  
4 vértices  
2 arestas  
2 componentes  
não bipartido  
diâmetro: 36  
vértices de corte: dois  
arestas de corte: (um, dois), (dois, quatro)
```

---

## Funcionalidades Implementadas

- Leitura dinâmica do grafo  
- Inserção de vértices e arestas com verificação de duplicatas  
- Identificação de componentes conexas  
- Verificação de bipartição via BFS  
- Cálculo do diâmetro com múltiplas BFS  
- Detecção de vértices e arestas de corte com DFS e lowpoints

---

## Observações

- O grafo é **não direcionado** e pode conter pesos.  
- Todos os nomes de vértices são tratados como **strings**.  
- Linhas vazias ou comentadas com `//` são ignoradas.
