// Attax (C/C++) - versão Grafica, 30 de Dezembro de 2014
// Luis Paulo Reis - Universidade do Minho, Portuga (lpreis@dsi.uminho.pt)

#include <cstdio>	  //input-output - scanf e printf
#include <cstdlib>	  //standard lib - rand e pause
#include <ctime>	  //ctime - inclui função time
#include "graphics.h" //biblioteca gráfica - WinBGI

#define SIZE 600	  // Tamanho da janela gráfica
#define sq (SIZE / N) // Tamanho da celula do tabuleiro em pixeis
#define NTABS 6		  // Numero de ficheiros com tabuleiros iniciais

struct movimento
{
	int xi, yi, xf, yf, jog, tipo;
};

#ifdef WINDOWS

#define LMOUSE 513
#define RMOUSE 516
#define CMOUSE 519

#else

#define LMOUSE 1
#define RMOUSE 3
#define CMOUSE 5
#define clearmouseclick(A)

#endif

// Estado do Jogo/Tabuleiro
int nMovs = 0, N = 8; // N - tamanho Tabul usado
int tabul[20][20];	  // tabuleiro: 0-vazio, 1-peça jog1, 2-peça jog2, 8-impossivel
int tabcopia[20][20]; // tabuleiro: 0-vazio, 1-peça jog1, 2-peça jog2, 8-impossivel

void copia()
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			tabcopia[j][i] = tabul[j][i];
}
void restaura()
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			tabul[j][i] = tabcopia[j][i];
}

// Dado o jogador atual retorna qual o outro jogador
int outroJog(int jog)
{
	if (jog == 1)
		return 2;
	else
		return 1;
}

// Assinala (ou desassinala tipo=0) uma quadricula x,y
void assinala_quad(int x, int y, int atip)
{
	int col = 15;
	if (atip == 1)
		col = 0;
	else if (atip == 2)
		col = 4;
	setcolor(col);
	setfillstyle(1, col);
	fillellipse(x * sq + 5, y * sq + 5, 4, 4);
	fillellipse(x * sq + sq - 5, y * sq + 5, 4, 4);
	fillellipse(x * sq + 5, y * sq + sq - 5, 4, 4);
	fillellipse(x * sq + sq - 5, y * sq + sq - 5, 4, 4);
}

// Mostra o tabuleiro em modo grafico
void mostra_tabul(void)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			setcolor(1);
			setfillstyle(1, 15);
			if (tabul[i][j] == 8)
				setfillstyle(1, 0);
			bar(j * sq + 1, i * sq + 1, j * sq + sq - 1, i * sq + sq - 1); // quadrado preto/branco
			if (tabul[i][j] == 1 || tabul[i][j] == 2)
			{
				setcolor(tabul[i][j]);
				setfillstyle(1, tabul[i][j]);
				fillellipse(j * sq + sq / 2, i * sq + sq / 2, sq / 2 - 7, sq / 2 - 7); // peca
			}
		}
}

// Lê o jogo do ficheiro (não atualiza o jogador a jogar nem o numero da jogada
void le_jogo(char nome[])
{
	FILE *fich;
	if ((fich = fopen(nome, "r")) == NULL)
	{
		printf("Impossivel ler %s \n", nome);
		return;
	}
	fscanf(fich, "%d", &N);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			fscanf(fich, "%d", &tabul[i][j]);
	fclose(fich);
}

// Grava o estado do jogo num ficheiro
void grava_jogo()
{
	FILE *fich;
	if ((fich = fopen("jogo.txt", "w")) == NULL)
	{
		printf("Impossivel gravar jogo.txt \n");
		return;
	}
	fprintf(fich, "%d\n", N);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			fprintf(fich, "%d ", tabul[i][j]);
		fprintf(fich, "\n");
	}
	fclose(fich);
}
// Inicializa o Tabuleiro lendo sucessivamente os varios tabs de ficheiros
void inicia_tabul()
{
	char nome[9] = "tabX.txt";
	int num = 1;
	do
	{
		nome[3] = num + 48; // Exemplo tab2.txt
		le_jogo(nome);
		cleardevice();
		mostra_tabul();
		delay(800);
		num++;
		if (num > NTABS)
			num = 1;
	} while (!ismouseclick(LMOUSE));
}

