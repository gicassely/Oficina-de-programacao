#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "cartas.h"
#include "jogo.h"

/***********************************************************************
Este arquivo contém todas as funções que definem as regras do jogo,
 como representar um jogo internamente na memória, contém funções que
 realizam cada uma das operações válidas no jogo (a maioria de mover
 cartas, mas também coisas como embaralhar todas as cartas das pilhas
 de periferia), bem como funções que dão informações sobre onde uma
 determinada carta pode ou não pode ser colocada segundo as regras, se
 um dado jogo já terminou ou não ou se o tempo limite dele se
 esgotou.
 **********************************************************************/

// Gera um novo struct que armazena os dados do jogo
struct jogo *inicializa_jogo(int tempo_limite){
  struct jogo *novo_jogo;
  struct pilha_de_cartas *baralho;
  int i, j, tmp = 1;
  novo_jogo = (struct jogo *) malloc(sizeof(struct jogo));
  if(novo_jogo == NULL) return NULL;
  novo_jogo -> embaralhamentos = 5;
  novo_jogo -> tempo_inicio = time(NULL);
  novo_jogo -> tempo_limite = tempo_limite * 60;
  /* Gerando um baralho com cartas do 2 até a Dama*/
  baralho = gera_pilha_de_cartas_inicial(2, DAMA);
  if(baralho == NULL){
    free(novo_jogo);
    return NULL;
  }
  /* Colocando quatro Ás, um em cada pilha da parte inferior central
     da mesa */
  for(i = 0; i < 4; i ++){
    novo_jogo -> crescente[i] = gera_pilha_de_cartas_vazia();
    if(novo_jogo -> crescente[i] == NULL ||
       coloca_carta_topo(novo_jogo -> crescente[i],
                         carta(0, i, AS)) == -1){
      // Se houver erro de alocação de memória, desalocamos o que foi
      // alocado até agora
      if(novo_jogo -> crescente[i] != NULL)
        destroi_pilha_de_cartas(novo_jogo -> crescente[i]);
      for(; i >= 0; i --)
        destroi_pilha_de_cartas(novo_jogo -> crescente[i]);
      free(novo_jogo);
      destroi_pilha_de_cartas(baralho);
      return NULL;
    }
  }
  /* Os outros quatro ÁS que não foram colocados na pilha crescente
     serão colocados no baralho.*/
  for(i = 0; i < 4; i ++){
    tmp = coloca_carta_topo(baralho, carta(1, i, AS));
    if(tmp == -1){
      // Se estamos aquui, houve erro de alocação e temos que
      // desalocar o que alocamos até agora
      destroi_pilha_de_cartas(baralho);
      /* Como vamos retornar mesmo, não faz mal reaproveitarmos a
         variável 'i': */
      for(i = 0; i < 4; i ++)
        destroi_pilha_de_cartas(novo_jogo -> crescente[i]);
      free(novo_jogo);
      return NULL;
    }
  }
  /* Colocando quatro reis nas 4 pilhas inferiores do centro da
     mesa */
  for(i = 0; i < 4; i ++){
    novo_jogo -> decrescente[i] = gera_pilha_de_cartas_vazia();
    if(novo_jogo -> decrescente[i] == NULL ||
       coloca_carta_topo(novo_jogo -> decrescente[i],
                         carta(0, i, REI)) == -1){
      // Se estamos aqui, teve erro de alocação e desalocamos o que
      // alocamos até agora
      if(novo_jogo -> decrescente[i] != NULL)
        destroi_pilha_de_cartas(novo_jogo -> decrescente[i]);
      for(; i >= 0; i --)
        destroi_pilha_de_cartas(novo_jogo -> decrescente[i]);
      for(i = 0; i < 4; i ++)
        destroi_pilha_de_cartas(novo_jogo -> crescente[i]);
      free(novo_jogo);
      destroi_pilha_de_cartas(baralho);
      return NULL;
    }
  }
  /* E os outros reis que sobraram, passamos para o baralho restante.*/
  for(i = 0; i < 4; i ++){
    tmp = coloca_carta_topo(baralho, carta(1, i, REI));
    if(tmp == -1){
      // Erro de alocação
      destroi_pilha_de_cartas(baralho);
      /* Como vamos retornar mesmo, não faz mal reaproveitarmos a
         variável 'i': */
      for(i = 0; i < 4; i ++){
        destroi_pilha_de_cartas(novo_jogo -> decrescente[i]);
        destroi_pilha_de_cartas(novo_jogo -> crescente[i]);
      }
      free(novo_jogo);
      return NULL;
    }
  }
  /* Agora que preenchemos o centro da tela, vamos emparalhar o que
     restou de nosso baralho e preencher as pilhas de cartas da
     periferia */
  embaralha_pilha_de_cartas(baralho);
  for(i = 0; i < 16; i ++){
    novo_jogo -> periferia[i] = gera_pilha_de_cartas_vazia();
    if(novo_jogo -> periferia[i] == NULL){
      // Erro de alocação
      for(i --; i >= 0; i --)
        destroi_pilha_de_cartas(novo_jogo -> periferia[i]);
      for(i = 0; i < 4; i ++){
        destroi_pilha_de_cartas(novo_jogo -> decrescente[i]);
        destroi_pilha_de_cartas(novo_jogo -> crescente[i]);
      }
      destroi_pilha_de_cartas(baralho);
      free(novo_jogo);
      return NULL;
    }
    // Tudo foi embaralhado. Agora damos as cartas, colocando-as nas
    // pilhas da periferia da mesa:
    for(j = 0; j < 6; j ++){
      tmp = transfere_carta(baralho, novo_jogo -> periferia[i]);
      if(tmp == -1){
        destroi_pilha_de_cartas(baralho);
        for(; i >= 0; i --)
          destroi_pilha_de_cartas(novo_jogo -> periferia[i]);
        for(i = 0; i < 4; i ++){
          destroi_pilha_de_cartas(novo_jogo -> decrescente[i]);
          destroi_pilha_de_cartas(novo_jogo -> crescente[i]);
        }
        free(novo_jogo);
        return NULL;
      }
    }
  }
  /* Finalizado. Agora não precisamos mais da pilha 'baralho'. */
  destroi_pilha_de_cartas(baralho);
  return novo_jogo;
}

