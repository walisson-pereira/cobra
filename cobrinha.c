/* programa labirinto.c */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

typedef int bool;
#define true 1
#define false 0

/* Directions */
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

/* Screens */
#define TITLE_SCREEN 1
#define DIFFICULT_SCREEN 2
#define GAME_SCREEN 3
#define VICTORY_SCREEN 4
#define GAMEOVER_SCREEN 5

struct place {
  bool food;
  bool snakeHead;
  int x; //point on screen to draw the hero
  int y;
  bool snakeBody;
  int countMove;
};

// prototipos das funcoes
void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void createfield (void);
void destroyfield (void);
void output(int, int, float, float, float, char *);
bool doCollision (int);
int whereIsHeroC (void);
int whereIsHeroR (void);
void fieldGenerator (void);
char* itoa(int, char[]);
void doAnWay (void);
void timeCount (int);
void snakeMoviment (int);
void go (int);
void cutTail (int);
int whereIsSnakeHeadC (void);
int whereIsSnakeHeadR (void);

//int ** field;
struct place ** field;
int row, column;
int snakeC, snakeR;
int foodC, foodR;
int windowSizeX, windowSizeY;
int borderX, borderY;
int steps;
bool gameover;
long int seed;
int screen;
int fieldProbability;
int survivalTime;
int snakeSize;
int countMove;
int currentDirection;
int speed;
int speedUp;
int foodCount;
int lastDirection; //I use it to remember my last moviment

// funcao principal
int main (int argc, char** argv)
{
  steps = 0;
  gameover = false;
  screen = GAME_SCREEN;
  seed = time (NULL);
  srand (seed);

  survivalTime = 0;
  currentDirection = UP;

  snakeSize = 10;
  countMove = 1;
  speed = 300;
  speedUp = 10;
  foodCount = 0;
  lastDirection = UP;
  
  row = 60;
  column = 60;

  windowSizeX = 1000;//100 + row * 10;
  windowSizeY = 1000;// + column * 10;
  borderX = 50;
  borderY = 50;

  snakeC = 10;
  snakeR = 10;

  do {
    foodC = rand() % column;
    foodR = rand() % row;
  } while (snakeC == foodC && snakeR == foodR);

  glutInit(&argc, argv);                              // start glut
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);           // especifies the colors and buffers
  glutInitWindowSize (windowSizeX, windowSizeY);             // specifies the window size
  glutInitWindowPosition (10, 10);                      // specifies where to show the window
  glutCreateWindow ("Snake");               // create window
  createfield ();
  init();
  glutDisplayFunc(display);                               // function to redraw using GLUT
  glutKeyboardFunc(keyboard);                             // keyboard function
  glutTimerFunc(1000, timeCount, 1);
  glutTimerFunc(speed, snakeMoviment, 1);
  glutMainLoop();                                         // show all created window

  return 0;
}



void init() {
  glClearColor(1.0, 1.0, 1.0, 1.0);    // cor de fundo
  glOrtho (0, (borderX + (row * 10)), 0, (borderY + (column * 10)), -1 ,1);     // modo de projecao ortogonal
} 

