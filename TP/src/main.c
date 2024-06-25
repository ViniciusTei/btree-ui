#include "raylib.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_CAPACITY 200
#define SCREEN_TOP 20
#define REC_HEIGHT 30
#define FONT_NORMAL 20

typedef struct Register {
  int width;
  int height;
  char *text;
} Register;

void MakeRegister(Register *newReg, int w, int h, char *c) {
  newReg->width = w;
  newReg->height = h;
  newReg->text = malloc(strlen(c) + 1);
  if (newReg->text == NULL) {
    printf("Error: memory allocation failed\n");
    return;
  }

  strcpy(newReg->text, c);
}

typedef struct {
  Register pages[ARRAY_CAPACITY];
  int countPages;
  int level;
} Levels;

typedef struct App {
  int screenWidth;
  int screenHeight;
  TipoApontador dict;
  Font font;
  Levels renderLevels[ARRAY_CAPACITY];
  int countRenderLevels;
} App;

void InitializePane(App *p) {
  p->screenHeight = 450;
  p->screenWidth = 800;
  Inicializa(&p->dict);
  p->countRenderLevels = 0;
}

int indexOfLevel(App p, int level) {
  int existis = -1;
  for (int i = 0; i < p.countRenderLevels; i++) {
    if (level == p.renderLevels[i].level) {
      existis = i;
    }
  }

  return existis;
}


int MeasurePageText(App *pa, TipoApontador p) {
  int recSize = 0;
  for (int i = 0; i < p->n; i++) {
    int textSize =
        MeasureTextEx(pa->font, p->r[i].Chave.nome, FONT_NORMAL, 10).x;
    recSize = textSize + recSize;
  }
  return recSize;
}

Register *atRegisterArray(Register **arr, int level) {
  if (arr == NULL) {
    return NULL;
  }

  return arr[level];
}

void CalculatePageSizes(App *pa, TipoApontador p, int nivel) {
  if (p == NULL) {
    return;
  }

  int recSize = MeasurePageText(pa, p);
  char *pageValues = malloc(sizeof(char) * recSize);
  pageValues[0] = '\0';

  for (int i = 0; i < p->n; i++) {
    strcat(pageValues, p->r[i].Chave.nome);
    strcat(pageValues, ", ");
  }

  int len = strlen(pageValues);
  if (len > 2) {
    pageValues[len - 2] = '\0';
  }

  Register newReg;
  MakeRegister(&newReg, recSize, REC_HEIGHT, pageValues);

  int item = indexOfLevel(*pa, nivel);
  
  //ainda nao existe rapaz
  if (item == -1) {
    item = pa->countRenderLevels;
    pa->renderLevels[item].level = nivel;
    pa->renderLevels[item].countPages = 0;
    pa->renderLevels[item].pages[pa->renderLevels[item].countPages] = newReg;
    pa->renderLevels[item].countPages++;
    pa->countRenderLevels++;
  } else {
    pa->renderLevels[item].pages[pa->renderLevels[item].countPages] = newReg;
    pa->renderLevels[item].countPages++;
  }

  for (int i = 0; i < p->n; i++) {
    CalculatePageSizes(pa, p->p[i], nivel + 1);
  }
}

int MeasureLevel(Levels p) {
  int size = 0;

  for (int i = 0; i < p.countPages; i++) {
    size += p.pages[i].width;
  }

  return size;
}

void DrawTree(App app) {
  int placeX, placeY;
  Register tmp;

  for (int i = 0; i < app.countRenderLevels; i++) {
   
    // calculating the level initial pos
    int halfPageSize = MeasureLevel(app.renderLevels[i]) / 2;
    placeX = (app.screenWidth * 0.5) - halfPageSize;
    placeY = 0 + (i * (REC_HEIGHT + 20));
    
    for (int j = 0; j < app.renderLevels[i].countPages; j++) {
      tmp = app.renderLevels[i].pages[j];
      DrawRectangle(placeX, placeY, tmp.width, tmp.height, DARKGRAY);

      Vector2 text = MeasureTextEx(app.font, tmp.text, FONT_NORMAL, 10);
      DrawText(tmp.text, placeX + 10,placeY + (int)(tmp.height/2) - (int)(text.y/2), FONT_NORMAL, LIGHTGRAY);

      //TODO: FIX
      //draw a line to the parent
      if (i > 0) {
        // calculating the parent pos
        int parentX = (app.screenWidth * 0.5);
        int parentY = REC_HEIGHT + ((i-1) * (REC_HEIGHT + 20));
        DrawLine(placeX, placeY, parentX, parentY, DARKGRAY);
      }
      
      placeX = placeX + tmp.width + 10;
    }
  }
}

int main(int argc, char *argv[]) {
  App p;
  FILE *arqReg;
  TipoRegistro x;
  int compIns = 10;

  InitializePane(&p);

  arqReg = fopen("../teste.txt", "r+");

  if (arqReg == NULL) {
    perror("Error trying to open file");
  }

  while (fscanf(arqReg, "%s %s", x.Chave.nome, x.Chave.cpf) != EOF) {
    Insere(x, &p.dict, &compIns);
  }

  InitWindow(p.screenWidth, p.screenHeight, "Hello world");

  // NOTE: Textures MUST be loaded after Window initialization (OpenGL context
  // is required)
  p.font = LoadFont("../raylib/examples/text/resources/pixantiqua.png");
  CalculatePageSizes(&p, p.dict, 0);
  SetTargetFPS(60);

  printf("array = {count: %d}\n", p.countRenderLevels);
  for (int i = 0; i < p.countRenderLevels; i++) {
    for (int j = 0; j < p.renderLevels[i].countPages; j++) {
      if (p.renderLevels[i].pages[j].text != NULL) {
        Register reg = p.renderLevels[i].pages[j];
        printf("array[%d][%d] = {text: %s, width: %d}\n", i, j, reg.text, reg.width);
      } else {
        printf("array[%d][%d] = {text: NULL}\n", i, j);
      }
    }
  }

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawTree(p);

    EndDrawing();
  }

  UnloadFont(p.font);

  return 0;
}