// Desaloca o struct que armazena todos os dados do jogo
void destroi_jogo(struct jogo *partida){
  int i;
  for(i = 0; i < 4; i ++){
    destroi_pilha_de_cartas(partida -> crescente[i]);
    destroi_pilha_de_cartas(partida -> decrescente[i]);
  }
  for(i = 0; i < 16; i ++)
    destroi_pilha_de_cartas(partida -> periferia[i]);
  free(partida);
}

// Impreime na saída padrão uma representação gráfica do estado atual
// do jogo.
void imprime_jogo(struct jogo *partida){
  int i;
  /* Linha 1 */
  printf("      ");
  for(i = 4; i < 12; i ++){
    imprime_pilha_de_cartas(partida -> periferia[i]);
    printf("  ");
  }
  printf("\n");
  /* Linha 2 */
  imprime_pilha_de_cartas(partida -> periferia[3]);
  printf("                                                  ");
  imprime_pilha_de_cartas(partida -> periferia[12]);
  printf("\n");
  /* Linha 3 */
  imprime_pilha_de_cartas(partida -> periferia[2]);
  printf("              ");
  for(i = 0; i < 4; i ++){
    imprime_pilha_de_cartas(partida -> decrescente[i]);
    printf("  ");
  }
  printf("            ");
  imprime_pilha_de_cartas(partida -> periferia[13]);
  printf("\n");
  /* Linha 4 */
  imprime_pilha_de_cartas(partida -> periferia[1]);
  printf("              ");
  for(i = 0; i < 4; i ++){
    imprime_pilha_de_cartas(partida -> crescente[i]);
    printf("  ");
  }
  printf("            ");
  imprime_pilha_de_cartas(partida -> periferia[14]);
  printf("\n");
  /* Linha 5 */
  imprime_pilha_de_cartas(partida -> periferia[0]);
  printf("                                                  ");
  imprime_pilha_de_cartas(partida -> periferia[15]);
  printf("\n");
}

// Move uma carta do vetor de cartas da periferia para o vetor de
// cartas crescentes do centro da tela
int move_periferia_crescente(struct jogo *partida, int origem,
                             int destino){
  int carta_origem, carta_destino, tmp;
  carta_origem = topo_da_pilha(partida -> periferia[origem]);
  carta_destino = topo_da_pilha(partida -> crescente[destino]);
  // Se houver posição inválida, saímos.
  if(origem < 0 || destino < 0 || origem >= 16 || destino >= 4 ||
     carta_origem == 0)
    return 0;
  // Só podemos mover a carta se ela for do mesmo naipe e se ela tiver
  // um valor maior em 1 do que a carta do topo da pilha de
  // destino. Se apilha de destino for vazia, só podemos mover um AS
  // para lá.
  if((carta_valor(carta_origem) == AS &&
      partida -> crescente[destino] -> tamanho == 0) ||
     (carta_valor(carta_origem) == carta_valor(carta_destino) + 1 &&
      carta_naipe(carta_origem) == carta_naipe(carta_destino)))
    tmp = transfere_carta(partida -> periferia[origem],
                          partida -> crescente[destino]);
  else
    return 0;
  if(tmp == -1){
    /* Erro de realloc. Sem espaço. Terminando tudo. */
    destroi_jogo(partida);
    return -1;
  }
  return 1;
}

