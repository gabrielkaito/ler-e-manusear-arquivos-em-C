#include <stdio.h>
#include <conio2.h>
#include <windows.h>


struct TPfilmes{
	char Nome[60], Classificacao[7];
	int Producao, Duracao;
	float Gasto, Renda, Espec;
};

void ARQUIVOBINARIO(void){
	FILE *ponteiro = fopen("Filmes2.txt","r");
	
	TPfilmes FILMES;
	if(ponteiro == NULL)
		printf("\nArquivo de filmes inexistente!\n");
	else{
		FILE *ponteiro2 = fopen("FilmesBin.dat", "wb+");
		rewind(ponteiro);
		fscanf(ponteiro, "%[^;];%d;%[^;];%f;%f;%d;%f\n", &FILMES.Nome, &FILMES.Producao, &FILMES.Classificacao, &FILMES.Gasto, &FILMES.Renda, &FILMES.Duracao, &FILMES.Espec);
		while(!feof(ponteiro)){
			fwrite(&FILMES, sizeof(TPfilmes),1,ponteiro2);
			fscanf(ponteiro, "%[^;];%d;%[^;];%f;%f;%d;%f\n", &FILMES.Nome, &FILMES.Producao, &FILMES.Classificacao, &FILMES.Gasto, &FILMES.Renda, &FILMES.Duracao, &FILMES.Espec);
		}
		fclose(ponteiro);
		fclose(ponteiro2);
	}
}

void MAIORLUCRO(void){
	FILE *ponteiro = fopen("FilmesBin.dat", "rb");
	
	TPfilmes FILMES;
	char Nome[60];
	float Maior = 0, Lucro;
	
	clrscr();
	rewind(ponteiro);
	fread(&FILMES, sizeof(TPfilmes),1,ponteiro);
	while(!feof(ponteiro)){
		Lucro = FILMES.Renda - FILMES.Gasto;
		if( Lucro > Maior){
			Maior = Lucro;
			strcpy(Nome, FILMES.Nome);
		}
		fread(&FILMES, sizeof(TPfilmes),1,ponteiro);
	}
	printf("\n**** FILME COM MAIOR LUCRO ****");
	printf("\n- Nome: %s", Nome);
	printf("\n- Lucro: %.3f", Maior);
	
	fclose(ponteiro);
	getch();
}

void MAIORTAXAESPEC(void){
	FILE *ponteiro = fopen("FilmesBin.dat","rb");
	TPfilmes FILMES;
	float Maior = 0;
	char Nome[60];
	
	clrscr();
	rewind(ponteiro);
	fread(&FILMES, sizeof(TPfilmes),1,ponteiro);
	while(!feof(ponteiro)){
		if (FILMES.Espec > Maior){
			Maior = FILMES.Espec;
			strcpy(Nome, FILMES.Nome);
		}
		fread(&FILMES, sizeof(TPfilmes),1,ponteiro);
	}
	printf("\n**** FILME COM MAIOR TAXA DE ESPECTADORES ****");
	printf("\n- Nome: %s", Nome);
	printf("\n- Taxa: %.1f", Maior);
	
	fclose(ponteiro);
	getch();
}

void DURACAOMEDIA (void){
	FILE *ponteiro = fopen("FilmesBin.dat","rb");
	
	TPfilmes FILMES;
	int Soma = 0, Media;
	int Cont = 0;
	
	clrscr();
	rewind(ponteiro);
	fread(&FILMES, sizeof(TPfilmes),1,ponteiro);
	while (!feof(ponteiro)){
		Soma = Soma + FILMES.Duracao;
		Cont++;
		
		fread(&FILMES, sizeof(TPfilmes),1,ponteiro);
	}
	Media = Soma/Cont;	
	printf("\n**** DURACAO MEDIA DOS FILMES ****");
	printf("\n- %d minutos", Media);

	fclose(ponteiro);
	getch();
}

void ANOMAISFILMES(void){	
	//inserir as estruturas ordenadas em outro arquivo, fazer um while com contador !feof(ponteiro) || ano de producao for igual, 
	//se o ano for diferente começa com outro contador e vai indo, guardo o valor apenas do maior
	FILE *ponteiro = fopen("FilmesBin.dat","rb");
	FILE *ponteiro2 = fopen("FilmesTemp.dat","wb+");
	
	TPfilmes FILMES;
	TPfilmes FILMESAUX;
	int aux, ano, i, maior = 0, anomaior;
	
	clrscr();
	rewind(ponteiro);
	fread(&FILMES, sizeof(TPfilmes),1,ponteiro);
	
	while(!feof(ponteiro)){
		if(ponteiro2 == NULL)
			fwrite(&FILMES, sizeof(TPfilmes),1,ponteiro2);
		else{
			fseek(ponteiro2,0,2);
			fwrite(&FILMES, sizeof(TPfilmes),1,ponteiro2);
			aux = (ftell(ponteiro2)/sizeof(TPfilmes)) - 2;
			
			fseek(ponteiro2,aux*sizeof(TPfilmes),0);
			fread(&FILMESAUX, sizeof(TPfilmes),1,ponteiro2);
			
			while(aux >= 0 && FILMES.Producao < FILMESAUX.Producao){
				fseek(ponteiro2,aux*sizeof(TPfilmes),0);
				fwrite(&FILMES, sizeof(TPfilmes),1,ponteiro2);
				fwrite(&FILMESAUX, sizeof(TPfilmes),1,ponteiro2);
					
				aux--;
				fseek(ponteiro2,aux*sizeof(TPfilmes),0);
				fread(&FILMESAUX, sizeof(TPfilmes),1,ponteiro2);
				fread(&FILMES, sizeof(TPfilmes),1,ponteiro2);
			}		
		}
		
		fread(&FILMES, sizeof(TPfilmes),1,ponteiro);
	}
	fclose(ponteiro);
	
	rewind(ponteiro2);
	fread(&FILMES, sizeof(TPfilmes),1,ponteiro2);
	ano = FILMES.Producao;
	aux = 0;
		

	// preciso colocar o aux pra maior e ano para anomaior		
	while(!feof(ponteiro2)){
		if (FILMES.Producao == ano){
			fread(&FILMES, sizeof(TPfilmes),1,ponteiro2);	
			aux++;
		}
		else{
			printf("\nAno: %d", ano);
			printf("\nQuantidade: %d\n", aux);
			
			ano = FILMES.Producao;
			fread(&FILMES, sizeof(TPfilmes),1,ponteiro2);	
			aux = 1;
		}
	}
	printf("\nAno: %d", ano);
	printf("\nQuantidade: %d\n", aux);
}


int MENU(void){
	char op;
	
	clrscr();
	printf("\n(A) - FILME COM MAIOR LUCRO");
	printf("\n(B) - FILME COM MAIOR TAXA DE ESPECTADORES");
	printf("\n(C) - DURACAO MEDIA DOS FILMES");
	printf("\n(D) - ANO COM O MAIOR NUMERO DE FILMES PRODUZIDOS\n");
	
	fflush(stdin);
	scanf("%c", &op);
		
	return toupper(op);
}


void EXECUTAR(void){
	char op;
	
	clrscr();
	do{
		op = MENU();
		switch(op){			
				case 'A': MAIORLUCRO();
						break;
				
				case 'B': MAIORTAXAESPEC();
						break;
				
				case 'C': DURACAOMEDIA();
						break;
				
				case 'D': ANOMAISFILMES();
						break;
		}		
	} while(op != 27);
		
}




int main(void){
	
	ARQUIVOBINARIO();
	
	EXECUTAR();	
	
}
