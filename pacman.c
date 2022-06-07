#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <math.h>

#define WIDTH 60

//Colores 
#define RESET "\x1B[0m"
#define YEL_P   "\x1B[33m"
#define WHI_G1   "\x1B[37m"
#define MAG_G2   "\x1B[35m"
#define CYN_G3   "\x1B[36m"
#define GRN_G4   "\x1B[32m"

typedef struct Pacman{
	int posX;
	int	posY;
	int oldX;
	int oldY;
	int Life;
	char charact;
}Pacman;

typedef struct Ghost{ 
	int posX;
	int	posY;
	int oldX;
	int oldY;
	int kill;	//boolean
	char charact;
	char mov;
}Ghost;

//Funciones generales
void delay(unsigned int milliseconds);
void gotoxy(int x, int y);
void timer(int tStart, int tEnd);
int difTimer(int tStart, int tEnd);

//Funciones iniciales	
void menu();
void inf();		
void screenPoint();
char screenFinal(int* score);

void start(Pacman* pm, Ghost* ghost, Ghost* ghost2, Ghost* ghost3, Ghost* ghost4, int mapa[][30], int mapaO[][30]);
void copyMapa(int mapaO[][30], int mapa[][30]);

//Funciones de salida
void mapaDraw(int mapa[][30]);
void printMapa(int map[][30],int i,int j);	
void printPac(int Nx, int Ny, int Ox, int Oy, char charc, int mapa[][30], int a);
void auxPrintPac(int Ox, int Oy, int mapa[][30], char perso);

//Funciones de control del juego
void checkStatus(Pacman* pm, Ghost* ghost, int* qtd_comeu, int* score);
void checkWin(int mapa[][30], Pacman *pm);
void points(int *score,int mapa[][30], Pacman* pm);

//Movimento
int verMovX(char** key,char** keepMove,int x, int y,int mapa[][30]);
int verMovY(char** key,char** keepMove, int y, int x,int mapa[][30]);
void movPac( Pacman* pm, char* key, char* keepMove, int mapa[][30]);
int calculadist(int PMx, int PMy, int GHx, int GHy);
void movGhost(Ghost* ghost, Pacman* pacman, int mapa[][30]);	
void movGhost2(Ghost* ghost, Pacman* pacman, int mapa[][30]);
void randomMov(Ghost** ghost, int mapa[][30]);
void follPac(Ghost** ghost, Pacman** pacman, int mapa[][30]);
void goPac(Ghost** ghost, Pacman** pacman, int mapa[][30]);
void teleport(Pacman **pm);
void teleportGhost(Ghost*** ghost);