// Pede ao utlizador que escolha um dos 4 modos de jogo possiveis:
// 1-Hum/Hum 2-Hum/PC 3-PC/Hum 4-PC/PC
int tipo_jogo(void)
{
	char ch;
	printf("Jogo de Attax\nEscolha o modo de jogo: \n1-Hum/Hum 2-Hum/PC 3-PC/Hum 4-PC/PC\n");
	do
	{
		fflush(stdin);
		ch = getchar();
	} while (ch < '1' || ch > '4');
	return ch - 48;
}

// Finaliza o jogo indicando quem venceu ou se foi empate
void finaliza(int ti, int jog, char venc)
{
	mostra_tabul();
	delay(500);
	cleardevice();
	setcolor(1);
	setbkcolor(0);
	settextstyle(0, 0, 2);
	if (venc == 0)
		outtextxy(SIZE / 2 - 200, SIZE / 2, "Nao Ha' Vencedor! Empate!!!\n");
	else if (venc == 1)
		outtextxy(SIZE / 2 - 200, SIZE / 2, "Venceu o Jogador 1 - Azul!");
	else
		outtextxy(SIZE / 2 - 200, SIZE / 2, "Venceu o Jogador 2 - Verde!");
	delay(2000);
	mostra_tabul();
	delay(3000);
}

// Indica se (x,y) está dentro do tabuleiro
bool dentro(int x, int y)
{
	return (x >= 0 && x <= N - 1 && y >= 0 && y <= N - 1);
}

// Indica se mov se verifica entre duas coordenadas adjacentes a 1 ou 2 de distancia
bool adjacente(movimento mov, int dist)
{
	return (abs(mov.xi - mov.xf) == dist && abs(mov.yi - mov.yf) <= dist ||
			abs(mov.yi - mov.yf) == dist && abs(mov.xi - mov.xf) <= dist);
}

// indica se mov e' um movimento valido e qual o seu tipo
bool movimento_valido(movimento &mov)
{
	if (!dentro(mov.xi, mov.yi) || !dentro(mov.xf, mov.yf))
		return false; // fora do tabuleiro

	if (tabul[mov.yi][mov.xi] == mov.jog && tabul[mov.yf][mov.xf] == 0 &&
		adjacente(mov, 1))
	{
		mov.tipo = 0;
		return true;
	} // expansao da peca

	if (tabul[mov.yi][mov.xi] == mov.jog && tabul[mov.yf][mov.xf] == 0 &&
		adjacente(mov, 2))
	{
		mov.tipo = 1;
		return true;
	} // movimento da peca

	return false;
}

void multiplica(movimento mov)
{
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++)
			if (tabul[mov.yf + dy][mov.xf + dx] == outroJog(mov.jog))
				tabul[mov.yf + dy][mov.xf + dx] = mov.jog;
}

// Executa o movimento mov do tipo tipo
void executa_movimento(movimento mov)
{
	tabul[mov.yf][mov.xf] = mov.jog;
	if (mov.tipo == 1)
		tabul[mov.yi][mov.xi] = 0; // movimento
	multiplica(mov);
}

int conta_pecas(int num)
{
	int cp = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (tabul[i][j] == num)
				cp++;
	return cp;
}

// Avaliação Muito simples 5 pontos para damas e 1 para peças
int avalia(int jog)
{
	return conta_pecas(jog) - conta_pecas(outroJog(jog));
}

// Movimento do Computador - Joga Aleatorio
void jogada_PC(int jog, int n)
{
	movimento mov = {0, 0, 0, 0, jog, 0}, bestmov;
	int bestav = -1000;
	for (int yi = 0; yi < N; yi++)
		for (int xi = 0; xi < N; xi++)
			for (int yf = 0; yf < N; yf++)
				for (int xf = 0; xf < N; xf++) /* recorre todas las posibles jugadas */
				{
					mov.yi = yi;
					mov.xi = xi;
					mov.yf = yf;
					mov.xf = xf;
					if (movimento_valido(mov)) /* comprueba si el mov es valido */
					{
						copia();
						executa_movimento(mov);
						int av = avalia(jog); /* calcula la ventaja */
						if (av >= bestav)
						{
							bestav = av;
							bestmov = mov;
						}
						printf("%d %d -> %d %d (%d): %d", mov.yi, mov.xi, mov.yf, mov.xf, mov.tipo, av);
						restaura();
					}
				}
	executa_movimento(bestmov);
}

