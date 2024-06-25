#include "textinput.h"
#include "raylib.h"
#include "string.h"

void CreateTextInput(TextInput *t, int x, int y, char *label) {
  strcpy(t->value, "\0");
  strcpy(t->label, label);
  t->letterCount = 0;
  t->mouseOnText = false;
  t->textBox = (Rectangle){x, y, 80, 15};
  t->framesCounter = 0;

  t->mouseOnButton = false;
  t->button = (Rectangle){x + 82, y, 18, 15};
}

void UpdateTextInput(TextInput *t) {
  if (CheckCollisionPointRec(GetMousePosition(), t->textBox))
    t->mouseOnText = true;
  else
    t->mouseOnText = false;

  if (CheckCollisionPointRec(GetMousePosition(), t->button))
    t->mouseOnButton = true;
  else
    t->mouseOnButton = false;

  if (t->mouseOnText) {
    // Set the window's cursor to the I-Beam
    SetMouseCursor(MOUSE_CURSOR_IBEAM);

    // Get char pressed (unicode character) on the queue
    int key = GetCharPressed();

    // Check if more characters have been pressed on the same frame
    while (key > 0) {
      // NOTE: Only allow keys in range [32..125]
      if ((key >= 32) && (key <= 125) && (t->letterCount < MAX_INPUT_CHARS)) {
        t->value[t->letterCount] = (char)key;
        t->value[t->letterCount + 1] =
            '\0'; // Add null terminator at the end of the string.
        t->letterCount++;
      }

      key = GetCharPressed(); // Check next character in the queue
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
      t->letterCount--;
      if (t->letterCount < 0)
        t->letterCount = 0;
      t->value[t->letterCount] = '\0';
    }
  } else
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

  t->mouseOnText ? t->framesCounter++ : (t->framesCounter = 0);
}

void RenderTextInput(TextInput *t) {
  DrawText(t->label, t->textBox.x, t->textBox.y - t->textBox.height, 8, GRAY);

  DrawRectangleRec(t->textBox, LIGHTGRAY);
  if (t->mouseOnText)
    DrawRectangleLines((int)t->textBox.x, (int)t->textBox.y, (int)t->textBox.width,
                       (int)t->textBox.height, RED);
  else
    DrawRectangleLines((int)t->textBox.x, (int)t->textBox.y, (int)t->textBox.width,
                       (int)t->textBox.height, DARKGRAY);

  Color buttonColor;
  buttonColor = t->mouseOnButton ? RED : DARKGRAY;
  DrawRectangleRec(t->button, LIGHTGRAY);
  DrawRectangleLines((int)t->button.x, (int)t->button.y, (int)t->button.width,
                       (int)t->button.height, buttonColor);
  DrawText("OK", (int)t->button.x + 3, (int)t->button.y + 2, 2, MAROON);

  DrawText(t->value, (int)t->textBox.x + 5, (int)t->textBox.y + 2, 12, MAROON);

  if (t->mouseOnText) {
    if (t->letterCount < MAX_INPUT_CHARS) {
      // Draw blinking underscore char
      if (((t->framesCounter / 20) % 2) == 0)
        DrawText("_", (int)t->textBox.x + 8 + MeasureText(t->value, 12),
                 (int)t->textBox.y + 2, 8, MAROON);
    } else
      DrawText("10/10", t->textBox.x, t->textBox.y + t->textBox.height + 4, 8, GRAY);
  }
}

void ResetTextInput(TextInput *t);