// Move uma carta do vetor de cartas da periferia para o vetor de
// cartas decrescentes do centro da tela
int move_periferia_decrescente(struct jogo *partida, int origem,
                               int destino){
  int carta_origem, carta_destino, tmp;
  carta_origem = topo_da_pilha(partida -> periferia[origem]);
  carta_destino = topo_da_pilha(partida -> decrescente[destino]);
  // Saímos em caso de posições inválidas
  if(origem < 0 || destino < 0 || origem >= 16 || destino >= 4 ||
     carta_origem == 0)
    return 0;
  // Só podemos mover a carta se ela for do mesmo naipe e com o valor
  // 1 ponto menor que a carta do topo da pilha de destino. Se a pilha
  // de destino for vazia, só podemos mover um REI para lá.
  if((carta_valor(carta_origem) == REI &&
      partida -> decrescente[destino] -> tamanho == 0) ||
     (carta_valor(carta_origem) == carta_valor(carta_destino) -1 &&
      carta_naipe(carta_origem) == carta_naipe(carta_destino)))
    tmp = transfere_carta(partida -> periferia[origem],
                          partida -> decrescente[destino]);
  else
    return 0;
  if(tmp == -1){
    /* Erro de realloc. Sem espaço. Terminando tudo. */
    destroi_jogo(partida);
    return -1;
  }
  return 1;
}

// Move uma carta de uma das pilhas perféricas para outra pilha
// periférica.
int move_entre_periferias(struct jogo *partida, int origem, int destino){
  int carta_origem, carta_destino, tmp;
  carta_origem = topo_da_pilha(partida -> periferia[origem]);
  carta_destino = topo_da_pilha(partida -> periferia[destino]);
  // Saímos em caso de posição inválida
  if(origem < 0 || destino < 0 || origem >= 16 || destino >= 16 ||
     carta_origem == 0)
    return 0;
  // Se estamos movendo para um destino igual à origem, isso deve
  // representar um sucesso automático. Não precisamos testar mais
  // nada. Mas também nada muda no estado do jogo
  if(origem == destino)
    return topo_da_pilha(partida->periferia[origem]);
  // Só podemos mover se a pilha de destino for vazia ou se as cartas
  // do topo de ambas tem o mesmo naipe.
  if(carta_naipe(carta_origem) !=  carta_naipe(carta_destino) &&
     carta_destino != 0)
    return 0;
  // Se a pilha de destino não é vazia, só podemos mover para lá
  // cartas que são um ponto maiores ou menores que a carta do topo da
  // pilha
  if(carta_valor(carta_origem) != carta_valor(carta_destino) - 1 &&
     carta_valor(carta_origem) != carta_valor(carta_destino) + 1 &&
     partida -> periferia[destino] -> tamanho != 0)
    return 0;
  tmp = transfere_carta(partida -> periferia[origem],
                        partida -> periferia[destino]);
  if(tmp == -1){
    /* Erro de realloc. Acabando com tudo no jogo. */
    destroi_jogo(partida);
    free(partida);
    return -1;
  }
  return 1;
}

// Move uma carta da pilha central de baixo para a pilha central de
// cima.
int move_crescente_decrescente(struct jogo *partida, int origem,
                               int destino){
  int carta_origem, carta_destino, tmp;
  // Saímos em caso de posição inválida.
  if(origem != destino || origem < 0 || origem > 3)
    return 0;
  carta_origem = topo_da_pilha(partida -> crescente[origem]);
  carta_destino = topo_da_pilha(partida -> decrescente[destino]);
  // Se a carta de baixo não é o decremento da de cima enem é um REI
  // no caso da de cima ser uma pilha vazia, então saímos da função
  if(carta_valor(carta_origem) != carta_valor(carta_destino) - 1 &&
     (carta_destino == 0 && carta_valor(carta_origem) != REI))
    return 0;
  tmp = transfere_carta(partida -> crescente[origem],
                        partida -> decrescente[destino]);
  if(tmp == -1){
    destroi_jogo(partida);
    return -1;
  }
  return 1;
}