int main(int argc, char** argv){
	int mapa[30][30];
	int mapaO[30][30] = {
						{4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,5},//1
						{8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,8},//2
						{8,1,4,9,9,9,9,9,9,9,9,9,5,1,4,5,1,4,9,9,9,9,9,9,9,9,9,5,1,8},//3
						{8,1,6,9,9,9,5,4,9,9,9,9,7,1,8,8,1,6,9,9,9,9,5,4,9,9,9,7,1,8},//4
						{8,1,1,1,1,1,8,8,1,1,1,1,1,1,8,8,1,1,1,1,1,1,8,8,1,1,1,1,1,8},//5
						{8,9,9,1,8,1,6,7,1,9,9,9,9,9,7,6,9,9,9,9,9,1,6,7,1,8,1,9,9,8},//6
						{8,2,1,1,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,1,1,2,8},//7
						{8,1,9,9,7,1,9,9,9,9,9,9,9,1,4,5,1,9,9,9,9,9,9,9,1,6,9,9,1,8},//8
						{8,1,1,1,1,1,1,1,1,1,1,1,1,1,8,8,1,1,1,1,1,1,1,1,1,1,1,1,1,8},//9
						{6,9,9,9,5,1,4,5,1,4,9,9,9,9,7,6,9,9,9,9,5,1,4,5,1,4,9,9,9,7},//10
						{0,0,0,0,8,1,8,8,0,6,9,9,9,9,9,9,9,9,9,9,7,0,8,8,1,8,0,0,0,0},//11
						{0,0,0,0,8,1,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,1,8,0,0,0,0},//12
						{0,0,0,0,8,1,8,8,0,4,9,9,5,0,0,0,0,4,9,9,5,0,8,8,1,8,0,0,0,0},//13
						{0,0,0,0,8,1,8,8,0,8,0,0,8,0,0,0,0,8,0,0,8,0,8,8,1,8,0,0,0,0},//14
						{9,9,9,9,7,1,6,7,0,8,0,0,8,0,0,0,0,8,0,0,8,0,6,7,1,6,9,9,9,9},//15
						{0,1,1,1,1,1,0,0,0,8,0,0,8,0,0,0,0,8,0,0,8,0,0,0,1,1,1,1,1,0},//15
						{9,9,9,9,5,1,4,5,0,8,0,0,8,0,0,0,0,8,0,0,8,0,4,5,1,4,9,9,9,9},//14
						{0,0,0,0,8,1,8,8,0,6,9,9,7,0,0,0,0,6,9,9,7,0,8,8,1,8,0,0,0,0},//13
						{0,0,0,0,8,1,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,1,8,0,0,0,0},//12
						{0,0,0,0,8,1,8,8,0,4,9,9,9,9,9,9,9,9,9,9,5,0,8,8,1,8,0,0,0,0},//11
						{4,9,9,9,7,1,6,7,1,6,9,9,9,9,5,4,9,9,9,9,7,1,6,7,1,6,9,9,9,5},//10
						{8,1,1,1,1,1,1,1,1,1,1,1,1,1,8,8,1,1,1,1,1,1,1,1,1,1,1,1,1,8},//9
						{8,1,9,9,5,1,9,9,9,9,9,9,9,1,6,7,1,9,9,9,9,9,9,9,1,4,9,9,1,8},//8
						{8,2,1,1,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,1,1,2,8},//7
						{8,9,9,1,8,1,4,5,1,9,9,9,9,9,5,4,9,9,9,9,9,1,4,5,1,8,1,9,9,8},//6
						{8,1,1,1,1,1,8,8,1,1,1,1,1,1,8,8,1,1,1,1,1,1,8,8,1,1,1,1,1,8},//5
						{8,1,4,9,9,9,7,6,9,9,9,9,5,1,8,8,1,4,9,9,9,9,7,6,9,9,9,5,1,8},//4
						{8,1,6,9,9,9,9,9,9,9,9,9,7,1,6,7,1,6,9,9,9,9,9,9,9,9,9,7,1,8},//3
						{8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,8},//2					
						{6,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,7}};//1
						
	int gameOver=0;
	int score = 0; 
	int speed = 150;
	char key = 'y', keepMove, f = 'n';
	int tStart, tEnd, tEat = 10, qtd_comeu = 1, especial = 0;

	Pacman pm;
	Ghost ghost, ghost2, ghost3, ghost4;

	srand(time(0));

	system("MODE con cols=150 lines=30");
	system("echo");
	system("cls");

	while(!gameOver){

		if(f == 'n'){
			menu();
			inf();
			delay(5000);
			score = 0;
		}
		start(&pm, &ghost, &ghost2, &ghost3, &ghost4, mapa, mapaO);

		while(pm.Life > 0){
			//Verifica pulsación de tecla
			if(kbhit()){
				keepMove = key;
				key = getch();
			}

			
			//Comprobación de precedencia
			if(!especial){
				movPac(&pm, &key, &keepMove, mapa);

				movGhost(&ghost, &pm, mapa);
				movGhost2(&ghost2, &pm, mapa);
				movGhost2(&ghost3, &pm, mapa);
				movGhost2(&ghost4, &pm, mapa);
			} else{
				movGhost(&ghost, &pm, mapa);
				movGhost2(&ghost2, &pm, mapa);
				movGhost2(&ghost3, &pm, mapa);
				movGhost2(&ghost4, &pm, mapa);

				movPac(&pm, &key, &keepMove, mapa);
			}

			//revisa la posición del mapa haciendo referencia a la posición de pacman
			if(mapa[pm.posY][pm.posX-WIDTH] == 2){
				especial = 1;
				tStart = clock();
				//ghost
				ghost.charact = 'm';
				ghost.kill = 1;
				//ghost2
				ghost2.charact = 'm';
				ghost2.kill = 1;
				//ghost3
				ghost3.charact = 'm';
				ghost3.kill = 1;
				//ghost4
				ghost4.charact = 'm';
				ghost4.kill = 1;

			}

			checkStatus(&pm,&ghost, &qtd_comeu, &score);
			checkStatus(&pm,&ghost2, &qtd_comeu, &score);
			checkStatus(&pm,&ghost3, &qtd_comeu, &score);
			checkStatus(&pm,&ghost4, &qtd_comeu, &score);
			tEnd = clock();

			if(especial){
				timer(tStart, tEnd);
			}

			if(difTimer(tStart, tEnd) >= tEat){
				especial = 0;
				//ghost
				ghost.kill = 0;
				ghost.charact = 'w';
				//ghost2
				ghost2.kill = 0;
				ghost2.charact = 'w';
				//ghost3
				ghost3.kill = 0;
				ghost3.charact = 'w';
				//ghost4
				ghost4.kill = 0;
				ghost4.charact = 'w';
				
				qtd_comeu = 1;
			} 

			points(&score, mapa, &pm);
			checkWin(mapa,&pm);

			if(!especial){
				printPac(pm.posX, pm.posY, pm.oldX, pm.oldY, pm.charact, mapa, 0);
				printPac(ghost.posX, ghost.posY, ghost.oldX, ghost.oldY, ghost.charact, mapa, 1);
				printPac(ghost2.posX, ghost2.posY, ghost2.oldX, ghost2.oldY, ghost2.charact, mapa, 2);
				printPac(ghost3.posX, ghost3.posY, ghost3.oldX, ghost3.oldY, ghost3.charact, mapa, 3);
				printPac(ghost4.posX, ghost4.posY, ghost4.oldX, ghost4.oldY, ghost4.charact, mapa, 4);
			} else{
				printPac(ghost.posX, ghost.posY, ghost.oldX, ghost.oldY, ghost.charact, mapa,1);
				printPac(ghost2.posX, ghost2.posY, ghost2.oldX, ghost2.oldY, ghost2.charact, mapa,2);
				printPac(ghost3.posX, ghost3.posY, ghost3.oldX, ghost3.oldY, ghost3.charact, mapa,3);
				printPac(ghost4.posX, ghost4.posY, ghost4.oldX, ghost4.oldY, ghost4.charact, mapa,4);
				printPac(pm.posX, pm.posY, pm.oldX, pm.oldY, pm.charact, mapa,0);
			}
			delay(speed);
		}

		system("cls"); 
		speed = 150;
		f = screenFinal(&score);

		if(f =='y'){
			score = 0;
			system("cls");
		} else if (f == 'n'){
			system("cls");
		}
	}

	return 0;
}

