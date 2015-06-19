#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdint.h>
#include "memory.h"


#define MAX_SIZE 0xFFFF

#define CYCLES_PER_UPDATE 100


typedef struct {
  std::string command;
  int32_t argument;
} Instruction;

typedef struct {
  uint32_t    id;
  std::string name;
  uint32_t    pos;
} Label;

class Program {
  public:
    Program(Memory*);
    ~Program();

    void load(std::string);

    void update();

    bool isLoaded();

  private:
    Memory* mem;

    bool     compile(std::vector<std::string>*, uint32_t);

    bool     addInstruction(std::string, std::string);

    void     setLabelPosition(std::string, uint32_t);
    uint32_t getLabelId(std::string);
    uint32_t getLabelPos(uint32_t);

    bool     argIsAddress();
    bool     argIsNull();
    uint32_t argGetAddress();
    uint32_t argument();

    void     stackEnsure(uint8_t);
    uint16_t stackTop();
    uint16_t stackSecond();
    void     stackPush(uint16_t);
    uint16_t stackPop();
    void     stackSwap();
    void     stackRemove();
    void     stackLimit();



    bool loaded;

    std::vector<Instruction> set;
    std::vector<Label>       labels;
    std::vector<int16_t>     stack;
    uint32_t pc;
    uint32_t ret;

    uint8_t  t8;
    uint16_t t16;

    uint32_t cycle;

    void MOV();

    void PSH();
    void POP();
    void SWP();
    void REM();

    void ADD();
    void SUB();
    void MUL();
    void DIV();

    void NEG();

    void AND();
    void NOR();

    void IFZ();
    void IFN();
    void IFG();
    void IFL();

    void JMP();

    void JSR();
    void RET();
};