// Move uma carta de uma das pilhas centrais de cima para uma das
// pilhas centrais de baixo.
int move_decrescente_crescente(struct jogo *partida, int origem,
                               int destino){
  int carta_origem, carta_destino, tmp;
  // Saindo em caso de posição inválida
  if(origem != destino || origem < 0 || origem > 3)
    return 0;
  carta_origem = topo_da_pilha(partida -> decrescente[origem]);
  carta_destino = topo_da_pilha(partida -> crescente[destino]);
  // Não devemos prosseguir se a carta de cima não for um incremento
  // da carta de baixo e nem for um AS no caso da pilha de baixo ser
  // vazia.
    if(carta_valor(carta_origem) != carta_valor(carta_destino) + 1 &&
     (carta_destino == 0 && carta_valor(carta_origem) != AS))
    return 0;
  tmp = transfere_carta(partida -> decrescente[origem],
                        partida -> crescente[destino]);
  if(tmp == -1){
    destroi_jogo(partida);
    return -1;
  }
  return 1;
}

// Move uma carta da pilha central inferior para uma das pilhas de
// periferia
int move_crescente_periferia(struct jogo *partida, int origem,
                             int destino){
  int carta_origem, carta_destino, tmp;
  carta_origem = topo_da_pilha(partida -> crescente[origem]);
  carta_destino = topo_da_pilha(partida -> periferia[destino]);
  // Não prosseguimos em caso de posições inválidas ou se não
  // houver carta na origem.
  if(origem < 0 || origem > 3 || destino < 0 || destino > 16 ||
     carta_origem == 0)
    return 0;
  // Não prosseguimos se houver carta no destino e uma carta não for o
  // incremento da outra e do mesmo naipe.
  if(carta_destino != 0 &&
     ! ((carta_valor(carta_origem) == carta_valor(carta_destino) + 1 ||
         carta_valor(carta_origem) == carta_valor(carta_destino) - 1) &&
        carta_naipe(carta_origem) == carta_naipe(carta_destino))){
    return 0;
  }
  tmp = transfere_carta(partida -> crescente[origem],
                        partida -> periferia[destino]);
  if(tmp == -1){
    // Erro de alocação. Encerrando.
    destroi_jogo(partida);
    return -1;
  }
  return 1;
}

// Move uma carta da região central superior para uma das pilhas de
// periferia.
int move_decrescente_periferia(struct jogo *partida, int origem,
                             int destino){
  int carta_origem, carta_destino, tmp;
  carta_origem = topo_da_pilha(partida -> decrescente[origem]);
  carta_destino = topo_da_pilha(partida -> periferia[destino]);
  // Não prosseguimos em caso de posição inválida
  if(origem < 0 || origem > 3 || destino < 0 || destino > 16 ||
     carta_origem == 0)
    return 0;
  // Não prosseguimos caso o destino não seja vazio e uma carta não
  // seja o incremento da outra do mesmo naipe
  if(carta_destino != 0 &&
     ! ((carta_valor(carta_origem) == carta_valor(carta_destino) + 1 ||
         carta_valor(carta_origem) == carta_valor(carta_destino) - 1) &&
        carta_naipe(carta_origem) == carta_naipe(carta_destino))){
    return 0;
  }
  tmp = transfere_carta(partida -> decrescente[origem],
                        partida -> periferia[destino]);
  if(tmp == -1){
    // Erro de alocação. Encerrando.
    destroi_jogo(partida);
    return -1;
  }
  return 1;
}

// Esta função embaralha todas as cartas da periferia da mesa do jogo.
int embaralha_periferia(struct jogo *partida){
  struct pilha_de_cartas *baralho;
  int i, j, tmp;
  /* Criando um baralho auxiliar para onde iremos transferir toda a
     periferia. */
  baralho = gera_pilha_de_cartas_vazia();
  if(baralho == NULL){
    // Se não houver espaço, caímos fora. */
    destroi_jogo(partida);
    return -1;
  }
  /* Movendo toda a periferia para o baralho */
  for(i = 0; i < 16; i ++)
    for(j = 0; j < partida -> periferia[i] -> tamanho; j ++){
      tmp = transfere_carta(partida -> periferia[i], baralho);
      if(tmp == -1){ /* Erro de realloc. Terminando tudo */
        destroi_jogo(partida);
        destroi_pilha_de_cartas(baralho);
        free(partida);
        return -1;
      }
    }
  /* Embaralhando o baralho auxiliar */
  embaralha_pilha_de_cartas(baralho);
  /* Movendo o baralho para a periferia de novo */
  while(baralho -> tamanho > 0)
    for(i = 0; i < 16; i ++){
      if(baralho -> tamanho == 0)
        break;
      tmp = transfere_carta(baralho, partida -> periferia[i]);
      if(tmp == -1){
        destroi_jogo(partida);
        return -1;
      }
    }
  /* Destruindo o baralho e encerrando */
  destroi_pilha_de_cartas(baralho);
  return 1;
}