//Funcion de delay
void delay(unsigned int milliseconds){	
    clock_t start = clock();
    while((clock() - start) * 1000 / CLOCKS_PER_SEC < milliseconds){}
}

void gotoxy(int x, int y){
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void timer(int tStart, int tEnd){
	int i = 10 - difTimer(tStart, tEnd);
}

int difTimer(int tStart, int tEnd){
	return ((tEnd - tStart)/(CLOCKS_PER_SEC));
}

void menu(){
	char mStart;
	int i;

	gotoxy(WIDTH,6);
	printf(" _____   _____   _____ PACMAN GAME _____   _____   _____ \n");
	gotoxy(WIDTH + 14,11);
	printf("  Press SPACE to PLAY");
	gotoxy(WIDTH +16,13);
	printf(" Press ESC to EXIT");
    //Verifica si se presionó alguna tecla, si es así, guarda la tecla
	while(mStart != 32){
		for(i=0;i<26;i++){
			if(kbhit()){					
				mStart = getch();
			break;
		 	}								
		}
		if(mStart == 27){
			//Cerrar juego
			exit(1);
		}
		gotoxy(0,0);
	}
	system("cls");
}

void inf(){
	gotoxy(WIDTH  + 13,5);
	printf("CONTROLS!");
	gotoxy(WIDTH,7);
	printf("W -> UP");
	gotoxy(WIDTH,8);
	printf("S -> DOWN");
	gotoxy(WIDTH,9);
	printf("A -> LEFT");
	gotoxy(WIDTH,10);
	printf("D -> RIGHT");
	gotoxy(WIDTH +13, 12);
	printf("RULES!");
	gotoxy(WIDTH,14);
	printf("%c Pacman wins the game when it has taken all coins in the map", 175);
	gotoxy(WIDTH,15);
	printf("%c Pacman loses when an enemy touches it",175);
}

void start(Pacman* pm, Ghost* ghost,Ghost* ghost2, Ghost* ghost3, Ghost* ghost4, int mapa[][30], int mapaO[][30]){

	copyMapa(mapaO, mapa);
	system("cls");  
	delay(1000);
	mapaDraw(mapa);
	screenPoint();
	
	//Config pacman
	(*pm).Life = 3;
	(*pm).charact = 'C';
	(*pm).posX = WIDTH + 14; 						//distance
	(*pm).posY	= 23; 						//Height
	gotoxy((*pm).posX,(*pm).posY);		//Initial pos


	//ghost
	(*ghost).charact = 'w';
	(*ghost).kill = 0;
	(*ghost).posX = WIDTH + 14;
    (*ghost).posY = 13;
	(*ghost).mov  = 'w';
    gotoxy((*ghost).posX, (*ghost).posY);
    gotoxy(0,0);

	//ghost2
	(*ghost2).charact = 'w';
	(*ghost2).kill = 0;
	(*ghost2).posX = WIDTH + 14;
    (*ghost2).posY = 12;
	(*ghost2).mov  = 'd';
    gotoxy((*ghost2).posX, (*ghost2).posY);
    gotoxy(0,0);

	//ghost3
	(*ghost3).charact = 'w';
	(*ghost3).kill = 0;
	(*ghost3).posX = WIDTH + 14;
    (*ghost3).posY = 11;
	(*ghost3).mov  = 'w';
    gotoxy((*ghost3).posX, (*ghost3).posY);
    gotoxy(0,0);

	//ghost4
	(*ghost4).charact = 'w';
	(*ghost4).kill = 0;
	(*ghost4).posX = WIDTH + 14;
    (*ghost4).posY = 10;
	(*ghost4).mov  = 'a';
    gotoxy((*ghost4).posX, (*ghost4).posY);
    gotoxy(0,0);
}

char screenFinal(int* score){
	char f;

	gotoxy(WIDTH + 10,10);
	//Final Screen
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);
	gotoxy(WIDTH+10,11);
	printf("%c    !!!!SCORE!!!    %c",186,186);
	gotoxy(WIDTH+10,12);
	printf("%c                    %c",186,186);
	gotoxy(WIDTH+10,13);
	printf("%c                    %c",186,186);
	gotoxy(WIDTH+10,14);
	printf("%c      Retry?        %c",186,186);
	gotoxy(WIDTH+10,15);
	printf("%c       (y/n)        %c",186,186);
	gotoxy(WIDTH+10,16);
	printf("%c                    %c",186,186);
	gotoxy(WIDTH+10,17);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);

	gotoxy(WIDTH +19,12);
	printf("%d", *score);
	gotoxy(0,0); 

	while(f != 'y'|| f != 'n'){
		if(kbhit()){
			f = getch();
			if(f=='y')
				return f;
		} else if(f == 'n'){
				return f;
		}
	} 
}

