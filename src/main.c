#include "arvore.h"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "textinput.h"
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

void ResetRenderLevels(App *app) {
  // Iterate through each level
  for (int i = 0; i < app->countRenderLevels; i++) {
    Levels *level = &app->renderLevels[i];

    // Free the memory allocated for each page text
    for (int j = 0; j < level->countPages; j++) {
      free(level->pages[j].text);
      level->pages[j].text = NULL;
    }

    // Reset the count of pages in the level
    level->countPages = 0;
    level->level = 0;
  }

  // Reset the total count of render levels
  app->countRenderLevels = 0;
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

void TraverseAndStorePageInfo(App *pa, TipoApontador p, int nivel,
                              ParentIdx parent) {
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
    int fullPageSize = MeasureLevel(app.renderLevels[i]);
    int halfPageSize = fullPageSize / 2;
    placeX = (SCREEN_W * 0.5) - halfPageSize;
    placeY = SCREEN_TOP + (i * (REC_HEIGHT + REC_MARGIN));

    for (int j = 0; j < app.renderLevels[i].countPages; j++) {
      tmp = app.renderLevels[i].pages[j];
      DrawRectangle(placeX, placeY, (int)(tmp.width),
                    (int)(tmp.height), GRAY);
      DrawRectangleLines(placeX, placeY, (int)(tmp.width),
                         (int)(tmp.height), DARKGRAY);

      Vector2 text =
          MeasureTextEx(app.font, tmp.text, FONT_NORMAL, FONT_NORMAL_SPACING);
      DrawText(tmp.text, placeX + REC_PAD,
               placeY + (int)((tmp.height) / 2) - (int)(text.y / 2),
               FONT_NORMAL, BLACK);

      if (i > 0) {
        ParentIdx parent = app.renderLevels[i].parents[j];
        // calculating the parent pos
        int parentX = (SCREEN_W * 0.5) -
                      (int)(MeasureLevel(app.renderLevels[parent.x]) / 2);
        for (int k = 0; k < parent.y; k++) {
          parentX += (app.renderLevels[parent.x].pages[k].width) + 10;
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
  TextInput input, search, delete;

  InitializeApp(&p);

  CreateTextInput(&input, 10, 400, "Insert");
  CreateTextInput(&search, 120, 400, "Search");
  CreateTextInput(&delete, 230, 400, "Delete");

  arqReg = fopen("../teste.txt", "r+");

  if (arqReg == NULL) {
    perror("Error trying to open file!\n");
    exit(EXIT_FAILURE);
  }

  while (fscanf(arqReg, "%s", x.Chave.nome) != EOF) {
    Insere(x, &p.dict, &compIns);
  }

  if (debug_flag) {
    printf("B-Tree in order\n");
    Imprime(p.dict);
    printf("\nEnd of B-Tree\n");
  }

  InitWindow(SCREEN_W, SCREEN_H, "B-Tree Visualization");

  // setup camera
  Camera2D camera = { 0 };
  camera.target = (Vector2){ SCREEN_W*0.5, SCREEN_H*0.5 };
  camera.offset = (Vector2){ SCREEN_W/2.0f, SCREEN_H/2.0f };
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
  Vector2 prevMousePos = GetMousePosition();

  // NOTE: Textures MUST be loaded after Window initialization (OpenGL context
  // is required)
  p.font = LoadFont("../raylib/examples/text/resources/pixantiqua.png");
  ParentIdx rootParent = {-1, -1};
  SetTargetFPS(60);

  while (!WindowShouldClose()) {

    // UPDATE
    float mouseDelta = GetMouseWheelMove(); 
    float newZoom = camera.zoom + mouseDelta * 0.01f;
    if (newZoom <=0) {
      newZoom = 0.01f;
    }
    camera.zoom = newZoom;

    Vector2 mousePos = GetMousePosition();
    Vector2 delta = Vector2Subtract(prevMousePos, mousePos);
    prevMousePos = mousePos;

    if (IsMouseButtonDown(FALSE)) {
      camera.target = GetScreenToWorld2D(Vector2Add(camera.offset, delta), camera);
    }

    if (IsKeyPressed(KEY_LEFT)) {
      camera.rotation += 10;
    } else if (IsKeyPressed(KEY_RIGHT)) {
      camera.rotation -= 10;
    }

    TraverseAndStorePageInfo(&p, p.dict, 0, rootParent);
    
    UpdateTextInput(&input);
    UpdateTextInput(&search);
    UpdateTextInput(&delete);

    BeginDrawing();
    //CAMERA
    // RENDER
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);
    RenderBtree(p);
    
    EndMode2D();
    // everything after this line wont be affected by the camera
    // footer with controll inputs
    DrawLine(0, 370, SCREEN_W, 370, DARKGRAY);
    RenderTextInput(&input);
    RenderTextInput(&search);
    RenderTextInput(&delete);

    //HELP MENU
    DrawRectangle( 10, 10, 208, 75, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines( 10, 10, 208, 75, BLUE);
    DrawText("Use the mouse to move the canvas!", 20, 20, 10, BLACK);
    DrawText("- Mouse left click to move", 40, 40, 10, DARKGRAY);
    DrawText("- Mouse Wheel to Zoom in-out", 40, 60, 10, DARKGRAY);
    // RESET
    EndDrawing();
    ResetRenderLevels(&p);
  }

  UnloadFont(p.font);
  ResetRenderLevels(&p);
  CloseWindow();

  return 0;
}
