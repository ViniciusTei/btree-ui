#include "arraylist.h"
#include "raylib.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SCREEN_TOP 20
#define REC_HEIGHT 30
#define FONT_NORMAL 20

typedef struct Register {
  int placeX;
  int placeY;
  int width;
  int height;
  char *text;
} Register;

Register MakeRegister(int x, int y, int w, int h, char *c) {
  Register r2;
  r2.placeX = x;
  r2.placeY = y;
  r2.text = c;
  return r2;
}

typedef struct App {
  int screenWidth;
  int screenHeight;
  TipoApontador dict;
  ArrayList renderedPageSizes;
} App;

void InitializePane(App *p) {
  p->screenHeight = 450;
  p->screenWidth = 800;
  Inicializa(&p->dict);
  initializeArrayList(&p->renderedPageSizes);
}

int MeasurePageText(TipoApontador p) {
  int recSize = 0;
  for (int i = 0; i < p->n; i++) {
    recSize = MeasureText(p->r[i].Chave.nome, FONT_NORMAL) + recSize;
  }
  return recSize;
}

int CalculateRecSize(TipoApontador p) { return MeasurePageText(p); }

void CalculatePageSizes(App *pa, TipoApontador p, int nivel) {
  char *pageValues;
  ArrayList pages;

  if (p == NULL) {
    return;
  }

  int recSize = MeasurePageText(p);
  pageValues = (char *)malloc(sizeof(char) * recSize);
  for (int i = 0; i < p->n; i++) {
    strcat(pageValues, p->r[i].Chave.nome);
    strcat(pageValues, ", ");
  }
  
  Register newReg = MakeRegister(0, 0, recSize, REC_HEIGHT, pageValues);
  void *item = atArrayList(&pa->renderedPageSizes, nivel);
  if (item == NULL) {
    initializeArrayList(&pages);
    pushItemArrayList(&pages, &newReg, sizeof(newReg));
    insertAtArrayList(&pa->renderedPageSizes, nivel, &pages, sizeof(pages));
  } else {
    pushItemArrayList((ArrayList*) item, &newReg, sizeof(newReg));
    insertAtArrayList(&pa->renderedPageSizes, nivel, (ArrayList*) item, sizeof(item));
  }

  for (int i = 0; i < p->n; i++) {
    CalculatePageSizes(pa, p->p[i], nivel + 1);
  }
}

void DrawTree(App app) {
  int placeX, placeY;
  Register tmp;

  for (int i = 0; i < app.renderedPageSizes.last; i++) {
    placeX = app.screenWidth * 0.5;
    placeY = 0 + (i * REC_HEIGHT);

    ArrayList *pageItems = (ArrayList*) app.renderedPageSizes.array[i].item;
    for (int j = 0; j < pageItems->last; j++) {
      tmp = *(Register *) pageItems->array[j].item;
      DrawRectangle(placeX, placeY, tmp.width, tmp.height, DARKGRAY);
      DrawText(tmp.text, placeX, placeY, FONT_NORMAL, LIGHTGRAY); 
      placeX = placeX + tmp.width + 10;
    }
  }
}

int main(int argc, char *argv[]) {
  App p;
  FILE *arqReg;
  TipoRegistro x;
  int compIns = 0;

  InitializePane(&p);

  arqReg = fopen("../teste.txt", "r+");

  if (arqReg == NULL) {
    perror("Error trying to open file");
  }

  while (fscanf(arqReg, "%s %s", x.Chave.nome, x.Chave.cpf) != EOF) {
    Insere(x, &p.dict, &compIns);
  }

  // calculate all the nodes
  CalculatePageSizes(&p, p.dict, 0);

  InitWindow(p.screenWidth, p.screenHeight, "Hello world");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawTree(p);

    EndDrawing();
  }
  return 0;
}