void mapaDraw(int mapa[][30]){
	int i,j;
	for(i=0;i<30;i++){
		for(j=0;j<30;j++){
			printMapa(mapa,i,j);
		}
	}
}

void printMapa(int mapa[][30],int i, int j){
		
	switch(mapa[i][j]){
		case 0:{
			gotoxy(WIDTH+j,i);
			printf(" ");
			break;
		}
		case 1:{
			gotoxy(WIDTH+j,i);
			printf("*");
			break;
		}
		case 2:{
			gotoxy(WIDTH+j,i);
			printf("@");
			break;
		}
		case 4:{
			gotoxy(WIDTH+j,i);
			printf("%c", 201);
			break;
		}
		case 5:{
			gotoxy(WIDTH+j,i);
			printf("%c", 187);
			break;
		}
		case 6:{
			gotoxy(WIDTH+j,i);
			printf("%c", 200);
			break;
		}
		case 7:{
			gotoxy(WIDTH+j,i);
			printf("%c", 188);
			break;
		}
		case 8:{		
			gotoxy(WIDTH+j,i);
			printf("%c", 186);
			break;
		}
		case 9:{
			gotoxy(WIDTH+j,i);
			printf("%c", 205);
			break;
		}
	}
}

void printPac(int Nx, int Ny, int Ox, int Oy, char charc, int mapa[][30], int a){
	if(Nx!=Ox || Ny!=Oy)
	{
		switch(a){
			case 0: {
				gotoxy(Nx,Ny);
				printf(YEL_P "%c" RESET, charc);
				gotoxy(0,0);
				auxPrintPac(Ox, Oy, mapa, charc);
				break;
			}
			case 1:{
				gotoxy(Nx,Ny);
				printf(WHI_G1 "%c" RESET, charc);
				gotoxy(0,0);
				auxPrintPac(Ox, Oy, mapa, charc);	
				break;
			}
			case 2:{
				gotoxy(Nx,Ny);
				printf(MAG_G2 "%c" RESET, charc);
				gotoxy(0,0);
				auxPrintPac(Ox, Oy, mapa, charc);
				break;
			}
			case 3:{
				gotoxy(Nx,Ny);
				printf(CYN_G3 "%c" RESET, charc);
				gotoxy(0,0);
				auxPrintPac(Ox, Oy, mapa, charc);
				break;
			}
			case 4:{
				gotoxy(Nx,Ny);
				printf(GRN_G4 "%c" RESET, charc);
				gotoxy(0,0);
				auxPrintPac(Ox, Oy, mapa, charc);
				break;
			}	
		}
	}
}

