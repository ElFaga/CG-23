#include <GL/glut.h> 
// Bibliotecas essenciais
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <string.h>
#include <stdarg.h>

//  ********************
// Variaveis globais

// Altera o tipo de tela
int screen_option = 0;
// Variaveis do Mouse 
int mouse_x, mouse_y, Win_x, Win_y, object_select;
// Variaveis estaticas para o tipo de visao
static int view_state = 0;
// Animacao das pecas
int spin, spinboxes;
// Define o vencedor
int win;
// Identifica a vez
int player, computer;
// Jogo iniciou
int start_game;
// Auxilio para construir o X
GLUquadricObj * Cylinder;
//  ********************
// Tabuleiro, Informacoes e Variaveis

// Todas as opcoes de local estao representadas abaixo
// 0 | 1 | 2
// 3 | 4 | 5
// 6 | 7 | 8
// Mapa das posicoes
int box_map[9];
// Cada posicao armazena um valor
// -1 = Jogador ocupou a posicao, logo sera desenhado um O
// 1 = Computador ocupou a posicao, logo sera desenhado um X
// 0 = Espaco Vago

// Coordenadas das posicoes do tabuleiro
// Auxilia na identificacao de qual posicao foi selecionada
int object_map[9][2] = {{-6,6},{0,6},{6,6},{-6,0},{0,0},{6,0},{-6,-6},{0,-6},{6,-6}};

// Armazena a possibilidade de vitorias
static int box[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

//  ********************
// Inicializa o tabuleiro
void init_game(void) {
  int i;
  // Limpa o tabuleiro para um novo jogo
  for (i = 0; i < 9; i++) {
    box_map[i] = 0;
  }
  // Inicializando variaveis do jogo
  win = 0;
  start_game = 1;
}

// Verifica se ha vencedores
int check_move(void) {
  
  int i, t = 0;
  // Verifica se tem 3 em sequencia
  for (i = 0; i < 8; i++) {
    // Soma o valor presente nos quadrantes
    t = box_map[box[i][0]] + box_map[box[i][1]] + box_map[box[i][2]];
    if ((t == 3) || (t == -3)) {
      // Identifica quais caixas fazem parte da vitoria para rodar a animacao
      spinboxes = i;
      // retorna 1 caso encontre um vencedor
      return (1);
    }
  }
  
  t = 0;
  // Verifica se deu velha
  for (i = 0; i < 8; i++) {
    t = t + abs(box_map[box[i][0]]) + abs(box_map[box[i][1]]) + abs(box_map[box[i][2]]);
  }
  // Retorna 2 caso positivo
  if (t == 24) return (2);
  
  // Se nada aconteceu, o jogo esta rolando, retorna 0
  return (0);
}

// Um algoritmo simples para simular um jogador real
// Bloqueando jogadas e jogando onde pode ganhar
int blocking_win() {
  int i, t;
  for (i = 0; i < 8; i++) {
    t = box_map[box[i][0]] + box_map[box[i][1]] + box_map[box[i][2]];
    // Buscando locais vagos
    if ((t == 2) || (t == -2)) {
      if (box_map[box[i][0]] == 0) box_map[box[i][0]] = computer;
      if (box_map[box[i][1]] == 0) box_map[box[i][1]] = computer;
      if (box_map[box[i][2]] == 0) box_map[box[i][2]] = computer;
      // Se o computador conseguiu jogar, retorna 1
      return (1);
    }
  }
  // Se o computador nao jogou, retorna 0
  return (0);
}

// Verifica espacos vazios no canto
int check_corner(void) {
  int i;
  if (box_map[0] == 0) {
    box_map[0] = computer;
    i = 1;
    return (1);
  }
  if (box_map[2] == 0) {
    box_map[2] = computer;
    i = 1;
    return (1);
  }
  if (box_map[6] == 0) {
    box_map[6] = computer;
    i = 1;
    return (1);
  }
  if (box_map[8] == 0) {
    box_map[8] = computer;
    i = 1;
    return (1);
  }
  return (0);
}

// Verifica espacos livres nas linhas
int check_row(void) {
  // Centro
  if (box_map[4] == 0) {
    box_map[4] = computer;
    return (1);
  }
  // Restante
  if (box_map[1] == 0) {
    box_map[1] = computer;
    return (1);
  }
  if (box_map[3] == 0) {
    box_map[3] = computer;
    return (1);
  }
  if (box_map[5] == 0) {
    box_map[5] = computer;
    return (1);
  }
  if (box_map[7] == 0) {
    box_map[7] = computer;
    return (1);
  }
  // Se nao achar posicoes livre, retorna 0
  return (0);
}

// Logica para os movimentos do computador
int computer_move() {
  if (blocking_win() == 1) return (1);
  if (check_corner() == 1) return (1);
  if (check_row() == 1) return (1);
  return (0);
}

// Apresenta textos na tela
void Sprint(int x, int y, char * st) {
  int l, i;
  l = strlen(st);
  glRasterPos2i(x, y);
  for (i = 0; i < l; i++){
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
  }
}

static void TimeEvent(int te) {
  spin++;
  if (spin > 360) spin = 180;
  glutPostRedisplay();
  glutTimerFunc(8, TimeEvent, 1);
}


void init_light(void)
{
  GLfloat mat_specular[] = {0.5, 0.5, 0.5, 1.0};
  GLfloat mat_shininess[] = { 100.0 };
 
  GLfloat light0_position[] = { 0.0, 0.0, 4.0, 0.0 };
  GLfloat light0_color[] = { 1.0, 1.0, 1.0, 1.0 };

  GLfloat light1_position[] = {-1.0, 1.0, 0.4, 0.0};
  GLfloat light1_color[] = {0.0, 0.0, 1.0, 1.0};

  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_SMOOTH);

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_DEPTH_TEST);
}
void init(void) {

  glClearColor(0.082, 0.125, 0.169, 0.0);
  start_game = 0;
  win = 0;

  Cylinder = gluNewQuadric();
  gluQuadricDrawStyle(Cylinder, GLU_FILL);
  gluQuadricNormals(Cylinder, GLU_SMOOTH);
  gluQuadricOrientation(Cylinder, GLU_OUTSIDE);

}

