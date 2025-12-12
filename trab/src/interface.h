#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "jogo.h"

// Tipos de jogadas:

void inicializa_interface(void);
int obtem_tempo_de_jogo(void);
void avisa_que_tempo_esgotou(struct jogo *);
void avisa_que_jogador_ganhou(struct jogo *);
void avisa_que_jogador_desistiu(struct jogo *);
int coleta_opcao_do_jogador(struct jogo *);
void move(struct jogo *);
void embaralha(struct jogo *);
int menu_escolhe_cartas_moviveis(struct jogo *);
int menu_escolhe_para_onde_mover(struct jogo *, int);

#endif
