#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 3
#define TAB_SIZE 8
#define NAME_SIZE 6
#define BUFF_SIZE 10

struct Board {
  int row;
  int col;
  char* grid;
};

struct Player {
  int score;
  int num;
  char tag;
  char name[NAME_SIZE+1];
  struct Player* next;
};

// helper functions
int coordsToDex(int x, int y, int cols);
void removeNewLine(char* str);
int getScoreLength(int score);
void tagSwap(struct Player* p);
void squareToDex(int* sq, int* dex);

// program functions
void getName(struct Player* p);
void buildBoard(struct Board* g);
void printBoard(struct Board* g, struct Player* p1, struct Player* p2, int* init);
int verifySquare(int* sq, int* dex, struct Board* g, char* buff);
int openSquares(struct Board* g);
int victory(struct Board* g);
int playAgain(struct Board* g, int* init);
int playableBoard(struct Board* g, struct Player* p, int* init);
void readSquare(int* sq, int* dex, struct Board* g, char* name);

// driver
void playTicTakToe(void);

/*** Implimentation ***/

// helper functions
int coordsToDex(int x, int y, int cols) {
  int r = (y-1)*2+1;
  int c = (x-1)*4+2;
  return r*cols+c;
}

void removeNewLine(char* str) {
  int last = strlen(str) - 1;
  if (str[last] == '\n') str[last] = '\0';
}

int getScoreLength(int score) {
  if (!score) return 1;
  int len = 0;
  while (score) {
    score = score / 10;
    len++;
  }
  return len;
}

void tagSwap(struct Player* p) {
  char tmp = p->tag;
  p->tag = p->next->tag;
  p->next->tag = tmp;
}

void squareToDex(int* sq, int* dex) {
  *dex = 67 - 26*((*sq - 1)/BOARD_SIZE) + 4*((*sq - 1)%BOARD_SIZE);
}

// program functions
void getName(struct Player* p) {
  char buff[BUFF_SIZE];
  printf("Enter player %d's name: ",p->num);
  fgets(buff,BUFF_SIZE,stdin);
  while (strlen(buff) > NAME_SIZE + 1) {
    printf("Enter up to six characters: ");
    fgets(buff,BUFF_SIZE,stdin);
  }
  strncpy(p->name,buff,NAME_SIZE);
  removeNewLine(p->name);
}

void buildBoard(struct Board* g) {
  for (int r = 0; r < g->row; r++) {
    for (int c = 0; c < g->col; c++) {
      if (r%2) {
	if (c%4) g->grid[r*g->col+c] = ' ';
	else g->grid[r*g->col+c] = '|';
      }
      else {
	if (c%4) g->grid[r*g->col+c] = '-';
	else g->grid[r*g->col+c] = '+';
      }
    }
  }
}

void printBoard(struct Board* g, struct Player* p1, struct Player* p2, int* init) {
  int p1namelen = strlen(p1->name);
  int p2namelen = strlen(p2->name);
  int p1loffset = (TAB_SIZE - p1namelen) / 2 + p1namelen%2;
  int p2loffset = (TAB_SIZE - p2namelen) / 2;
  char p1left[p1loffset+1];
  char p2left[p2loffset+1];
  for (int i = 0; i < p1loffset; i++) p1left[i] = ' ';
  p1left[p1loffset] = '\0';
  for (int i = 0; i < p2loffset; i++) p2left[i] = ' ';
  p2left[p2loffset] = '\0';

  int p1scorelen = getScoreLength(p1->score);
  int p2scorelen = getScoreLength(p2->score);
  int s1loffset = (TAB_SIZE - p1scorelen) / 2 + p1namelen%2;
  if (p1scorelen > p1namelen && !(p1scorelen%2) && p1namelen%2) s1loffset--;
  int s2loffset = (TAB_SIZE - p2scorelen) / 2;
  if (p2scorelen < p2namelen && !(p2scorelen%2) && p2namelen%2) s2loffset--;
  char s1left[s1loffset+1];
  char s2left[s2loffset+1];
  for (int i = 0; i < s1loffset; i++) s1left[i] = ' ';
  s1left[s1loffset] = '\0';
  for (int i = 0; i < s2loffset; i++) s2left[i] = ' ';
  s2left[s2loffset] = '\0';

  int num = 7;
  for (int r = 0; r < g->row; r++) {
    if (r == 2) printf("%s%s", p1left, p1->name);
    if (r == 3) printf("%s%d", s1left, p1->score);
    printf("\t");
    for (int c = 0; c < g->col; c++) {
      if (*init && r%2 && !((c-2)%4)) {
	printf("%d",num);
	num++;
      } else {
	printf("%c", g->grid[r*g->col+c]);
      }
    }
    if (r == 2) printf("%s%s", p2left, p2->name);
    if (r == 3) printf("%s%d", s2left, p2->score);
    printf("\n");
    if (*init && r%2) num -= 6;
  }
  *init = 0;
}

