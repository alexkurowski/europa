#include "memory.h"

Memory::Memory() {}
Memory::~Memory() {}

//=============================================================================


//=============================================================================

uint8_t Memory::screenMode() {
  if (m[SCREEN_MODE] == 0)
    return 0; // shell
  else
    return 1; // bitmap
}

colorMap Memory::getColors() {
  p = COLORS;
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 3; j++) {
      colors[i][j] = m[p++];
    }
  }
  return colors;
}

colorBit Memory::getCurrentColors() {
  currentColors[0] = getRightByte(m[CURRENT_COLORS]);
  currentColors[1] = getLeftByte(m[CURRENT_COLORS]);
  return currentColors;
}

uint8_t* Memory::fontAddress() {
  return &m[FONT];
}

uint8_t* Memory::screenAddress() {
  return &m[SCREEN_ADDRESS];
}

//=============================================================================

void Memory::buildDefaultRAM() {
 for (int i = 0; i < MEMORY_SIZE; i++)
   m[i] = 0;

 p = FONT;
 for (int i = 0; i < (sizeof(originalFont)/sizeof(*originalFont)); i++)
   m[p++] = originalFont[i];

 p = CURRENT_COLORS;
 m[p++] = originalColor;

 p = COLORS;
 for (int i = 0; i < 16; i++)
   for (int j = 0; j < 3; j++)
     m[p++] = originalColors[i][j];

 p = SCREEN_MODE;
 m[p] = 0;

 // DEBUG SCREEN CONTENT
 // for (int i = SCREEN_ADDRESS; i < CURRENT_COLORS; i++) {
   // memory[i] = 64 + i - SCREEN_ADDRESS;
   // memory[i] = 0;
 // }

}