void display(void){
  glClear(GL_COLOR_BUFFER_BIT);               // limpa a janela

    /* A small point means a place center */
    int shiftX = borderX / 2;
    int shiftY = borderY / 2;
    int x,y,i,j;
    glPointSize (1.0);
    for (i = 0; i < row; i++) {
      x = ((i + 1) * 10) + shiftX;
      for (j = 0; j < column; j++) {
        y = ((j + 1) * 10) + shiftY;

        field[i][j].x = x;
        field[i][j].y = y;

        glPointSize (1.0);
        glColor3f (0.0, 0.0, 0.0);
        glBegin (GL_POINTS);
          glVertex2i (x, y);
        glEnd();

        /* Draw snakeHead */
        if (field[i][j].snakeHead == true) {
          glPointSize (10.0);
          glColor3f (1.0, 1.0, 0.0);
          glBegin(GL_POINTS);
            glVertex2i (field[i][j].x, field[i][j].y);
          glEnd();        
        }

        /* Draw snakeBody */
        if (field[i][j].snakeBody == true) {
          glPointSize (10.0);
          glColor3f (0.0, 1.0, 0.0);
          glBegin(GL_POINTS);
            glVertex2i (field[i][j].x, field[i][j].y);
          glEnd();        
        }
      
        /* Draw food */
        if (field[i][j].food == true) {
          glPointSize (10.0);
          glColor3f (1.0, 0.0, 0.0);
          glBegin(GL_POINTS);
            glVertex2i (field[i][j].x, field[i][j].y);
          glEnd();        
        }     
      }
    }  

    /* Draw borders */
    glPointSize (3.0);
    glColor3f (0.0, 0.0, 1.0);          

    for (i = 0; i < column; i++) {
      j = 0;
      glBegin (GL_LINES);
        glVertex2i ((field[i][j].x - 5), field[i][j].y - 5);
        glVertex2i ((field[i][j].x + 5), field[i][j].y - 5);           
      glEnd ();
    }
    for (i = 0; i < column; i++) {
      j = row - 1;
      glBegin (GL_LINES);
        glVertex2i ((field[i][j].x - 5), field[i][j].y + 5);
        glVertex2i ((field[i][j].x + 5), field[i][j].y + 5);           
      glEnd ();
    }
    for (j = 0; j < row; j++) {
      i = 0;
      glBegin (GL_LINES);
        glVertex2i ((field[i][j].x - 5), field[i][j].y - 5);
        glVertex2i ((field[i][j].x - 5), field[i][j].y + 5);           
      glEnd ();
    }
    for (j = 0; j < row; j++) {
      i = column - 1;
      glBegin (GL_LINES);
        glVertex2i ((field[i][j].x + 5), field[i][j].y - 5);
        glVertex2i ((field[i][j].x + 5), field[i][j].y + 5);           
      glEnd ();
    }

    char *str = "Poisonous Snake";
    output(300, 640, 0.0, 0.0, 0.0, str);

    char str4[30] = "Survival time: ";
    char str5[10];
    itoa(survivalTime, str5);
    output(400, 10, 0.0, 0.0, 0.0, strcat(str4, str5));

    char str6[30] = "Food: ";
    char str7[10];
    itoa(foodCount, str7);
    output(200, 10, 0.0, 0.0, 0.0, strcat(str6, str7));

  
    glFlush();

}

bool doCollision (int direction) {
  bool collision = false;
  int c = whereIsSnakeHeadC ();
  int r = whereIsSnakeHeadR ();
  
  //verifica se chegou nas bordas (colisão com a parede)
  if (direction == UP && r >= row - 1)
    collision = true;
  if (direction == DOWN && r <= 0)
    collision = true;
  if (direction == LEFT && c <= 0)
    collision = true;
  if (direction == RIGHT && c >= column - 1)
    collision = true;
  
  if (collision == false) { //se não  colidiu com uma parede, então vamos ver se ele colide com o corpo dela
    //verifica se colidiu com o corpo da cobra
    if (direction == UP && field[c][r+1].snakeBody == true)
      collision = true;
    if (direction == DOWN && field[c][r-1].snakeBody == true)
      collision = true;
    if (direction == LEFT && field[c-1][r].snakeBody == true)
      collision = true;
    if (direction == RIGHT && field[c+1][r].snakeBody == true)
      collision = true;
  }
  /*
  if (direction == UP && lastDirection == DOWN)
    collision = false;
  if (direction == DOWN && lastDirection == UP)
    collision = false;
  if (direction == LEFT && lastDirection == RIGHT)
    collision = false;
  if (direction == RIGHT && lastDirection == LEFT)
    collision = false;
*/
  printf ("a cobra colidiu em %d = %d\n", direction, collision);
  if (collision) {
    printf ("Fim de jogo com %d comidas e durou %d segundos\n", foodCount, survivalTime);
    exit(0);
  }

  return collision;
}