// Determina todas as jogadas validas de um jogador
int jogadas_validas(int jog, movimento movs[], int tipoAss)
{
	int nmovs = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			if (tabul[i][j] == jog)
			{
				for (int k = -1; k <= 1; k++)
					for (int l = -1; l <= 1; l++)
					{
						movs[nmovs].jog = jog;
						movs[nmovs].yi = i;
						movs[nmovs].xi = j;
						movs[nmovs].yf = i + k;
						movs[nmovs].xf = j + l;
						if (movimento_valido(movs[nmovs]))
						{
							assinala_quad(j + l, i + k, tipoAss);
							nmovs++;
						}
					}
			}
		}
	return nmovs;
}

// Deteramina todas as jogadas validas de uma dada peça
int jogadas_validas_pos(int jog, int xi, int yi, movimento movs[], int tipoAss)
{
	int nmovs = 0;
	if (tabul[yi][xi] == jog)
	{
		for (int k = 0; k < N; k++)
			for (int l = 0; l < N; l++)
			{
				movs[nmovs].jog = jog;
				movs[nmovs].yi = yi;
				movs[nmovs].xi = xi;
				movs[nmovs].yf = k;
				movs[nmovs].xf = l;
				if (movimento_valido(movs[nmovs]))
				{
					assinala_quad(l, k, tipoAss);
					nmovs++;
				}
			}
	}
	return nmovs;
}

// Verificar se o jogo terminou retornando o vencedor
int fim_jogo(int jog)
{
	movimento movs[500];
	if (jogadas_validas(outroJog(jog), movs, 0) > 0)
		return -1; // existem jogadas
	if (avalia(jog) > 0)
		return jog;
	else if (avalia(jog) <= 0)
		return outroJog(jog);
	else
		return 0; // empate
}

// Jogada do Humano usando o butao esquerdo do rato para seleccionar xi,yi e xf,yf
// O Botao central do rato grava o jogo e o direito lê o jogo anteriormente gravado
void jogada_Humano(int jog)
{
	movimento movs[500], mov = {0, 0, 0, 0, jog};
	int px, py, cl = 0;
	do
	{
		int mc = mouseclick();
		if (mc == RMOUSE)
		{
			le_jogo("jogo.txt");
			cleardevice();
			mostra_tabul();
			clearmouseclick(RMOUSE);
		}
		if (mc == CMOUSE)
		{
			grava_jogo();
			mostra_tabul();
			clearmouseclick(519);
		}
		if (mc == LMOUSE)
		{
			px = mousex() / sq;
			py = mousey() / sq;
			if (cl == 0 && tabul[py][px] == jog)
			{
				cl = 1;
				mov.xi = px;
				mov.yi = py;
				assinala_quad(px, py, 1);
				jogadas_validas_pos(jog, px, py, movs, 1);
			}
			else if (cl == 1)
			{
				cl = 0;
				mov.xf = px;
				mov.yf = py;
				assinala_quad(mov.xi, mov.yi, 0);
				jogadas_validas_pos(jog, mov.xi, mov.yi, movs, 0);
			}
			clearmouseclick(513);
		}
		delay(1);
	} while (cl == 1 || !movimento_valido(mov));
	executa_movimento(mov);
}

// Dependendo do modo de jogo e do numero da jogada
// pede uma jogada ao humano ou calcula uma jogada para o PC
void jogada(int n, int jog, int tJog)
{
	if (n % 2 == 1)
	{
		if (tJog <= 2)
			jogada_Humano(jog);
		else
			jogada_PC(jog, n);
	}
	else
	{
		if (tJog == 1 || tJog == 3)
			jogada_Humano(jog);
		else
			jogada_PC(jog, n);
	}
}

// Função principal (main) do jogo - contém o ciclo central do jogo
int main(void)
{
	int fim = -1, jog = 0, tipo = tipo_jogo();
	printf("tipo de jogo %d", tipo);
	srand(time(NULL));
	initwindow(SIZE + 8, SIZE + 11); // cria a janela grafica
	inicia_tabul();					 // inicializa o tabuleiro
	do
	{
		nMovs++;
		jog = outroJog(jog); // incrementa jogada e troca de jogador
		printf("Attax Jogada No: %d  Jogador: %d, Aval: %d\n", nMovs, jog, avalia(jog));
		mostra_tabul();			  // mostra o tabuleiro no ecran
		jogada(nMovs, jog, tipo); // executa jogada Humano/PC
		fim = fim_jogo(jog);	  // verifica se o jogo acabou
		delay(100);
	} while (fim == -1);
	finaliza(tipo, jog, fim); // mostra quem venceu o jogo
	return 0;
	system("pause");
}