int openSquares(struct Board* g) {
  for (int y = 1; y <= BOARD_SIZE; y++) {
    for (int x = 1; x <= BOARD_SIZE; x++) {
      if (g->grid[coordsToDex(x,y,g->col)] == ' ') return 1;
    }
  }
  return 0;
}

int victory(struct Board* g) {
  for (int x = 1; x <= BOARD_SIZE; x++) {
    if (g->grid[coordsToDex(x,1,g->col)] != ' ' &&
	g->grid[coordsToDex(x,1,g->col)] == g->grid[coordsToDex(x,2,g->col)] &&
	g->grid[coordsToDex(x,2,g->col)] == g->grid[coordsToDex(x,3,g->col)])
      return 1;
  }
  for (int y = 1; y <= BOARD_SIZE; y++) {
    if (g->grid[coordsToDex(1,y,g->col)] != ' ' &&
	g->grid[coordsToDex(1,y,g->col)] == g->grid[coordsToDex(2,y,g->col)] &&
	g->grid[coordsToDex(2,y,g->col)] == g->grid[coordsToDex(3,y,g->col)])
      return 1;
  }
  if (g->grid[coordsToDex(1,1,g->col)] != ' ' &&
      g->grid[coordsToDex(1,1,g->col)] == g->grid[coordsToDex(2,2,g->col)] &&
      g->grid[coordsToDex(2,2,g->col)] == g->grid[coordsToDex(3,3,g->col)])
    return 1;
  if (g->grid[coordsToDex(1,3,g->col)] != ' ' &&
      g->grid[coordsToDex(1,3,g->col)] == g->grid[coordsToDex(2,2,g->col)] &&
      g->grid[coordsToDex(2,2,g->col)] == g->grid[coordsToDex(3,1,g->col)])
    return 1;
  return 0;
}

int playAgain(struct Board* g, int* init) {
  char buff[BUFF_SIZE];
  printf("Play again? ");
  while(1) {
    fgets(buff, BUFF_SIZE, stdin);
    char ch = buff[0];
    switch (ch) {
    case 'y' :
      free(g->grid);
      buildBoard(g);
      *init = 1;
      return 1;
    case 'n' :
      return 0;
    default :
      printf("Enter 'y' or 'n': ");
    }
  }
}

int playableBoard(struct Board* g, struct Player* p, int* init) {
  if (victory(g)) {
    p->next->score++;
    if (p->num == 1) printBoard(g,p,p->next,init);
    else printBoard(g,p->next,p,init);
    printf("Winner: %s\n", p->next->name);
    tagSwap(p);
    return playAgain(g,init);
  }

  if (openSquares(g)) return 1;

  if (p->num == 1) printBoard(g,p,p->next,init);
  else printBoard(g,p->next,p,init);
  printf("Stalemate.\n");
  tagSwap(p);
  return playAgain(g,init);
}

int verifySquare(int* sq, int* dex, struct Board* g, char* buff) {
  if (strlen(buff) > 1) {
    printf("Enter a single charcter: ");
    return 1;
  }
  *sq = buff[0] - 48;
  if (*sq > BOARD_SIZE*BOARD_SIZE || *sq <= 0) {
    printf("Enter a value between 1 and %d: ", BOARD_SIZE*BOARD_SIZE);
    return 1;
  }
  squareToDex(sq,dex);
  if (g->grid[*dex] != ' ') {
    printf("Square occupied. Choose another: ");
    return 1;
  }
  return 0;
}

void readSquare(int* sq, int* dex, struct Board* g, char* name) {
  char buff[BUFF_SIZE];
  printf("%s enter a square number: ", name);
  do {
    fgets(buff, BUFF_SIZE, stdin);
    removeNewLine(buff);
  } while (verifySquare(sq,dex,g,buff));
}

// driver
void playTicTakToe(void) {
  struct Board g;
  g.row = 7;
  g.col = 13;
  g.grid = (char *) malloc(g.row*g.col * sizeof(char));
  buildBoard(&g);

  struct Player p1;
  p1.score = 0;
  p1.num = 1;
  p1.tag = 'X';
  getName(&p1);

  struct Player p2;
  p2.score = 0;
  p2.num = 2;
  p2.tag = 'O';
  getName(&p2);

  p1.next = &p2;
  p2.next = &p1;

  int sq;
  int dex;
  int init = 1;
  struct Player* curr = &p1;

  while(playableBoard(&g,curr,&init)) {
    printBoard(&g,&p1,&p2,&init);
    readSquare(&sq,&dex,&g,curr->name);
    g.grid[dex] = curr->tag;
    curr = curr->next;
  }

  free(g.grid);
  printf("Goodbye.\n");
}
