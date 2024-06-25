#include "raylib.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_CAPACITY 200
#define SCREEN_W 800
#define SCREEN_H 450
#define SCREEN_TOP 20
#define REC_HEIGHT 22
#define REC_MARGIN 20
#define REC_PAD 8
#define FONT_NORMAL 10
#define FONT_NORMAL_SPACING 2

typedef struct Register {
  int width;
  int height;
  char *text;
} Register;

void CreateRegister(Register *newReg, int w, int h, char *c) {
  newReg->width = w;
  newReg->height = h;
  newReg->text = malloc(strlen(c) + 1);
  if (newReg->text == NULL) {
    printf("Error: memory allocation failed\n");
    return;
  }

  strcpy(newReg->text, c);
}

typedef struct ParentIdx {
  int x;
  int y;
} ParentIdx;

typedef struct {
  Register pages[ARRAY_CAPACITY];
  ParentIdx parents[ARRAY_CAPACITY];
  size_t countPages;
  int level;
} Levels;

typedef struct App {
  TipoApontador dict;

  Font font;
  Levels renderLevels[ARRAY_CAPACITY];
  int countRenderLevels;
} App;

void InitializeApp(App *p) {
  Inicializa(&p->dict);
  p->countRenderLevels = 0;
}

int FindRenderLevelIndex(App p, int level) {
  int existis = -1;
  for (int i = 0; i < p.countRenderLevels; i++) {
    if (level == p.renderLevels[i].level) {
      existis = i;
    }
  }

  return existis;
}

int CalculatePageTextWidth(App *pa, TipoApontador p) {
  int recSize = 0;
  for (int i = 0; i < p->n; i++) {
    int textSize = MeasureTextEx(pa->font, p->r[i].Chave.nome, FONT_NORMAL,
                                 FONT_NORMAL_SPACING)
                       .x;
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

void TraverseAndStorePageInfo(App *pa, TipoApontador p, int nivel, ParentIdx parent) {
  if (p == NULL) {
    return;
  }

  int recSize = CalculatePageTextWidth(pa, p);
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
  CreateRegister(&newReg, recSize + (REC_PAD * 2), REC_HEIGHT, pageValues);

  int item = FindRenderLevelIndex(*pa, nivel);

  // ainda nao existe rapaz
  if (item == -1) {
    item = pa->countRenderLevels;
    pa->renderLevels[item].level = nivel;
    pa->renderLevels[item].countPages = 0;
    pa->countRenderLevels++;
  }
  pa->renderLevels[item].pages[pa->renderLevels[item].countPages] = newReg;
  pa->renderLevels[item].parents[pa->renderLevels[item].countPages] = parent;
  pa->renderLevels[item].countPages++;

  ParentIdx newParentIdx = {item, pa->renderLevels[item].countPages - 1};
  for (int i = 0; i < p->n; i++) {
    TraverseAndStorePageInfo(pa, p->p[i], nivel + 1, newParentIdx);
  }

  TraverseAndStorePageInfo(pa, p->p[p->n], nivel + 1, newParentIdx);
}

int MeasureLevel(Levels p) {
  int size = 0;

  for (int i = 0; i < p.countPages; i++) {
    size += p.pages[i].width;
  }

  return size;
}

void RenderBtree(App app) {
  int placeX, placeY;
  Register tmp;

  for (int i = 0; i < app.countRenderLevels; i++) {

    // calculating the level initial pos
    int halfPageSize = MeasureLevel(app.renderLevels[i]) / 2;
    placeX = (SCREEN_W * 0.5) - halfPageSize;
    placeY = SCREEN_TOP + (i * (REC_HEIGHT + REC_MARGIN));

    for (int j = 0; j < app.renderLevels[i].countPages; j++) {
      tmp = app.renderLevels[i].pages[j];
      DrawRectangle(placeX, placeY, tmp.width, tmp.height, GRAY);
      DrawRectangleLines(placeX, placeY, tmp.width, tmp.height, DARKGRAY);

      Vector2 text =
          MeasureTextEx(app.font, tmp.text, FONT_NORMAL, FONT_NORMAL_SPACING);
      DrawText(tmp.text, placeX + REC_PAD,
               placeY + (int)(tmp.height / 2) - (int)(text.y / 2), FONT_NORMAL,
               BLACK);

      if (i > 0) {
        ParentIdx parent = app.renderLevels[i].parents[j];
        // calculating the parent pos
        int parentX = (SCREEN_W * 0.5) -
                      (int)(MeasureLevel(app.renderLevels[parent.x]) / 2);
        for (int k = 0; k < parent.y; k++) {
          parentX += app.renderLevels[parent.x].pages[k].width + 10;
        }
        int parentY = SCREEN_TOP + ((i - 1) * (REC_HEIGHT + REC_MARGIN));
        DrawLine(placeX + (tmp.width / 2), placeY,
                 parentX +
                     (app.renderLevels[parent.x].pages[parent.y].width / 2),
                 parentY + REC_HEIGHT, DARKGRAY);
      }

      placeX = placeX + tmp.width + 10;
    }
  }
}

void print_usage(const char *prog_name) {
  fprintf(stderr, "Usage: %s [--help] [--debug] [--file path]\n", prog_name);
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  int debug_flag = 0;
  char *fromFile;
  fromFile = NULL;

  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "--debug") == 0) {
      debug_flag = 1;
    }
    if (strcmp(argv[i], "--help") == 0) {
      print_usage("./bin/app");
    }
    if (strcmp(argv[i], "--file") == 0) {
      fromFile = malloc(sizeof(argv[i + 1] + 1));
      strcpy(fromFile, argv[i + 1]);
    }
  }

  if (fromFile == NULL) {
    fromFile = malloc(sizeof(char) * 21);
    strcpy(fromFile, "../teste.txt\0");
  }

  App p;
  FILE *arqReg;
  TipoRegistro x;
  int compIns = 10;

  InitializeApp(&p);

  arqReg = fopen("../teste.txt", "r+");

  if (arqReg == NULL) {
    perror("Error trying to open file!\n");
    exit(EXIT_FAILURE);
  }

  while (fscanf(arqReg, "%s %s", x.Chave.nome, x.Chave.cpf) != EOF) {
    Insere(x, &p.dict, &compIns);
  }

  if (debug_flag) {
    printf("B-Tree\n");
    Imprime(p.dict);
    printf("\nEnd of B-Tree\n");
  }

  InitWindow(SCREEN_W, SCREEN_H, "B-Tree");

  // NOTE: Textures MUST be loaded after Window initialization (OpenGL context
  // is required)
  p.font = LoadFont("../raylib/examples/text/resources/pixantiqua.png");
  ParentIdx rootParent = {-1, -1};
  TraverseAndStorePageInfo(&p, p.dict, 0, rootParent);
  SetTargetFPS(60);

  if (debug_flag) {
    printf("Render array = {count: %d}\n", p.countRenderLevels);
    for (int i = 0; i < p.countRenderLevels; i++) {
      for (int j = 0; j < p.renderLevels[i].countPages; j++) {
        if (p.renderLevels[i].pages[j].text != NULL) {
          Register reg = p.renderLevels[i].pages[j];
          printf("array[%d][%d] = {text: %s, width: %d}\n", i, j, reg.text,
                 reg.width);
        } else {
          printf("array[%d][%d] = {text: NULL}\n", i, j);
        }
      }
    }
  }

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    RenderBtree(p);

    EndDrawing();
  }

  UnloadFont(p.font);

  return 0;
}