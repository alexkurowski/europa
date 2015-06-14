#include "data.h"

uint8_t getBitAt(uint8_t val, uint8_t pos) {
  return (val >> pos) & 0x1;
}

uint8_t getLeftByte(uint8_t val) {
  return val >> 4;
}

uint8_t getRightByte(uint8_t val) {
  return val & 0x0F;
}