// Desenha um O e gira caso o jogador ganhe
void Draw_O(int x, int y, int z, int a) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(a, 0, 1, 0);
  glutSolidTorus(0.5, 2.0, 30, 30);
  glPopMatrix();
}

// Desenha um X e gira caso o computador ganhe
void Draw_X(int x, int y, int z, int a) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glPushMatrix();
  glRotatef(a, 0, 1, 0);
  glRotatef(90, 0, 1, 0);
  glRotatef(45, 1, 0, 0);
  glTranslatef(0, 0, -3);
  gluCylinder(Cylinder, 0.5, 0.5, 6, 16, 16);
  glPopMatrix();
  glPushMatrix();
  glRotatef(a, 1, 0, 0);
  glRotatef(90, 0, 1, 0);
  glRotatef(315, 1, 0, 0);
  glTranslatef(0, 0, -3);
  gluCylinder(Cylinder, 0.5, 0.5, 6, 16, 16);
  glPopMatrix();
  glPopMatrix();
}

void display(void) {
  
  // Se a ajuda tiver sido selecionada
  if (screen_option == 3) {
    //Limpa a tela e define a cor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);
    
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_COLOR_MATERIAL);
    glColor3f(1.0, 1.0, 1.0);
    Sprint(-3, -2, "Para iniciar, clique com o botao direito");
    Sprint(-3, -5, "E clique em Iniciar");
    glutSwapBuffers();
  } 
  
  // Tela de inicio
  else if (screen_option == 0) {

    // Limpa a tela
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_COLOR_MATERIAL);
    glColor3f(1.0, 1.0, 1.0);
    Sprint(-2, 3,  "Jogo da Velha");
    Sprint(-3, -1, "Gabriel Faga Monteiro");
    Sprint(2, -1, "43560");
    Sprint(-3, -5, "Comece com o botao direito");

    glutSwapBuffers();
  } 

  // Jogo rolando
  else {
    int ix, iy;
    int i; 
    int j; 

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 30.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDisable(GL_COLOR_MATERIAL);
    glColor3f(1.0, 0.0, 0.0);
    init_light();
    // Fim de jogo
    if (win == 1) Sprint(2, 8, "Vitoria do Jogador!");
    if (win == -1) Sprint(2, 8, "Vitoria do Computador!");
    if (win == 2) Sprint(2, 8, "Deu Velha!");
    
    // Visao Perspectiva
    if (view_state == 1) {
      glColor3f(1.0, 0.0, 0.0);
      Sprint(-3, 8, "Visao Perspectiva");
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(60, 1, 1, 30);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
    }
    // Visao Ortho
    else {
      glColor3f(1.0, 0.0, 0.0);
      Sprint(-2, 8, "Visao Ortho");
    }

    gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
    
    // Desenha o tabuleiro
    for (ix = 0; ix < 4; ix++) {
      glPushMatrix();
      glColor3f(1, 1, 1);
      glBegin(GL_LINES);
        glVertex2i(-9, -9 + ix * 6);
        glVertex2i(9, -9 + ix * 6);
      glEnd();
      glPopMatrix();
    }
    for (iy = 0; iy < 4; iy++) {
      glPushMatrix();
      glColor3f(1, 1, 1);
      glBegin(GL_LINES);
        glVertex2i(-9 + iy * 6, 9);
        glVertex2i(-9 + iy * 6, -9);
      glEnd();
      glPopMatrix();
    }
    
    // Desenha o objeto no quadrante
    for (i = 0; i < 9; i++) {
      j = 0;
      if (abs(win) == 1) {
        if ((i == box[spinboxes][0]) || (i == box[spinboxes][1]) || (i == box[spinboxes][2])) {
          j = spin;
        } else j = 0;
      }
      if (box_map[i] == 1) Draw_X(object_map[i][0], object_map[i][1], -1, j);
      if (box_map[i] == -1) Draw_O(object_map[i][0], object_map[i][1], -1, j);
    }

    glutSwapBuffers();
  }
}

