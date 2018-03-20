#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio_ext.h>

#define TOTAL_SCENES 1
#define FACTIONS 4

#define KEY_LEFT 'l'
#define KEY_RIGHT 'r'
struct sscene {
	char person[10];
	char text[50];
	char left[15];
	char right[15];
	int infleft[FACTIONS];
	int infright[FACTIONS];
};

typedef struct sscene scene;

void playscene(int code, scene *scn, char *title, char *name, int *inf, char facs[FACTIONS][13]);
void readscenes(scene *scn);
int checkInfluence(int *);
void printInfluences(int *, char facs[FACTIONS][13], int wc);
int findNextScene(scene *, int *, int);

int main(){
	char name[10];
	char title[6];
	int gender, gameover = 0;
	int influences[] = { 50, 20, 30, 40 };
	char factionchars[FACTIONS][13];
	strcpy(factionchars[0], "Army\0");
	strcpy(factionchars[1], "Bourgeoisie\0");
	strcpy(factionchars[2], "Church\0");
	strcpy(factionchars[3], "Common folk\0");

	int graph[TOTAL_SCENES][TOTAL_SCENES][2] = {0};
	int i, j;
	FILE *fgraph = fopen("graph.txt", "r");
	for (i = 0; i < TOTAL_SCENES; i++){
		for (j = 0; j < TOTAL_SCENES; j++){
			fscanf(fgraph, "%d", &graph[i][j][0]); //Left-hand decisions
		}
	}
	for (i = 0; i < TOTAL_SCENES; i++){
		for (j = 0; j < TOTAL_SCENES; j++){
			fscanf(fgraph, "%d", &graph[i][j][1]); //Right-hand decisions
		}
	}

	scene scenes[TOTAL_SCENES];

	readscenes(scenes);

	int currScene = 0, years = -2; 

	printf("What is your name?\n> ");
	scanf("%[^\n]s", name);
	printf("\nAnd a");
	do {
		printf("re you a king (type 1) or a queen (type 0)?");
		scanf("%i", &gender);
		if (gender != 1 && gender != 0) printf("Sorry, I didn't understand that...\nA");
	} while (gender != 1 && gender != 0);
	if (gender) strcpy(title, "King"); else strcpy(title, "Queen");

	while (!gameover) {
		playscene(0, scenes, title, name, influences, factionchars);
		
		gameover = checkInfluence(influences);
	}

	printInfluences(influences, factionchars, 1);
	switch (gameover) {
		case 1: printf("\n\tYour army has revolted and dethroned you. Your legacy falls at the might of your generals.\n"); break;
		case 2: printf("\n\tThe rich folk decided they were better off without you. Your execution marked the start of a new era.\n"); break;
		case 3: printf("\n\tThe priests that once supported you now claim you've been posessed. As the people lose their faith, you lose your people.\n"); break;
		case 4: printf("\n\tThe common folk grab torches and pitchforks. In a fateful night, they storm your castle, and in the blink of an eye, your kingdom is no more.\n"); break;
		default: printf("Unknown ending..."); break;
	}
	return 0;
}

int checkInfluence(int *inf) {
	int i;	
	for (i = 0; i < FACTIONS; i++) if (inf[i] <= 0) return i+1;
	return 0;
}

void printInfluences(int *inf, char facs[FACTIONS][13], int wc) {
		int i;
		if (wc) system("clear");
		for (i = 0; i < FACTIONS; i++){
			printf("\t%13s |", facs[i]);
		}
		printf("\n");
		for (i = 0; i < FACTIONS; i++){
			printf("\t%13d |", inf[i]);
		}
		printf("\n-------------------------------------------------------------------------------------\n");
}

int findNextScene(scene *all, int *graph, int current){
	int possible[TOTAL_SCENES];
	int valid = 0, i;
	for (i = 0; i < TOTAL_SCENES; i++) { if (graph[current][i]) { possible[valid] = i; valid++; } }
	return possible[rand() % valid];
}

void playscene(int code, scene *scn, char *title, char *name, int *inf, char facs[FACTIONS][13]){
	int op, done = 0, i;
	do {
		printInfluences(inf, facs, 1);
		printf("\t%s says...\n\n", scn[code].person);
		printf("\t\t%s\n", scn[code].text);

		printf("\n");

		printf("\t(%c) %s\n", KEY_LEFT, scn[code].left);
		printf("\t\tOR\n");
		printf("\t(%c) %s\n", KEY_RIGHT, scn[code].right);

		printf("\n\tWhat will you do, %s %s?", title, name);
		__fpurge(stdin);
		
		op = getchar();

		printf("%d\n", op);

		if (op == KEY_LEFT) {
			
			for (i = 0; i < FACTIONS; i++) inf[i] = inf[i] + scn[code].infleft[i];

			done = 1;
		} else if (op == KEY_RIGHT) {
			for (i = 0; i < FACTIONS; i++) inf[i] = inf[i] + scn[code].infright[i];
			done = 1;
		}
	} while (done != 1);
}

void readscenes(scene *scn) {
	FILE *f;
	f = fopen("scenes.txt", "r");
	
	int i = 0, j, t;
	
	for (i = 0; i < TOTAL_SCENES; i++){
		fscanf(f, "%[^\n]s", scn[i].person);
		
		getc(f);
		fscanf(f, "%[^\n]s", scn[i].text);
		
		getc(f);
		fscanf(f, "%[^\n]s", scn[i].left);
		
		getc(f);
		for (j = 0; j < FACTIONS; j++) {
			fscanf(f, "%d", &scn[i].infleft[j]);
			
		}
		getc(f);
		fscanf(f, "%[^\n]s", scn[i].right);
		
		
		for (j = 0; j < FACTIONS; j++) {
			fscanf(f, "%d", &scn[i].infright[j]);
			
		}
		
	}
}
