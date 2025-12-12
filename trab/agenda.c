#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define maxcontato 150
#define maxlinha 151

void imprime(char *pont);
void insere(char arquivo[],int index);
void modifica(char *arquivo,int index);
void f_busca(char *arquivo, char pal[]);
void deleta(char *arquivo, int a);
int fnum(char *arquivo,int seletor);
void le_string(char *aux);
void menu(char *arq);
int  num_int(int a);

int main(int argc, char *argv[])
  {
       int cont=0,num_arq;
       char *bufer;
       FILE *arqb;
       FILE *arq = fopen(argv[1],"r");
      if (argc!=2)
          {
              printf("Parametros invalidos, voce nao passou o nome do arquivo.\n");
              printf("Digite o nome de um arquvo um arquvo ou digite 0 para sair.\n");
              scanf("%s",bufer);
              if (bufer[0]=='0')
                 return -1;
              else
                 {
		    arqb = fopen(bufer,"w");   
                    cont=1;
                    fclose (arqb);
                  }
           }
        if (arq==NULL)
          {
               printf("Parametros invalidos, voce passou um arquivo nao existente.\n");
               printf("Deseja criar um arquivo com o nome digitado?\nSe sim digite 's', ao contrario digite 'n'\n");
               char criar;
               scanf("%c",&criar);
               while(criar!='s'&&(criar)!='n')
                  scanf("%c",&criar); 
               if (criar=='s')
                  {
                       strcpy (bufer,argv[1]);
		       arqb = fopen(bufer,"w");   
                       cont=1;
                  }
               else
                  return -1;
          }
       if (cont==0)
 	    {
                num_arq=strlen(argv[1]);
                bufer=malloc((num_arq+1)*sizeof(char));
                strcpy (bufer,argv[1]);
                bufer[num_arq+1]='\0';
                
           }
       fclose (arq);    
       int index, op = 0;
       system("clear");
       while (op != 6)
	    {
                  inicio:
                  printf("\n   --Escolha uma das seguintes opicoes--\n\n");
	  	  printf("[1] : Para mostrar agenda \n");
		  printf("[2] : Para buscar informacoes de um contato \n");
		  printf("[3] : Para modificar contato \n");
	  	  printf("[4] : Para inserir contato \n");
		  printf("[5] : Para excluir contato \n");
		  printf("[6] : Para sair \n\n");
		  scanf("%i", &op);
	  	  switch(op)
	  	    {
	  	   	  case 1:
	  	   	 	 {
	  	   	         imprime(bufer);
	  	   	 	 }break;
	  	   	  case 2:
				 {
				    char busca[40];
				    printf("Digite o que deseja buscar na agenda (nome, num. telefone ou e-mail).\n");
				    scanf("%s", busca);
				    f_busca(bufer, busca);
				 }break;
	  	   	  case 3:
			        {
				    menu(bufer); 
                                    printf("Digite o numero indexador do usuario a ser modificado:\n");
      			            scanf("%i", &index);
                                    if (index<1)
                                        printf("O indexador deve ser um inteiro maior que 0");  
                                    while(index<1)
                                        scanf("%i", &index);
				    insere(bufer,index);
                                    goto inicio;
				 }
	  	   	  case 4:
	  	   	 	 {
	  	   	 	    insere(bufer,0);
	  	   	 	 }break;
	  	   	  case 5:
	  	   	 	 {
                                    menu(bufer);
				    getchar(); 
				    printf("Insira o numero indexador do usuario a ser deletado:\n");
                                    int z;
                                    scanf("&i",&z);
				    deleta(bufer,z);
	  	   	 	 }break;
	  	    }
	   }        
         printf("O numero de contatos e' = %i.\n",fnum(bufer,1));
         return 0;
  }


