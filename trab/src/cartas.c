#include <stdio.h>
#include <stdlib.h>
#include "cartas.h"

/***********************************************************************
Este arquivo contém todas as funções usadas lidar com operações gerais
de cartas de baralho. Como representar uma pilha de cartas, embaralhar
pilhas de cartas, representar cartas, obter o valor delas, retirar e
colocar cartas em pilhas.
***********************************************************************/


/* Uma carta é um dentre 104 números diferentes. O bit menos
   significativo define de qual baralho a carta foi tirada (0 ou
   1). Os próximos dois bits menos significativos armazenam o naipe da
   carta segundo a lógica 0:Ouros, 1:Espadas, 2:Copas, 3:Paus. os
   próximos 4 bits menos significativos possuem o valor numérico da
   carta entre 1 e 13 (os demais valores são inválidos). Os demais
   bits não são considerados relevantes */

/* Funções que obtém dados das cartas */
// O número do baralho é o último bit: ...............x
int carta_baralho(int carta){
  return carta % 2;
}
// O número do naipe é de 0 à 3 e são os dois penúltimos bits:
// .............xx.
int carta_naipe(int carta){
  return (carta / 2) % 4;
}
// E os próximos 4 bits menos signifiicativos representa o valor
// numérico da carta: .........xxxx...
int carta_valor(int carta){
  return (carta / 8) % 16;
}

/* Função que gera carta dado o número de baralho, o naipe e o valor*/
int carta(int baralho, int naipe, int valor){
  return baralho + naipe * 2 + valor * 8;
}

/* Gera pilha de cartas vazia. Em caso de erro retorna NULL. */
struct pilha_de_cartas *gera_pilha_de_cartas_vazia(void){
  struct pilha_de_cartas *nova_pilha;
  nova_pilha = (struct pilha_de_cartas *)
    malloc(sizeof(struct pilha_de_cartas));
  if(nova_pilha != NULL){
    nova_pilha -> tamanho = 0;
    nova_pilha -> pilha = (int *) malloc(sizeof(int));
    if(nova_pilha -> pilha != NULL)
      nova_pilha -> pilha[0] = 0;
    else{
      free(nova_pilha);
      nova_pilha = NULL;
    }
  }
  return nova_pilha;
}

/* Gera uma pilha de cartas inicial com cartas usando dois baralhos e
   valores de cartas do valor 'inicio' até o 'fim'. Por exemplo:
   gera_pilha_de_cartas(AS, AS) -> Gera uma pilha de cartas com 8 Ás,
   2 de cada naipe. Enquanto gera_pilha_de_cartas(AS, REI) gera 104
   cartas, duas cópias de cada carta de um baralho completo.
*/
struct pilha_de_cartas *gera_pilha_de_cartas_inicial(int inicio,
                                                     int fim){
  // Alocando o espaço:
  int baralho, naipe, valor, i;
  struct pilha_de_cartas *nova_pilha;
  nova_pilha = (struct pilha_de_cartas *)
    malloc(sizeof(struct pilha_de_cartas));
  if(nova_pilha != NULL){
    nova_pilha -> tamanho = 2 * 4 * (fim - inicio + 1);
    nova_pilha -> pilha = (int *) malloc(sizeof(int) *
                                         (nova_pilha -> tamanho + 1));
    if(nova_pilha -> pilha == NULL){
      free(nova_pilha);
      nova_pilha = NULL;
    }
  }
  // Preenchendo a pilha:
  if(nova_pilha != NULL){
    i = 0;
    for(baralho = 0; baralho < 2; baralho ++)
      for(naipe = 0; naipe <= PAUS; naipe ++)
        for(valor = inicio; valor <= fim; valor ++){
          nova_pilha -> pilha[i] = baralho + naipe * 2 +
            valor * 8;
          i ++;
        }
    nova_pilha -> pilha[nova_pilha -> tamanho] = 0;
  }
  return nova_pilha;
}