void auxPrintPac(int Ox, int Oy, int mapa[][30], char perso){
	if(perso == 'w'|| perso == 'm'){	
		printMapa(mapa, Oy, (Ox-WIDTH));	
		gotoxy(0,0);		
	} else{
		gotoxy(Ox,Oy);
		printf(" ");
		gotoxy(0,0);
	}
}

void movPac(Pacman* pm, char* key, char* keepMove, int mapa[][30]){

	if((*pm).oldX != (*pm).posX ){
		(*pm).oldX = (*pm).posX;
	}
	if((*pm).oldY != (*pm).posY)    {
		(*pm).oldY = (*pm).posY;
	}

	if(*keepMove == 'w' || *keepMove == 's'){
		(*pm).posX = verMovX(&key, &keepMove, (*pm).oldX, (*pm).oldY, mapa);
		(*pm).posY = verMovY(&key, &keepMove, (*pm).oldY, (*pm).oldX, mapa);
	} else if(*keepMove == 'a' || *keepMove == 'd'){
		(*pm).posY = verMovY(&key, &keepMove, (*pm).oldY, (*pm).oldX, mapa);
		(*pm).posX = verMovX(&key, &keepMove, (*pm).oldX, (*pm).oldY, mapa);
	}
	teleport(&pm);
}

int verMovX(char** key, char** keepMove, int x,int y,int mapa[][30]){
	switch(**key){
		case 'a':{
			if(mapa[y][x -(1+WIDTH)] <=3){
				x--;
				**keepMove = **key;
				return x;
			} else{
				**key = **keepMove;
				return x;}
			break;
		}
		case 'd':{
			if(mapa[y][x +(1-WIDTH)] <=3){
				x++;
				**keepMove = **key;				
				return x;
			} else {
				**key = **keepMove;
				return x;}
			break;
		}
		default:{
			return x;
			break;
		}
	}
}

int verMovY(char** key, char** keepMove, int y, int x,int mapa[][30]){
	//Pointer
	switch(**key){
		case 'w':{
			if(mapa[y-1][x-WIDTH] <=3){
				y--;
				**keepMove = **key;
				return y;
			}else{
				**key = **keepMove;
				return y;}
			break;
		}
		case 's':{
			if(mapa[y+1][x-WIDTH]<=3){
				y++;
				**keepMove = **key;				
				return y;
			}else{
				**key = **keepMove;
				return y;}
			break;
		}
		default:{
			return y;
			break;
		}
	}
}

