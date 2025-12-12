#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"

/***********************************************************************
Este arquivo contém todas as funções usadas para se interagir com o
usuário. São funções de Entrada e Saída. Teoricamente seria o único
arquivo-fonte que deveria ser substituido se a interface do jogo
precisar ser mudada.
***********************************************************************/

/* Função chamada pela função "main" antes de iniciar a interação com
   o usuário. */
void inicializa_interface(void){
  system("clear");
  return;
}

/* Função chamada pela função "main" e responsável por interagir com o
   usuário para definir qual será o tempo de jogo. Basicamente escolhe
   o modo de jogo "Melhor Tempo" ou "Contra o Relógio". Também fornece
   o meio pelo qual o usuário sai do jogo. */
int obtem_tempo_de_jogo(void){
  int resposta = -1;
  printf("Escolha o modo de jogo:\n\n");
  printf("(1) Melhor Tempo     (2) Contra o Relógio    (3) Sair\n\n");
  printf("Sua escolha: ");
  scanf("%d", &resposta);
  while ( getchar() != '\n' );
  if(resposta == 1)
    return 0;
  else if(resposta == 2){
    resposta = -1;
    printf("Quanto tempo em minutos você deseja ter? Resposta: ");
    scanf("%d", &resposta);
    while ( getchar() != '\n' );
    if(resposta <= 0){
      printf("Resposta inválida. Assumindo tempo padrão de 20 minutos.\n\n");
      return 20;
    }
    else return resposta;
  }
  else if(resposta == 3)
    exit(0);
  else{
    printf("RESPOSTA INVÁLIDA!!!!!\n\n");
    return obtem_tempo_de_jogo();
  }
}

/* Função chamada pela função "main" que avisa quando o tempo de jogo
   se esgotou caso esteja-se jogando com um limite de tempo. */
void avisa_que_tempo_esgotou(struct jogo *partida){
  printf("TEMPO ESGOTADO!!!\n");
  printf("Seu tempo de %d minutos para jogar se encerrou.\n\n",
         partida -> tempo_limite);
  printf("Pressione ENTER para continuar.\n");
  while ( getchar() != '\n' );
}

/* Função chamada pela função "main" que avisa quando o jogo termina
   com vitória do jogador. */
void avisa_que_jogador_ganhou(struct jogo *partida){
  time_t tempo = time(NULL) - partida -> tempo_inicio;
  printf("Você ganhou!!!\n");
  printf("Tempo gasto: %d minutos e %d segundos.\n",
         (int) tempo / 60, (int) tempo % 60);
  if(partida -> tempo_limite != 0){
    printf("Tempo máximo permitido: %d minutos.\n",
           partida -> tempo_limite / 60);
  }
  printf("Pressione ENTER para continuar.\n");
  while ( getchar() != '\n' );

}

/* Função chamada pela função "main" que escreve uma mensagem caso o
   jogador tenha desistido do jogo. */
void avisa_que_jogador_desistiu(struct jogo *partida){
  time_t tempo = time(NULL) - partida -> tempo_inicio;
  printf("Você desistiu após %d minutos e %d segundos de jogo.\n",
         (int) tempo / 60, (int) tempo % 60);
  printf("Pressione ENTER para continuar.\n");
  while ( getchar() != '\n' );
}

/* Função chamada pela função "main" que a cada iteração pergunta o
   que o jogador deseja fazer. */
int coleta_opcao_do_jogador(struct jogo *partida){
  int opcao = -1;
  system("clear");
  imprime_jogo(partida);
  if(tem_movimentos_possiveis(partida))
    printf("\n              1-Move    ");
  else
    printf("\n                        ");
  if(partida -> embaralhamentos > 0)
    printf("2-Embaralha   ");
  else
    printf("              ");
  printf("3-Desiste\n");
  printf("Sua opção: ");
  scanf("%d", &opcao);
  while ( getchar() != '\n' );
  if(opcao < 1 || opcao > 3){
    printf("Opção inválida!!!\n");
    return coleta_opcao_do_jogador(partida);
  }
  return opcao;
}

/* Esta função gera um menu onde o usuário pode escolher um número de
   carta tal qu a carta em questão pode ser movida para algum lugar
   durante o jogo.*/
