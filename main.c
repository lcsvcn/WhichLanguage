#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PAL 7000
#define LOG 0
#define DADOS_RELEVANTES 1

struct palavras {
	char str[50];
	int pos;
};

struct dicionario {
	char lingua[50];
	char arquivo[50];
	int cont_ocorr;
	struct palavras PAL[MAX_PAL];
	int fim_das_palavras;
};


int main(int argc, char *argv[]) {
	FILE *arq_indentificado;
	FILE *arq_conf;
 	int num_linhas = 0;
    int i, j, k;
    char str[50];
    char arquivo[50];
    char arq_entrada[50];

    if(argc < 2) {
    	printf("guesslang <nome_arquivo_entrada>\n");
    	return 1;
    }

    strcpy(arq_entrada, "guesslang/");
    strcat(arq_entrada, argv[1]);
    
    if(LOG) printf("%s\n", arq_entrada);

    // adiciona diretorio mais nome do arquivo
	arq_indentificado = fopen(arq_entrada, "r");

	// se deu erro ao criar o arquivo
	if (arq_indentificado == NULL) {
	    printf("Nao existe arquivo que foi pedido\n");
	    return 1;
	} 

	// abre o arquivo de configuracao
	arq_conf = fopen("guesslang/guesslang.conf", "r");

	// se deu erro ao criar o arquivo
	if (arq_conf == NULL) {
	    printf("Arquivo de configuracao inexistente\n");
	    return 1;
	}	



  	// pega espaco ou \n
  	char lixo[100];

	// percorre o arquivo de configuracao para saber quantas linhas tem e portanto quantos dicionario terá
	for(i = 0; !feof(arq_conf); i++) {
		if(fscanf(arq_conf, "%s : %s", lixo, lixo) == EOF) {
			// como comeca do zero, o numero de linhas eh i + 1
			num_linhas = i;
			break;
		}
  	}

    // volta ao inicio do arquivo agora que sabe o numero de linhas
    rewind(arq_conf);

    // cria o dicionario com o tamanho deles no arquivo conf
    struct dicionario DIC[num_linhas+1];
    FILE *arq_dicionario[num_linhas+1];

	//zera o numero de ocorrencias de cada lingua
	for(i = 0; i < num_linhas; i++) {
		DIC[i].cont_ocorr = 0;
	}


	// agora le novamente o arquivo conf pegando a linha e vendo a lingua e seu arq correspondente
   	for(i = 0; !feof(arq_conf); i++) {
     	fscanf(arq_conf, "%s : %s", DIC[i].lingua, arquivo);
     	strcpy(DIC[i].arquivo, "guesslang/");
     	strcat(DIC[i].arquivo, arquivo);
     	if(LOG) printf("%s\n", DIC[i].arquivo);	
  	}

  	if(LOG) printf("num_linhas = %d\n",  num_linhas);

  	// percorre o numero de linguas e arq e vai lendo um por um
  	for(i = 0; i < num_linhas; i++) {
  		arq_dicionario[i] = fopen(DIC[i].arquivo, "r");

		for(j=0; ;j++) {
			if(fscanf(arq_dicionario[i], "%[a-zA-Z'1-9]", str) == EOF) {
				break;
			}
			strcpy(DIC[i].PAL[j].str, str);

    		if(fscanf(arq_dicionario[i], "%[^a-zA-Z'1-9]", lixo) == EOF) {
    			break;
    		}

    		if(LOG) printf("(%s)", DIC[i].PAL[j].str);
    		
		}

		if(LOG) printf("\n");

		DIC[i].fim_das_palavras = j; // armazena o numero de palavras percorridas no dicionario atual
	}

	
	// ler o arquivo a ser identificado e ver o numero de ocorrencias em cada dicionario
	// depois ver qual dos dicionario tem maior ocorrencia e imprimir a lingua
	for(i = 0; !feof(arq_indentificado); i++) {
  		fscanf(arq_indentificado, "%s", str);
		for(j = 0; j < num_linhas; j++) {
			for(k = 0; k < DIC[j].fim_das_palavras; k++) {
				 if(strcasecmp(DIC[j].PAL[k].str, str) == 0) {
				 	DIC[j].cont_ocorr++;
				 	j++;		// vai pro prox dicionario
				 }
			}
		}
	}


	int maior_ocorr = -1;
	int pos = -1;

	for(i = 0; i < num_linhas; i++) {
		if(DIC[i].cont_ocorr > maior_ocorr) {
			maior_ocorr = DIC[i].cont_ocorr;
			pos = i;
		}
	}
	
	if(DADOS_RELEVANTES) {
		for(i = 0; i < num_linhas; i++) {
			printf("%s %d\n", DIC[i].lingua, DIC[i].cont_ocorr);
		
		}
	}
	// pos sera a que teve maior numero de ocorrencias no texto
	printf("A lingua predominante é: %s\n", DIC[pos].lingua);

  	fclose(arq_indentificado);	
  	fclose(arq_conf);
  	for(i = 0; i < num_linhas; i++) {
  		fclose(arq_dicionario[i]);
  	}

	return 0;
}