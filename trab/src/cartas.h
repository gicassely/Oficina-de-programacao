#ifndef _CARTAS_H_
#define _CARTAS_H_

/* Uma carta pode ter 4 naipes diferentes */
#define OUROS   0
#define ESPADAS 1
#define COPAS   2
#define PAUS    3

/* Nomes especiais de alguns valores numéricos de cartas */
#define AS      1
#define VALETE 11
#define DAMA   12
#define REI    13


/* Uma pilha de cartas é um array delimitado por 0 armazenado em uma
   estrutura que também tem o tamanho de tal array. É um array de
   inteiro porque representamos cada carta como inteiro.*/
struct pilha_de_cartas{
  int tamanho;
  int *pilha;
};

/* Obtém os dados de uma carta */
int carta_baralho(int);
int carta_naipe(int);
int carta_valor(int);
/* Gera carta */
int carta(int, int, int);
struct pilha_de_cartas *gera_pilha_de_cartas_vazia(void);
struct pilha_de_cartas *gera_pilha_de_cartas_inicial(int, int);
void embaralha_pilha_de_cartas(struct pilha_de_cartas *);
int retira_carta_topo(struct pilha_de_cartas *);
int coloca_carta_topo(struct pilha_de_cartas *, int);
void destroi_pilha_de_cartas(struct pilha_de_cartas *);
int transfere_carta(struct pilha_de_cartas *, struct pilha_de_cartas *);
void imprime_carta(int);
void imprime_pilha_de_cartas(struct pilha_de_cartas *);
int topo_da_pilha(struct pilha_de_cartas *);

#endif