int menu_escolhe_cartas_moviveis(struct jogo *partida){
  int i, opcao;
  /* Primeira linha */
  printf("      ");
  for(i = 5; i < 13; i ++){
    if(movimentos_possiveis(partida,
                            topo_da_pilha(partida -> periferia[i - 1]))){
      if(i < 10)
        printf("( %d)  ", i);
      else
        printf("(%d)  ", i);
    }
    else printf("(__)  ");
  }
  printf("\n");
  /* Segunda Linha */
  if(movimentos_possiveis(partida,
                          topo_da_pilha(partida -> periferia[3])))
    printf("( 4)");
  else printf("(__)");
  printf("                                                  ");
  if(movimentos_possiveis(partida,
                          topo_da_pilha(partida -> periferia[12])))
    printf("(13)");
  else printf("(__)");
  printf("\n");
  /* Terceira Linha */
  if(movimentos_possiveis(partida,
                          topo_da_pilha(partida -> periferia[2])))
    printf("( 3)");
  else printf("(__)");
  printf("              ");
  for(i = 0; i <  4; i ++){
    if(movimentos_possiveis(partida,
                            topo_da_pilha(partida -> decrescente[i])))
      printf("(%d)  ", i + 17);
    else printf("(__)  ");
  }
  printf("            ");
  if(movimentos_possiveis(partida,
                          topo_da_pilha(partida -> periferia[13])))
    printf("(14)");
  else printf("(__)");
  printf("\n");
  /* Quarta Linha */
  if(movimentos_possiveis(partida,
                          topo_da_pilha(partida -> periferia[1])))
    printf("( 2)");
  else printf("(__)");
  printf("              ");
  for(i = 0; i <  4; i ++){
    if(movimentos_possiveis(partida,
                            topo_da_pilha(partida -> crescente[i])))
      printf("(%d)  ", i + 21);
    else printf("(__)  ");
  }
  printf("            ");
  if(movimentos_possiveis(partida,
                          topo_da_pilha(partida -> periferia[14])))
    printf("(15)");
  else printf("(__)");
  printf("\n");
  /* Quinta Linha */
  if(movimentos_possiveis(partida,
                          topo_da_pilha(partida -> periferia[0])))
    printf("( 1)");
  else printf("(__)");
  printf("                                                  ");
  if(movimentos_possiveis(partida,
                          topo_da_pilha(partida -> periferia[15])))
    printf("(16)");
  else printf("(__)");
  printf("\n\n");
  printf("Sua escolha: ");
  scanf("%d", &opcao);
  while ( getchar() != '\n' );
  return opcao;
}

/* Esta função serve para mostrar um menu onde o usuário pode
   selecionar o número de qualquer pilha de cartas do jogo para onde o
   usuário seria capaz de mover a carta passada como argumento. */
int menu_escolhe_para_onde_mover(struct jogo *partida, int carta){
  int i, opcao, movimentos, tmp;
  movimentos = movimentos_possiveis(partida, carta);
  /* Linha 1 */
  printf("      ");
  tmp = movimentos >> 4;
  for(i = 0; i < 8; i ++){
    if(tmp % 2){
      if(i < 5)
        printf("( %d)", 5+i);
      else
        printf("(%d)", 5+i);
    }
    else
      printf(" XX ");
    printf("  ");
    tmp >>= 1;
  }
  printf("\n");
  /* Linha 2 */
  if((movimentos >> 3) % 2)
    printf("( 4)");
  else
    printf(" XX ");
  printf("                                                  ");
  if((movimentos >> 12) % 2)
    printf("(13)");
  else printf(" XX ");
  printf("\n");
  /* Linha 3 */
  if((movimentos >> 2) % 2)
    printf("( 3)");
  else
    printf(" XX ");
  printf("              ");
  tmp = movimentos >> 16;
  for(i = 0; i < 4; i ++){
    if(tmp % 2)
      printf("(%d)  ", 17+i);
    else
      printf(" XX   ");
    tmp >>= 1;
  }
  printf("            ");
  if((movimentos >> 13) % 2)
    printf("(14)");
  else printf(" XX ");
  printf("\n");
  /* Linha 4*/
  if((movimentos >> 1) % 2)
    printf("( 2)");
  else
    printf(" XX ");
  printf("              ");
  tmp = movimentos >> 20;
  for(i = 0; i < 4; i ++){
    if(tmp % 2)
      printf("(%d)  ", 21+i);
    else
      printf(" XX   ");
    tmp >>= 1;
  }
  printf("            ");
  if((movimentos >> 14) % 2)
    printf("(15)");
  else printf(" XX ");
  printf("\n");
  /* Linha 5*/
  if(movimentos % 2)
    printf("( 1)");
  else
    printf(" XX ");
  printf("                                                  ");
  if((movimentos >> 15) % 2)
    printf("(16)");
  else printf(" XX ");
  printf("\n\n");
  printf("Sua escolha: ");
  scanf("%d", &opcao);
  while ( getchar() != '\n' );
  return opcao;
}

