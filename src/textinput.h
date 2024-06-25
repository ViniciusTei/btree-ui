#ifndef TEXTINPUT
#define TEXTINPUT

#include "raylib.h"

/*
 * This code is directly copyied from the Raylib examples
 * https://www.raylib.com/examples/text/loader.html?name=text_input_box
 */
#define MAX_INPUT_CHARS 10

typedef struct {
  char value[MAX_INPUT_CHARS + 1]; // NOTE: One extra space required for null terminator char '\0'
  int letterCount;

  Rectangle textBox;
  bool mouseOnText;
  int framesCounter; //NOTE: used to blink the cursor
  char label[MAX_INPUT_CHARS * 2]; // NOTE: One extra space required for null terminator char '\0'
  
  Rectangle button;
  bool mouseOnButton;
} TextInput;

void CreateTextInput(TextInput *t, int x, int y, char *label);
void UpdateTextInput(TextInput *t);
void RenderTextInput(TextInput *t);
void ResetTextInput(TextInput *t);

#endif // TEXTINPUT
