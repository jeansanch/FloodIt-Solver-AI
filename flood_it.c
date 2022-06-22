#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define SIZEX 100
#define SIZEY 100
#define FIL 10
#define NIVEL 3

typedef struct node{
	short tab[100][100];
	short color;
	short score;
	struct node *sons[10];
	struct node *pai;
} Node;

void liberaResto(Node *node, bool flag, short ignore);
void resetaMapa();
void geraFilhos(Node *node, short limit, short step);
void modifica(Node *node, short x, short y, short oldcolor, short color);
short pontuacao(Node *node, short x, short y, short cor);
bool verifica(Node *node);
void DEBUG_printaMapa(Node *node);

Node *max = NULL;
bool mapa[100][100] = {};

int main(){
// DEBUG
//	int testex, testey;
	short listaCores[1000];
	short listaSize = 0;
	short i = 0, j = 0;
	short ignore1, ignore2, ignore3;
	Node *root = malloc(sizeof(Node));
	scanf("%hd %hd %hd", &ignore1, &ignore2, &ignore3);
	//--------------------- TRATADOR DO INPUT ---------------------
	int cntrRead, cntrWord, reset, numero;
	char line[1024] = {'\0'};
	char lineToName[1024] = {'\0'};
	//leitura linha por linha preenchida no stdin
	while (fgets(line, sizeof (line), stdin)!=NULL){
		// procura por palavras
		for(cntrRead=0; line[cntrRead]!='\n' && line[cntrRead]!='\0'; cntrRead++){
			cntrWord=0;
			//separa as palavras
			while(line[cntrRead] != '\0' && line[cntrRead] != ' ' && line[cntrRead] != '\n'){
				lineToName[cntrWord] = line[cntrRead];
				cntrWord++;
				cntrRead++;
			}
			numero = atoi(lineToName);
			root->tab[i][j] = numero;
			if(i+1>=SIZEX){
				i = 0;
				j++;
			}
			else
				i++;
				
			//Resetando o leitor de palavras
			for(reset = 0; reset < cntrWord; reset++){
				lineToName[reset] = '\0';
			}
		}
		//Devido a aparentemente um bug no fgets, estou zerando a leitura dele antes de fazer a próxima leitura
		int resetFgets;
		for(resetFgets=0;resetFgets!=cntrRead;resetFgets++){
			line[resetFgets] = '\0';
		}
	}
	fclose(stdin);	
	root->color = root->tab[0][0];
	root->pai = NULL;
	root->score = pontuacao(root, 0, 0, root->tab[0][0]);
	resetaMapa();
	bool flag = false;
	
	flag = verifica(root);
	
	while(!flag){
		geraFilhos(root, NIVEL, 0);
		flag = verifica(max);
		if(flag){
//			listaCores[listaSize] = max->pai->pai->pai->color;
//			printf("DEBUG: %hd\n", max->pai->pai->pai->color);
//			DEBUG_printaMapa(max->pai->pai->pai);
			// listaSize++;
			listaCores[listaSize] = max->pai->pai->color;
//			printf("DEBUG: %hd\n", max->pai->pai->color);
//			DEBUG_printaMapa(max->pai->pai);
			listaSize++;
			listaCores[listaSize] = max->pai->color;
//			printf("DEBUG: %hd\n", max->pai->color);
//			DEBUG_printaMapa(max->pai);
			listaSize++;
			listaCores[listaSize] = max->color;
//			printf("DEBUG: %hd\n", max->color);
//			DEBUG_printaMapa(max);
			listaSize++;
		}
		else{
			root = max->pai->pai;
			listaCores[listaSize] = root->color;
			listaSize++;
//			printf("DEBUG: %hd\n", root->color);
//			DEBUG_printaMapa(root);
			liberaResto(root->pai, true, root->color-1);
			for(i = 0; i < FIL; i++){
				liberaResto(root->sons[i], false, 0);
			}
			max = NULL;
		}
	}
	
	printf("%d\n", listaSize);
	for(i = 0; i < listaSize-1; i++){
		printf("%d ", listaCores[i]);
	}
	printf("%d", listaCores[listaSize-1]);
//DEBUG - Garantindo a inserção dos dados
//	for(testey = 0; testey < SIZEY; testey++){
//		for(testex = 0; testex < SIZEX; testex++){
//			printf("%d ", root->tab[testex][testey]);
//		}
//		printf("\n");
//	}
	
	//Para cada nivel da arvore realiza tal ação

	
	//Verifica quem possui a maior pontuação
	
	
	//Retorna cor do caminho selecionado
	return 0;
}

