#ifndef _JOGO_H_
#define _JOGO_H_

#include <time.h>
#include "cartas.h"

struct jogo{
  // Cartas que circundam a mesa
  struct pilha_de_cartas *periferia[16];
  // Cartas superiores do centro
  struct pilha_de_cartas *decrescente[4];
  // Cartas inferiores do centro
  struct pilha_de_cartas *crescente[4];
  // Quantas vezes ainda podemos embaralhar as cartas e quanto tempo
  // o jogo pode durar (0 significa sem limite de tempo).
  int embaralhamentos, tempo_limite;
  // Quando o jogo começou
  time_t tempo_inicio;
};

struct jogo *inicializa_jogo(int);
void destroi_jogo(struct jogo *);
void imprime_jogo(struct jogo *);
int tem_movimentos_possiveis(struct jogo *);
int move_periferia_crescente(struct jogo *, int, int);
int move_periferia_decrescente(struct jogo *, int, int);
int move_entre_periferias(struct jogo *, int, int);
int move_crescente_decrescente(struct jogo *, int, int);
int move_decrescente_crescente(struct jogo *, int, int);
int move_crescente_periferia(struct jogo *, int, int);
int move_decrescente_periferia(struct jogo *, int, int);
int embaralha_periferia(struct jogo *);
int jogo_encerrado(struct jogo *);
int tempo_esgotado(struct jogo *);
int movimentos_possiveis(struct jogo *, int);

#endif