void screenPoint(){
 int i,j=0,x,y;
  
  for(i=0;i<2-j;i++){
	   if(i==0){
         x=WIDTH -25;
         y=3;
	    }
	   else {
	   	 x=WIDTH -25;
	   	 y=9;
	    }
	   
		//Tablas de vida y puntos;
	 gotoxy(x,y);
	 printf("%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,187);
	 gotoxy(x,y+1);
	 printf("%c",186);
	 if(i==0){
	 	gotoxy(x+3,y+1);
	    printf("SCORE");
	 }
	 else {
	 	gotoxy(x+3,y+1);
	    printf("LIFES");
	 }
	 gotoxy(x+10,y+1);
	 printf("%c",186);
	 gotoxy(x,y+2);
	 printf("%c",186);
	 gotoxy(x+10,y+2);
	 printf("%c",186);
	 gotoxy(x,y+3);
	 printf("%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,188);
	}
}

void points(int *score, int mapa[][30], Pacman* pm){
	switch(mapa[(*pm).posY][(*pm).posX-WIDTH]){
		case 1:{
			(*score)+=10;
			mapa[(*pm).posY][(*pm).posX-WIDTH] = 0;
			break;
		}
		case 2:{
			(*score)+=50;
			mapa[(*pm).posY][(*pm).posX-WIDTH] = 0;
			break;
		}
	}
    //Score
	gotoxy(WIDTH - 23,5);
	printf("  %d",(*score));
	//Lifes
	gotoxy(WIDTH -22,11);
	printf("  %d",(*pm).Life);
}

int calculadist(int PMx, int PMy, int GHx, int GHy){
	return sqrt(pow(GHx-PMx,2) + pow(GHy-PMy,2));
}

void movGhost(Ghost* ghost, Pacman* pacman, int mapa[][30]){

	if((*ghost).oldX != (*ghost).posX){
		(*ghost).oldX = (*ghost).posX;
	}
	if((*ghost).oldY != (*ghost).posY){
		(*ghost).oldY = (*ghost).posY;
	}
 
  	if((*ghost).kill == 0){
		if(rand()%100 < 70){
			follPac(&ghost, &pacman, mapa);
		}
	} else{
		if(rand()%100 < 50){
			goPac(&ghost, &pacman, mapa);
		} 
	}
}

void movGhost2(Ghost* ghost, Pacman* pacman, int mapa[][30]){

	int dist_menor = calculadist((*ghost).posX,(*ghost).posY,(*pacman).posX,(*pacman).posY);

	//Is to not let the characters turn off if they are stopped
	if((*ghost).oldX != (*ghost).posX){
		(*ghost).oldX = (*ghost).posX;
	}
	if((*ghost).oldY != (*ghost).posY){
		(*ghost).oldY = (*ghost).posY;
	}
	if(dist_menor < 5){

		if((*ghost).kill == 0){
		if(rand()%100 < 80){
			follPac(&ghost, &pacman, mapa);
		}
		} else {
		if(rand()%100 < 80){
			goPac(&ghost, &pacman, mapa);
			} 
		}
	} else {
		randomMov(&ghost, mapa);
	}
}

void teleport(Pacman **pm){
	if((**pm).posX == WIDTH){
		(**pm).posX = WIDTH + 29; 
	} else if((**pm).posX == WIDTH + 29){
		(**pm).posX = WIDTH;
	}	
}

void teleportGhost(Ghost*** ghost){
	if((***ghost).posX == WIDTH){
			(***ghost).posX = WIDTH +29;
	} else if((***ghost).posX == WIDTH +29){
		(***ghost).posX = WIDTH;
	}
}

void checkStatus(Pacman* pm, Ghost* ghost, int* qtd_comeu, int* score){

	if((*pm).posX == (*ghost).posX && (*pm).posY == (*ghost).posY){
		switch((*ghost).kill){
			case 0:{
				//RESPAWN PAC
				(*pm).Life --;
				(*pm).posX=WIDTH + 14;
				(*pm).posY=23;
				delay(1000);

				//respawn ghost
				(*ghost).posX =WIDTH + 14;
				(*ghost).posY = 13;
				(*ghost).kill = 0;
				(*ghost).charact = 'w';
				break;
			}
			case 1:{
				//respawn ghost
				(*ghost).posX =WIDTH + 14;
				(*ghost).posY = 13;
				(*ghost).kill = 0;
				(*ghost).charact = 'w';

				//showScreen
				gotoxy(WIDTH + 15,33);
				printf("+%d", 100*(*qtd_comeu));
				//DELAY 
				delay(300);
				gotoxy(WIDTH + 15,33);
				printf("              ");
				//LOG
				(*score) +=100*(*qtd_comeu);
				(*qtd_comeu)++;
				break;
			}
		}
	}	
}

void follPac(Ghost** ghost, Pacman** pacman, int mapa[][30]){
	int GHx_menor = (**ghost).posX;
	int GHy_menor = (**ghost).posY;
	int dist_menor;

	dist_menor = calculadist((**ghost).posX,(**ghost).posY,(**pacman).posX,(**pacman).posY);

	if((mapa[(**ghost).posY][(**ghost).posX - (1+WIDTH)] < 3) && (calculadist(((**ghost).posX-1),(**ghost).posY,(**pacman).posX,(**pacman).posY) < dist_menor)){
		GHx_menor = ((**ghost).posX - 1);
		GHy_menor = (**ghost).posY;
		}
		else if((mapa[(**ghost).posY][((**ghost).posX + (1-WIDTH))] < 3 ) && (calculadist(((**ghost).posX +1),(**ghost).posY,(**pacman).posX,(**pacman).posY) < dist_menor))
		{
		GHx_menor = ((**ghost).posX) + 1;
		GHy_menor = (**ghost).posY;
		}
		else if((mapa[(**ghost).posY-1][((**ghost).posX - WIDTH)] < 3 ) && (calculadist((**ghost).posX ,((**ghost).posY-1),(**pacman).posX,(**pacman).posY) < dist_menor))
		{
		GHx_menor = (**ghost).posX ;
		GHy_menor = ((**ghost).posY - 1);
		}
		else if((mapa[(**ghost).posY + 1][((**ghost).posX - WIDTH)] < 3) && (calculadist((**ghost).posX ,((**ghost).posY+1),(**pacman).posX,(**pacman).posY) < dist_menor))
		{
		GHx_menor = (**ghost).posX ;
		GHy_menor = ((**ghost).posY + 1);
		}
	

	if((**ghost).posX  == GHx_menor && ((**ghost).posY == GHy_menor)){
		if(( pow( ((**ghost).posX -1) - (**pacman).posX ,2) < pow( ((**ghost).posX - (**pacman).posX) ,2))  && (mapa[(**ghost).posY][(**ghost).posX - (1+WIDTH)] < 3)){
			GHx_menor = (**ghost).posX -1;
		} else if(( pow( ((**ghost).posX +1) - (**pacman).posX, 2) < pow( ((**ghost).posX - (**pacman).posX), 2)) && (mapa[(**ghost).posY][(**ghost).posX + (1-WIDTH)] < 3)){
			GHx_menor = (**ghost).posX +1;
		}

		if(( pow( ((**ghost).posY-1) - (**pacman).posY ,2) < pow( ((**ghost).posY - (**pacman).posY) , 2)) && (mapa[(**ghost).posY-1][(**ghost).posX -WIDTH] < 3)){
			GHy_menor = (**ghost).posY-1;
		} else if(( pow( ((**ghost).posY+1) - (**pacman).posY, 2) < pow( ((**ghost).posY - (**pacman).posY), 2)) && (mapa[(**ghost).posY+1][(**ghost).posX -WIDTH] < 3)){
			GHy_menor = (**ghost).posY+1;
		}
	}

	(**ghost).posX  = GHx_menor;
	(**ghost).posY  = GHy_menor;
	teleportGhost(&ghost);
}

void goPac(Ghost** ghost, Pacman** pacman, int mapa[][30]){
	int GHx_menor = (**ghost).posX;
	int GHy_menor = (**ghost).posY;
	int dist_menor;

	dist_menor = calculadist((**ghost).posX,(**ghost).posY,(**pacman).posX,(**pacman).posY);

	if((mapa[(**ghost).posY][(**ghost).posX - (1+WIDTH)] < 3) && (calculadist(((**ghost).posX-1),(**ghost).posY,(**pacman).posX,(**pacman).posY) > dist_menor)){
		GHx_menor = ((**ghost).posX - 1);
		GHy_menor = (**ghost).posY;
		}	
		else if((mapa[(**ghost).posY][((**ghost).posX + (1-WIDTH))] < 3 ) && (calculadist(((**ghost).posX +1),(**ghost).posY,(**pacman).posX,(**pacman).posY) > dist_menor))
		{
		GHx_menor = ((**ghost).posX) + 1;
		GHy_menor = (**ghost).posY;
		}	
		else if((mapa[(**ghost).posY-1][((**ghost).posX - WIDTH)] < 3 ) && (calculadist((**ghost).posX ,((**ghost).posY-1),(**pacman).posX,(**pacman).posY) > dist_menor))
		{
		GHx_menor = (**ghost).posX ;
		GHy_menor = ((**ghost).posY - 1);
		}
		else if((mapa[(**ghost).posY + 1][((**ghost).posX - WIDTH)] < 3) && (calculadist((**ghost).posX ,((**ghost).posY+1),(**pacman).posX,(**pacman).posY) > dist_menor))
		{
		GHx_menor = (**ghost).posX ;
		GHy_menor = ((**ghost).posY + 1);
		}
	

	if((**ghost).posX  == GHx_menor && ((**ghost).posY == GHy_menor)){

		if(( pow( (**ghost).posX -1 - (**pacman).posX ,2) > pow( ((**ghost).posX - (**pacman).posX) ,2))  && (mapa[(**ghost).posY][(**ghost).posX - (1+WIDTH)] < 3)){
			GHx_menor = (**ghost).posX -1;
		} else if(( pow( ((**ghost).posX +1) - (**pacman).posX, 2) > pow( ((**ghost).posX - (**pacman).posX), 2)) && (mapa[(**ghost).posY][(**ghost).posX + (1-WIDTH)] < 3)){
			GHx_menor = (**ghost).posX +1;
		}

		if(( pow( ((**ghost).posY-1) - (**pacman).posY ,2) > pow( ((**ghost).posY - (**pacman).posY) , 2)) && (mapa[(**ghost).posY-1][(**ghost).posX - WIDTH] < 3)){
			GHy_menor = (**ghost).posY-1;
		} else if(( pow( ((**ghost).posY+1) - (**pacman).posY, 2) > pow( ((**ghost).posY - (**pacman).posY), 2)) && (mapa[(**ghost).posY+1][(**ghost).posX - WIDTH] < 3)){
			GHy_menor = (**ghost).posY+1;
		}
	}

	(**ghost).posX  = GHx_menor;
	(**ghost).posY  = GHy_menor;
	teleportGhost(&ghost);
}

void randomMov(Ghost** ghost, int mapa[][30]){
	
	switch((**ghost).mov){
		case 'w':{
			if(mapa[(**ghost).posY][(**ghost).posX  - (1+WIDTH)] < 3){
				(**ghost).posX --;
			} else{
				if(rand()%100 < 40){
					(**ghost).mov = 'a';
					randomMov(ghost, mapa);
				}else if(rand()%100 < 80){
					(**ghost).mov = 'd';
					randomMov(ghost, mapa);
				}else{
					(**ghost).mov = 's';
					randomMov(ghost, mapa);
				}
			}
			break;
		}
		case 'a':{
			if(mapa[(**ghost).posY-1][(**ghost).posX  - WIDTH] < 3){
				(**ghost).posY --;
			} else{
				if(rand()%100 < 40){
					(**ghost).mov = 'w';
					randomMov(ghost, mapa);
				}else if(rand()%100 < 80){
					(**ghost).mov = 's';
					randomMov(ghost, mapa);
				}else{
					(**ghost).mov = 'd';
					randomMov(ghost, mapa);
				}
			}
			break;
		}		
		case 's':{
			if(mapa[(**ghost).posY][(**ghost).posX  + (1-WIDTH)] < 3){
				(**ghost).posX ++;
			} else{
				if(rand()%100 < 40){
					(**ghost).mov = 'a';
					randomMov(ghost, mapa);
				}else if(rand()%100 < 80){
					(**ghost).mov = 'd';
					randomMov(ghost, mapa);
				}else{
					(**ghost).mov = 'w';
					randomMov(ghost, mapa);
				}
			}
			break;
		}
		case 'd':{
			if(mapa[(**ghost).posY+1][(**ghost).posX  - WIDTH] < 3){
				(**ghost).posY ++;
			} else{
				if(rand()%100 < 40){
					(**ghost).mov = 'w';
					randomMov(ghost, mapa);
				}else if(rand()%100 < 80){
					(**ghost).mov = 's';
					randomMov(ghost, mapa);
				}else{
					(**ghost).mov = 'a';
					randomMov(ghost, mapa);
				}
			}
			break;
		}
	}
	teleportGhost(&ghost);
}  

void copyMapa(int mapaO[30][30], int mapa[30][30]){
	int i,j;

	for(i=0;i<30;i++){
		for(j=0;j<30;j++){
			mapa[i][j]=mapaO[i][j];
		}
	}
}

void checkWin(int mapa[][30], Pacman *pm){
	int i,j, sum=0;
	for(i=1;i<29;i++){
		for(j=1;j<29;j++){
			if(mapa[i][j] == 1 ||mapa[i][j] == 2){
				sum ++;
			}
		}
	}
	if(sum == 0){
		(*pm).Life = -1;		
	}
}