void DEBUG_printaMapa(Node *node){
	int i,j;
	for(i = 0; i < SIZEX; i++){
		for(j=0; j < SIZEY; j++){
			if(node->tab[i][j] < 10)
				printf("0%hd ", node->tab[i][j]);
			else
				printf("%hd ", node->tab[i][j]);
		}
		printf("\n");
	}
}

void liberaResto(Node *node, bool flag, short ignore){
	int i;
	if(node != NULL){
		if(flag){
			if(node->sons != NULL){
				for(i = 0; i < ignore; i++){
					liberaResto(node->sons[i], false, 0);
				}
				for(i = ignore+1; i < FIL; i++){
					liberaResto(node->sons[i], false, 0);
				}
			}
		}
		else{
			if(node->sons != NULL){
				for(i = 0; i < FIL; i++){
					liberaResto(node->sons[i], false, 0);
				}
			}
		}
	free(node);
	}
}

void resetaMapa(){
	int i, j;
	for(i = 0; i < SIZEX; i++){
		for(j = 0; j < SIZEY; j++){
			mapa[i][j] = false;
		}
	}
}

void geraFilhos(Node *node, short limit, short step){
	int i, j, k;
	if(step < limit){
		for(i=0; i<FIL; i++){
			node->sons[i] = malloc(sizeof(Node));
			for(j = 0; j < SIZEX; j++){
				for(k = 0; k < SIZEY; k++){
					node->sons[i]->tab[j][k] = node->tab[j][k];
					node->sons[i]->color = i+1;
				}
			}
			node->sons[i]->pai = node;
			//chamar só se for o ultimo.
			if(step+1 == limit){
				node->sons[i]->score = pontuacao(node->sons[i], 0, 0, i+1);
				resetaMapa();
				if(max == NULL){
					max = node->sons[i];
				}
				else{
					if(node->sons[i]->score > max->score){
						max = node->sons[i];
					}
				}
				for(i = 0; i < FIL; i++){
					node->sons[i] = NULL;
				}
			}
			else{
				node->sons[i]->score = pontuacao(node->sons[i], 0, 0, i+1);
				resetaMapa();
				geraFilhos(node->sons[i], limit, step+1);			
			}
		}
	}
}

void modifica(Node *nodem, short x, short y, short oldcolor, short color){
	if(x == 0 && y == 0)
		oldcolor = nodem->tab[0][0];
	nodem->tab[x][y] = color;
	if(x-1 >= 0){
		if(nodem->tab[x-1][y] == oldcolor){
			modifica(nodem, x-1, y, oldcolor, color);
		}
	}
	if(x+1 < SIZEX){
		if(nodem->tab[x+1][y] == oldcolor){
			modifica(nodem, x+1, y, oldcolor, color);
		}
	}
	if(y-1 >= 0){
		if(nodem->tab[x][y-1] == oldcolor){
			modifica(nodem, x, y-1, oldcolor, color);
		}
	}
	if(y+1 < SIZEY){
		if(nodem->tab[x][y+1] == oldcolor){
			modifica(nodem, x, y+1, oldcolor, color);
		}
	}
}

short pontuacao(Node *node, short x, short y, short cor){
	if(x == 0 && y == 0){
		if(node->tab[x][y] != cor){
			modifica(node, 0, 0, 0, cor);
		}
	}
	if(x < SIZEX && 0 <= x && y < SIZEY && 0 <= y){
		if(mapa[x][y] == false){
			if(node->tab[x][y] == cor){
				mapa[x][y] = true;
				return 1+pontuacao(node, x+1, y, cor)+pontuacao(node, x-1, y, cor)+pontuacao(node, x, y-1, cor)+pontuacao(node, x, y+1, cor);
			}
		}
	}
	return 0;
}

bool verifica(Node *node){
	int i, j;
	for(i = 0; i < SIZEX; i++){
		for(j = 0; j< SIZEY; j++){
			if(node->tab[0][0] != node->tab[i][j]){
				return false;
			}
		}
	}
	return true;
}