void insere(char arquivo[],int index)
{
    int numc, i = 0,num_index;
    getchar();
    char aux,nome[60],numt[40],e_mail[50],aux5[50],*preenche,comfirma;
    printf("Digite o nome e sobrenome seguido de ENTER \n");
    le_string(nome);
    printf("Digite o numero de telefone Ex: '(41)9876-5431' seguido de ENTER \n");
    le_string(numt);
    printf("Digite o E-mail Ex: 'joao@gmail.com' seguido de ENTER \n");
    le_string(e_mail); 
    printf("Digite 's' se realmente deseja salvar as alteracoes, ao contrario digite 'n' \n");
    scanf("%c",&comfirma);
    while(comfirma!='s'&&comfirma!='n')
         scanf("%c",&comfirma);
    if (comfirma=='s')
        {
            FILE *arq;
            arq = fopen(arquivo,"r+b");
            numc=5+strlen (nome)+strlen (numt)+strlen (e_mail)+num_int(fnum(arquivo,1));
            preenche=malloc((maxcontato-numc)*sizeof(char));
	    i=0;
            while (numc<=maxcontato)
               {
                   numc++;
                   preenche[i]=' ';
                   i++;
               }
            preenche[numc]='\0';
            if (index!=0)
               {
                   fseek (arq,(index -1)*(maxcontato+1),SEEK_SET);
                   num_index=index;
               }  
            else
               {
		    fseek (arq,0,SEEK_END);
		    num_index=(fnum(arquivo,0)+1);
	       }  
            fprintf(arq,"\n%i:%s:%s:%s%s",num_index,nome,numt,e_mail,preenche);
            fclose (arq);
			
			
			/*
			Tentei inserir encima de um contato deletado 
			mas n�o deu serto:
			
			FILE *arq;
            arq = fopen(arquivo,"r+b");
            numc=4+strlen (nome)+strlen (numt)+strlen (e_mail)+num_int(fnum(arquivo,1));
            preenche=malloc((maxcontato-numc)*sizeof(char));
	    i=0;
            while (numc<maxcontato)
               {
                   numc++;
                   preenche[i]=' ';
                   i++;
               }
            preenche[numc]='\0';
            if (index!=0)
             {
                 fseek (arq,(index -1)*(maxlinha),SEEK_SET);
                 fprintf(arq,"%i:%s:%s:%s%s\n",index,nome,numt,e_mail,preenche);
             }
            else
			     {
			         rewind(arq);
			         char frase[maxlinha];
			         int w=0,linha=0;
			         while (fgets(frase, maxlinha, arq) != NULL)
                       {
                            w++;
							if(frase[0]!='-')
                                linha=w;
                       }
                     fclose (arq);
					 if (linha==0)
                        {
                             arq = fopen(arquivo,"a");
                             fprintf(arq,"%i:%s:%s:%s%s\n",fnum(arquivo,0),nome,numt,e_mail,preenche);
                             fclose (arq);
                        }
					 else
					    {
					        arq = fopen(arquivo,"r+b");
							fseek (arq,(linha -1)*(maxlinha),SEEK_SET);
					        fprintf(arq,"%i:%s:%s:%s%s\n",linha,nome,numt,e_mail,preenche);
					        fclose (arq);
					    }
			     }*/     
         }
}

void imprime(char *nome)
{
    system("clear");
    char frase[maxcontato+1];
    FILE *arq;
    arq = fopen(nome,"r");
    printf("\n");
	while (fgets(frase, maxlinha, arq) != NULL)
      {
    	 if((frase[0]!='-')&&(frase[0]!='\n'))
           printf("%s",frase);
      }
    printf("\n");
    fclose(arq);
}

int fnum(char *arquivo,int seletor)
{
    int i=0;
    char a[maxcontato];
    FILE *arq;
    arq = fopen(arquivo,"r");
    while (fgets(a, maxlinha+1, arq) != NULL)
       {
	     if(((a[0]!='-')||(seletor==0))&&(a[0]!='\n'))
              ++i; 
       }
    fclose(arq);
    return i;
}

void f_busca(char *arquivo, char *pal)
{
    int i,num;
    char a[maxcontato],aux,aux2[maxcontato];
    FILE *arq;
    arq = fopen(arquivo,"r");
    num=strlen(pal);
    for(i=0;i<=num;i++)
        {
            aux=pal[i];
            pal[i]=tolower(aux);
        }

    while (fgets(a, maxcontato, arq) != NULL)
      {
	   i=0;
           num=strlen(a);
	   for(i=0;i<=maxcontato;i++)
	   	aux2[i]=a[i];  
	   for(i=0;i<=maxcontato;i++)
              {
                    aux=a[i];
                    a[i]=tolower(aux);
                    i++;
              }       
           if (strstr(a, pal)!= NULL)
	      {
	           if(a[0]!='-')
		      {
			    printf("%s\n",aux2);
		      }
	      }
      }
    fclose(arq);
}

void deleta(char *arquivo,int index)
{
    int i;
    FILE *arq;
    arq = fopen(arquivo,"r+b");
    fseek (arq,(index)*(maxlinha),SEEK_SET);
    fprintf(arq,"%c",'-');
    fclose (arq);
}

int  num_int(int a)
{
    int cont=1;
    a/=10;
    while (a!=0)
      {
          cont++;
          a/=10;
      }
    return cont;
}

void le_string(char *aux)
{
     int i=0;
     char aux1;
     aux1=getchar();
     while (aux1 != '\n')
       {
          aux[i]=aux1;
	  ++i;
	  aux1=getchar();
       }
     aux[i]='\0';
}

void menu(char *arq)
{
      getchar();
      printf("Deseja consultar a agenda?\n:");
      printf("Se deseja imprimir digite : i\n");
      printf("Se deseja pesquisar digite : b\n"); 
      printf("Ao contrario digite : n\n");
      char comfirma;
      scanf("%c",&comfirma);
      while(comfirma!='i' && comfirma!='b' && comfirma!='n')
          scanf("%c",&comfirma);
      if (comfirma=='i')
	      imprime(arq);
      if (comfirma=='b')
          {
              char *busca;
              scanf("%s",busca); 
              f_busca(arq,busca);
          }
}