// Esta função retorna 1 se o jogo foi finalizado (não há cartas na
// periferia) e 0 caso contrário.
int jogo_encerrado(struct jogo *partida){
  int i;
  for(i = 0; i < 16; i ++)
    if(partida -> periferia[i] -> tamanho > 0)
      return 0;
  return 1;
}

// Esta função retorna 1 caso o intervalo em segundos entre o momento
// atual e o começo do jogo for maior do que o tempo máximo permitido
// para o jogo. Caso o tempo máximo esteja registrado como 0,
// assumimos que não há tempo máximo permitido e esta função sempre
// retorna 1.
int tempo_esgotado(struct jogo *partida){
  time_t agora = time(NULL);
  if(partida -> tempo_limite == 0)
    return 0;
  if(agora - partida -> tempo_inicio > partida -> tempo_limite)
    return 1;
  return 0;
}

// Esta função checa em quais pilhas do jogo seria possível colocar a
// carta recebida como argumento. Ela retorna um número binário, que
// pode possuir 1 ou 0 em cada um de seus 24 bits menos
// significativos. Os 16 bits menos significativos correspondem às 16
// pilhas de cartas da periferia. Os próximos 4 bits menos
// significativos correspondem às quatro pilhas centrais superiores do
// jogo. E os demais quatro bits sisgnificativos correspondem às 4
// pilhas de baixo:
// ???????.CCCC.DDDD.PPPPPPPPPPPPPPPPP
// Sempre que for válido mover a carta passada como argumento para uma
// dada região, o bit do número retornado por essa função que
// corresponde à esta região estará com o valor de 1. Caso contrário,
// estará valendo 0.
int movimentos_possiveis(struct jogo *partida, int carta){
  int i, resposta = 0, acumulador = 1;
  if(carta == 0) return 0;
  for(i = 0; i < 24; i ++){
    if(i < 16){
      /* Checando a possibilidade de mover para a periferia */
      int topo = topo_da_pilha(partida -> periferia[i]);
      if(topo == 0 ||
         (carta_naipe(carta) == carta_naipe(topo) &&
          (carta_valor(carta) == carta_valor(topo) + 1 ||
           carta_valor(carta) == carta_valor(topo) - 1))){
        resposta += acumulador;
      }
    }
    else if(i >= 16 && i < 20){
      int topo = topo_da_pilha(partida -> decrescente[i - 16]);
      if((topo != 0 && carta_naipe(carta) == carta_naipe(topo) &&
          carta_valor(carta) == carta_valor(topo) - 1) ||
         (topo == 0 && carta_valor(carta) == REI &&
          carta_naipe(carta) == i - 16)){
        resposta += acumulador;
      }
    }
    else{
      int topo = topo_da_pilha(partida -> crescente[i - 20]);
      if((topo != 0 && carta_naipe(carta) == carta_naipe(topo) &&
          carta_valor(carta) == carta_valor(topo) + 1) ||
         (topo == 0 && carta_valor(carta) == AS &&
          carta_naipe(carta) == i - 20)){
        resposta += acumulador;
      }
    }
    acumulador <<= 1;
  }
  return resposta;
}

// Baseada na função anterior, esta função retorna 1 ou 0. O valor 1 é
// retornado se existe alguma pilha do jogo cuja carta de cima pode
// ser movida e 0 caso contrário.
int tem_movimentos_possiveis(struct jogo *partida){
  int i;
  for(i = 0; i < 16; i ++){
    if(movimentos_possiveis(partida,
                            topo_da_pilha(partida -> periferia[i])))
      return 1;
  }
  for(i = 0; i < 4; i ++){
    if(movimentos_possiveis(partida,
                            topo_da_pilha(partida -> crescente[i])))
      return 1;
  }
  for(i = 0; i < 4; i ++){
    if(movimentos_possiveis(partida,
                            topo_da_pilha(partida -> decrescente[i])))
      return 1;
  }
  return 0;
}