/* Dada uma pilha de cartas, embaralha ela */
void embaralha_pilha_de_cartas(struct pilha_de_cartas *pilha){
  int i, temp, nova_posicao;
  for(i = 0; i < pilha -> tamanho; i ++){
    nova_posicao = rand() % pilha -> tamanho;
    temp = pilha -> pilha[nova_posicao];
    pilha -> pilha[nova_posicao] = pilha -> pilha[i];
    pilha -> pilha[i] = temp;
  }
}

/* Retira a carta do topo do baralho e a retorna. Em caso de erro,
   retornamos 0. Em caso de pilha de cartas vazia, retornamos 0. Caso
   haja erro de realocação de memória, retornamos -1.
 */
int retira_carta_topo(struct pilha_de_cartas *pilha){
  int carta, *tmp;
  if(pilha->tamanho == 0)
    return 0;
  carta  = pilha -> pilha[pilha -> tamanho - 1];
  pilha -> pilha[pilha -> tamanho - 1] = 0;
  tmp = (int *) realloc(pilha -> pilha,
                        sizeof(int) * pilha -> tamanho);
  if(tmp == NULL){
    return -1;
  }
  pilha -> pilha = tmp;
  pilha -> tamanho --;
  return carta;
}

/* Dada uma pilha de cartas e uma carta, coloca a carta dada no topo
   da pilha. Depois, retornamos o valor da carta colocada. Retornamos
   -1 se ocorreu erro de realocação de memória. Neste caso,
   desalocamos a pilha.*/
int coloca_carta_topo(struct pilha_de_cartas *pilha, int carta){
  int *tmp;
  tmp = (int *) realloc(pilha -> pilha,
                        sizeof(int) * (pilha -> tamanho + 2));
  if(tmp == NULL){
    return -1;
  }
  pilha -> pilha = tmp;
  pilha -> pilha[pilha -> tamanho] = carta;
  pilha -> tamanho ++;
  pilha -> pilha[pilha -> tamanho] = 0;
  return carta;
}

/* Função usada para desalocar uma pilha de cartas. */
void destroi_pilha_de_cartas(struct pilha_de_cartas *pilha){
  free(pilha -> pilha);
  free(pilha);
}

/* Transfere cartas de uma pilha para a outra. Ela cuida da tarefa de
   desalocar todo o espaço de ambas as pilhas se a transferência
   falhar por erro de realocação de memória. Retorna a carta
   transferia ou -1 caso tenha havido erro de realocação.*/
int transfere_carta(struct pilha_de_cartas *origem,
                    struct pilha_de_cartas *destino){
  int carta = retira_carta_topo(origem);
  if(carta == -1){
    return -1;
  }
  carta = coloca_carta_topo(destino, carta);
  if(carta == -1){
    return -1;
  }
  return carta;
}

/* Função usada para imprimir na saída padrão uma representação
   gráfica da carta */
void imprime_carta(int carta){
  switch(carta_valor(carta)){
  case 1:
    printf(" A");
    break;
  case 11:
    printf(" J");
    break;
  case 12:
    printf(" Q");
    break;
  case 13:
    printf(" K");
    break;
  default:
    printf("%02d", carta_valor(carta));
    break;
  }
  putchar('-');
  switch(carta_naipe(carta)){
  case 0:
    putchar('O');
    break;
  case 1:
    putchar('E');
    break;
  case 2:
    putchar('C');
    break;
  default:
    putchar('P');
    break;
  }
}

/* Função que imprime na saída padrão uma representação gráfica de uma
   pilha de cartas. Basicamente imprime a carta do topo da pilha. */
void imprime_pilha_de_cartas(struct pilha_de_cartas *pilha){
  if(pilha -> tamanho == 0)
    printf("    ");
  else
    imprime_carta(pilha -> pilha[pilha -> tamanho - 1]);
}

/* Função que retorna a carta que está no topo de uma pilha */
int topo_da_pilha(struct pilha_de_cartas *pilha){
  if(pilha -> tamanho == 0)
    return 0;
  return pilha -> pilha[pilha -> tamanho - 1];
}