int whereIsSnakeHeadC (void) {
  int i,j,c = 0;
  for (i = 0; i < row; i++)
    for (j = 0; j < column; j++)
      if (field[i][j].snakeHead == true)
        c = i;
  printf("a cobra está na C %d\n", c);
  return c;
}

int whereIsSnakeHeadR (void) {
  int i,j,r = 0;
  for (i = 0; i < row; i++)
    for (j = 0; j < column; j++)
      if (field[i][j].snakeHead == true)
        r = j;
  printf("a cobra está na R %d\n", r);
  return r;
}

void keyboard(unsigned char key, int x, int y){
//TODO fazer a lógica da movimentação
/*
  if (screen == GAMEOVER_SCREEN || screen == VICTORY_SCREEN) {
    switch (key) {
    case 27:                                         // tecla Esc (encerra o programa)
      destroyfield ();
    	exit(0);
      break;
    }
  }
*/

   int c, r;
   c = whereIsSnakeHeadC ();
   r = whereIsSnakeHeadR ();
  countMove++;
  if (screen == GAME_SCREEN) {
    switch (key) {
    case 27:                                         // tecla Esc (encerra o programa)
      destroyfield ();
    	exit(0);
      break;
    case 'w':
      go (UP);
      break;
    case 's':
      go (DOWN);
      break;
    case 'a':
      go (LEFT);
      break;
    case 'd':
      go (RIGHT);
      break;
    break;
    }
  
    //TODO: um contador de movimento que só permite o tamanho da cobra ser desenhado na tela.
//    c = whereIsHeroC ();
//    r = whereIsHeroR ();
//    if (field[c][r].end == true) {
//      screen = VICTORY_SCREEN;
//      printf ("Fim de jogo com %d passos\n", steps);
//    }
  }
/*
  if (screen == TITLE_SCREEN) {
    switch (key) {
    case 27:                                         // tecla Esc (encerra o programa)
      destroyfield ();
    	exit(0);
      break;
    case '1':
      survivalTime = 100;
      break;
    case '2':
      survivalTime = 30;
      break;
    case '3':
      survivalTime = 20;
      break;
    case '4':
      survivalTime = 10;
      break;
    }
    screen = GAME_SCREEN;
    glutTimerFunc(1000,timeCount,1);
  }
*/
  glutPostRedisplay();
}

void createfield () {
  field = (struct place **) malloc (row * sizeof(struct place *));
  if (!field)
    fprintf (stderr, "I could not create a field\n");
  int i,j;
  for (i = 0; i < row; i++) {
    *(field + i) = (struct place *) malloc (column * sizeof(struct place));
    if (!(*(field + i))) {
        fprintf (stderr, "I could not create a field\n");
        for (j = 0; j < i; j++)
          free (*(field+i));
        free (field);
        exit (1);
      }
    }
  for (i = 0; i < row; i++)
    for (j = 0; j < column; j++){
      field[i][j].snakeHead = false;
      field[i][j].snakeBody = false;
      field[i][j].food = false;
      field[i][j].countMove = 0;
    }
  field[snakeC][snakeR].snakeHead = true;
  field[foodC][foodR].food = true;

  printf ("Campo criado\n");
}

void destroyfield () {
    int i;
    for (i = 0; i < row; i++) {
      *(field + i) = (struct place *) malloc (column * sizeof(struct place));
      if (!(*(field + i))) {
          fprintf (stderr, "I could not create a field\n");
          int j;
          for (j = 0; j < i; j++)
            free (*(field+i));
          free (field);
      }
    }  
    printf ("Labirinto apagado\n");
    printf ("Fim de jogo com %d passos\n", steps);
}

void output(int x, int y, float r, float g, float b, char *string) {
  /* Valid font of GLUT
    GLUT_BITMAP_8_BY_13
    GLUT_BITMAP_9_BY_15
    GLUT_BITMAP_TIMES_ROMAN_10
    GLUT_BITMAP_TIMES_ROMAN_24
    GLUT_BITMAP_HELVETICA_10
    GLUT_BITMAP_HELVETICA_12
    GLUT_BITMAP_HELVETICA_18
  */

  glColor3f( r, g, b );
  glRasterPos2f(x, y);
  int len, i;
  len = (int)strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
}