void reshape(int w, int h) {
  Win_x = w;
  Win_y = h;
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}


void keyboard(unsigned char key, int x, int y) {
  
  switch (key) {
    // Alterna a visao
    case 'v':
    case 'V':
      view_state = abs(view_state - 1);
      break;

    case 27:
      exit(0); 
      break;
    default:
      break;
  }
}


void mouse(int button, int state, int x, int y) {

  mouse_x = (18 * (float)((float) x / (float) Win_x)) / 6;
  mouse_y = (18 * (float)((float) y / (float) Win_y)) / 6;
  
  object_select = mouse_x + mouse_y * 3;

  if (start_game == 0) {
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
      player = -1;
      computer = 1;
      init_game();
      return;
    }
  }
  if (start_game == 1) {
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
      if (win == 0) {
        if (box_map[object_select] == 0) {
          box_map[object_select] = player;
          win = check_move();
          if (win == 1) {
            start_game = 0;
            return;
          }
          computer_move();
          win = check_move();
          if (win == 1) {
            win = -1;
            start_game = 0;
          }
        }
      }
    }
    }
  if (win == 2) start_game = 0;
}


void menu(int choice) {
  switch (choice) {
  // Iniciar um jogo
  case 1:
    screen_option = 1;
    glutMouseFunc(mouse);
    break;
  // Mudar a visao
  case 2:
    view_state = abs(view_state - 1);
    break;
  // Tela de ajuda
  case 3:
    screen_option = 3;
    glutMouseFunc(mouse);
    break;
  // Encerra
  case 4:
    exit(0);
    break;
  }
}


int main(int argc, char ** argv) {

  // Basico
  glutInit( & argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(850, 600);
  glutInitWindowPosition(10, 10);
  glutCreateWindow(argv[0]);
  glutSetWindowTitle("Jogo da Velha - 43560");
  init();

  glutCreateMenu(menu);
  glutAddMenuEntry("Iniciar", 1);
  glutAddMenuEntry("Alterar Visao", 2);
  glutAddMenuEntry("Ajuda", 3);
  glutAddMenuEntry("Sair", 4);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  glutTimerFunc(50, TimeEvent, 1);
  
  glutMainLoop();
  return 0;
}

//  ********************
//  ********************
