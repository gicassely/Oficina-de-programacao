#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "cartas.h"
#include "jogo.h"
#include "interface.h"

int main(void){
  struct jogo *partida;
  int jogando = 1, em_andamento = 1, opcao = 0;
  // Inicializando o gerador de números aleatórios:
  srand((unsigned)time(NULL));
  while(jogando){
    // Inicializando jogo:
    inicializa_interface();
    partida = inicializa_jogo(obtem_tempo_de_jogo());
    if(partida == NULL){
      fprintf(stderr, "ERRO: Sem memória disponível.\n");
      return 1;
    }
    em_andamento = 1;
    while(em_andamento){
      if(jogo_encerrado(partida) || tempo_esgotado(partida)){
        em_andamento = 0;
        break;
      }
      opcao = coleta_opcao_do_jogador(partida);
      switch(opcao){
      case 1: move(partida); break;
      case 2: embaralha(partida); break;
      case 3: em_andamento = 0; break;
      default: break;
      }
    }
    if(tempo_esgotado(partida))
      avisa_que_tempo_esgotou(partida);
    else if(jogo_encerrado(partida))
      avisa_que_jogador_ganhou(partida);
    else
      avisa_que_jogador_desistiu(partida);
    destroi_jogo(partida);
  }
  return 0;
}