/* Função responsável por mover cartas pelo jogo. Ele interage com o
   usuário perguntando que carta ele quer mover e para onde.  Ela
   também realiza os ditos movimentos, fornecendo indicativos de quais
   são as cartas válidas de serem movidas e a posição válida delas.
*/
void move(struct jogo *partida){
  int opcao, movimentos, tmp, destino, origem;
  struct pilha_de_cartas *pilha_escolhida;
  /* Primeiro perguntamos que cartas desejamos mover */
  printf("Qual carta mover?\n");
  /* Aqui aparece na tela que cartas podem e não podem ser movidas. */
  opcao = menu_escolhe_cartas_moviveis(partida);
  if(opcao < 1 || opcao > 24){
    printf("Carta inválida!!!!!\n");
    printf("Pressione ENTER para continuar.\n");
    while ( getchar() != '\n' );
    return;
  }
  // Guardamos então qual pilha terá sua carta do topo movida. Guardar
  // o ponteiro é útil para acessar mais rapidamente as cartas e
  // também para caso ocorra algum problema na alocação de memória,
  // nós saberemos quais pilhas de cartas deram problemas e por isso
  // não precisam ser desalocadas.
  origem = opcao;
  if(opcao < 17)
    pilha_escolhida = partida -> periferia[opcao - 1];
  else if(opcao >= 17 && opcao < 21)
    pilha_escolhida = partida -> decrescente[opcao - 17];
  else
    pilha_escolhida = partida -> crescente[opcao - 21];
  // Caso a carta não pode ser movida, nós avisamos isso e não fazemos
  // nada.
  movimentos = movimentos_possiveis(partida,
                                    topo_da_pilha(pilha_escolhida));
  if(movimentos == 0){
    printf("Esta carta não pode ser movida.\n");
    printf("Pressione ENTER para continuar.\n");
    while ( getchar() != '\n' );
    return;
  }
  // Agora o usuário escolhe para onde mover a carta após mostrarmos
  // na tela os lugares disponíveis.
  printf("Quer mover a carta para onde?\n");
  opcao = menu_escolhe_para_onde_mover(partida,
                                       topo_da_pilha(pilha_escolhida));
  if(opcao < 1 || opcao > 24){
    printf("Destino inválido!!!!!\n");
    printf("Pressione ENTER para continuar.\n");
    while ( getchar() != '\n' );
    return;
  }
  destino = opcao;
  // Após obtermos o número do local para onde vamos mover a carta,
  // invocamos uma função adeqüada para a tarefa, dependendo de onde
  // está a pilha de origem e de destino. Mover da periferia para
  // outra região da periferia invoca funções diferentes do que mover
  // da periferia para as pilhas centrais superiores. Além disso,
  // também armazenamos um ponteiro da pilha de cartas do destino para
  // que caso tenhamos problemas de alocação de memória durante o
  // movimento, saibamos quais pilhas deram problema e portanto não
  // estão mais alocadas. Estas pilhas não precisam mais ser liberadas
  // quando formos desalocar o espaço ocupado pelo jogo.
  if(origem <= 16 && destino <= 16){
    tmp = move_entre_periferias(partida, origem - 1, destino - 1);
  }
  else if(origem <= 16 && destino > 16 && destino < 21)
    tmp = move_periferia_decrescente(partida, origem - 1, destino - 17);
  else if(origem <= 16 && destino > 20)
    tmp = move_periferia_crescente(partida, origem - 1, destino - 21);
  else if(origem > 20 && destino > 16 && destino <= 20)
    tmp = move_crescente_decrescente(partida, origem - 21, destino - 17);
  else if(origem <= 20 && origem > 16 && destino > 20)
    tmp = move_decrescente_crescente(partida, origem - 17, destino - 21);
  else  if(origem > 16 && origem <= 20 && destino < 17)
    tmp = move_decrescente_periferia(partida, origem - 17, destino - 1);
  else  if(origem > 20 && destino < 17)
    tmp = move_crescente_periferia(partida, origem - 21, destino - 1);
  else{
    // Se uma jogada inválida foi pedida, passamos aqui
    printf("Jogada não reconhecida!!\n");
    printf("Pressione ENTER para continuar.\n");
    while ( getchar() != '\n' );
    return;
  }
  if(tmp < 0){
    // Espera-se que isso nunca seja executado. Chegamos aqui somente
    // caso não tenhamos memória no computador. Neste caso,
    // desalocamos toda a memória e saímos.
    destroi_jogo(partida);
    fprintf(stderr, "Sem memória disponível. Encerrando.");
    exit(1);
  }
  else if(tmp == 0){
    // Chegamos aqui se o usuário insiste em mover cartas para pilhas
    // onde não é permitido de acordo com as regras do jogo.
    printf("Movimento inválido!!\n");
    printf("Pressione ENTER para continuar.\n");
    while ( getchar() != '\n' );
    return;
  }
  return;
}

/*
  Esta função é a interface de embaralhamento das cartas da periferia.
  Ela indica quantos embaralhamentos ainda restam e realiza o
  embaralhamento em si. Ela é invocada pela função "main" caso o
  usuário deseje embaralhar as cartas.
 */
void embaralha(struct jogo *partida){
  int tmp;
  if(partida -> embaralhamentos == 0){
    printf("Você já usou todos os 5 embaralhamentos possíveis!\n");
    printf("Pressione ENTER para continuar.\n");
    while ( getchar() != '\n' );
    return;
  }
  tmp = embaralha_periferia(partida);
  if(tmp < 0){
    fprintf(stderr, "Sem memória disponível. Abortando jogo.\n");
    exit(1);
  }
  partida -> embaralhamentos --;
  printf("Feito! Você ainda tem %d embaralhamentos disponíveis.\n",
         partida -> embaralhamentos);
  printf("Pressione ENTER para continuar.\n");
  while ( getchar() != '\n' );
}