/* it is a integer to string function I copied and pasted from internet */
char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

void timeCount (int value) {
  /* I do not use value, but it mandatory to glutTimerFunc*/
  if (screen == GAME_SCREEN) {
    survivalTime++;
//    if (timeLeft == 0)
//      screen = GAMEOVER_SCREEN;
   // go (currentDirection);
    glutPostRedisplay();
    glutTimerFunc(1000, timeCount, 1);
  }
}

void snakeMoviment (int value) {
  /* I do not use value, but it mandatory to glutTimerFunc*/
  if (screen == GAME_SCREEN) {
   // survivalTime++;
//    if (timeLeft == 0)
//      screen = GAMEOVER_SCREEN;
    go (currentDirection);
    glutPostRedisplay();
    glutTimerFunc(speed, snakeMoviment, 1);
  }
}

void go (int direction) {
  srand (seed);
  
  if ((lastDirection == UP && direction == DOWN) ||
      (lastDirection == DOWN && direction == UP) ||
      (lastDirection == LEFT && direction == RIGHT) ||
      (lastDirection == RIGHT && direction == LEFT)) {
    go (lastDirection);
  } else {

  int c, r;
  c = whereIsSnakeHeadC ();
  r = whereIsSnakeHeadR ();
  countMove++;

  if (field[c][r].food == true) {
    field[c][r].food = false;
    snakeSize++;
    foodCount++;
    int c_, r_;
    do {
      c_ = rand () % column;
      r_ = rand () % row;
      printf ("col = %d and row = %d\n", c_, r_);
    } while (field[c_][r_].snakeBody == true || field[c_][r_].snakeHead == true);
    field[c_][r_].food = true;
    speedUp--;
    if (speed > 50 && speedUp == 0) {    
      speed -= 50;
      speedUp = 10;
    }
  }

  lastDirection = currentDirection; //I need to save the last direction to do not try to come back.
  switch (direction) {
    case 27:                                         // tecla Esc (encerra o programa)
      destroyfield ();
    	exit(0);
      break;
    case UP:
      if (!doCollision(UP)) {
          currentDirection = UP;
          field[c][r].snakeBody = true;
          field[c][r].countMove = countMove;
          field[c][r].snakeHead = false;
          field[c][r+1].snakeHead = true;
      }
      break;
    case DOWN:
      if (!doCollision(DOWN)) {
          currentDirection = DOWN;
          field[c][r].snakeBody = true;
          field[c][r].countMove = countMove;
          field[c][r].snakeHead = false;
          field[c][r-1].snakeHead = true;
      }
      break;
    case LEFT:
      if (!doCollision(LEFT)) {
          currentDirection = LEFT;
          field[c][r].snakeBody = true;
          field[c][r].countMove = countMove;
          field[c][r].snakeHead = false;
          field[c-1][r].snakeHead = true;
      }
      break;
    case RIGHT:
      if (!doCollision(RIGHT)) {
          currentDirection = RIGHT;
          field[c][r].snakeBody = true;
          field[c][r].countMove = countMove;          
          field[c][r].snakeHead = false;
          field[c+1][r].snakeHead = true;
      }
      break;
    break;
    }
    cutTail (snakeSize);
  }
}

void cutTail (int snakeSize) {
  int i,j;
  int cutPoint = countMove - snakeSize;
  for (i = 0; i < row; i++)
    for (j = 0; j < column; j++) {
      if (field[i][j].countMove == cutPoint) {
        field[i][j].snakeBody = false;
        printf ("o rabo foi apagado em R %d e C %d\n", j, i);
      }
      if (field[i][j].countMove == cutPoint - 1) {
        field[i][j].snakeBody = false;
        printf ("o rabo foi apagado em R %d e C %d\n", j, i);
      }
    }
}
