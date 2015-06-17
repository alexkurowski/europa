#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdint.h>
#include "memory.h"


#define MAX_SIZE 0xFFFF


typedef struct {
  uint8_t command;
  int32_t argument;
} Instruction;

typedef struct {
  std::string name;
  uint32_t pos;
} Label;

typedef struct {
  uint32_t id;
  std::string name;
} Jump;

class Program {
  public:
    Program(Memory*);
    ~Program();

    void load(const char*);

    void update();

    bool isLoaded();

  private:
    Memory* mem;

    bool compile(std::vector<std::string>*, uint32_t);

    bool addInstruction(std::string, std::string);
    void addLabel(std::string, uint32_t);
    uint32_t fetchJump(std::string);

    bool loaded;

    std::vector<Instruction> set;
    std::vector<Label>       labels;
    std::vector<Jump>        jumpMap;
    uint32_t pc;


    void MOV();

    void PSH();
    void POP();
    void SWP();

    void ADD();
    void MUL();
    void DIV();

    void NEG();

    void AND();
    void BOR();
    void XOR();

    void IFZ();
    void IFN();
    void IFG();

    void JMP();
    void RET();
};
