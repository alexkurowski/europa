#include "program.h"

Program::Program(Memory* m) {
  mem = m;
  loaded = false;
}

Program::~Program() {}

//=============================================================================

void Program::load(const char* filename) {
  std::vector<std::string> content;
  std::ifstream file(filename);
  if (file == NULL) return;

  uint32_t size;
  for (size = 0; file.good(); size++) {
    content.push_back("");
    getline(file, content[size]);
  }

  if (compile(&content, size))
    loaded = true;
}

bool Program::compile(std::vector<std::string>* str, uint32_t size) {
  set.clear();
  labels.clear();
  jumpMap.clear();

  std::string whitespace = " \t\f\v\n\r";
  std::string comment    = "#;/\"";

  std::string instruction;
  std::string argument;

  std::string t;
  size_t textStart,
         textEnd,
         commentStart;

  uint32_t i = 0;
  uint32_t line;

  for (line = 0; line < size; line++) {
    textStart    = str->at(line).find_first_not_of(whitespace);
    commentStart = str->at(line).find_first_of(comment);
    textEnd      = str->at(line).find_last_not_of(whitespace, commentStart - 1);

    if (textStart == std::string::npos ||
        textEnd   == std::string::npos ||
        textStart == textEnd ||
        textStart >= commentStart ||
        commentStart == 0)
      continue;

    t = str->at(line).substr(textStart, textEnd - textStart + 1);

    if (t.find_first_of(whitespace) == std::string::npos) {
      instruction = t;
      argument    = "";
    } else {
      instruction = t.substr(0, t.find_first_of(whitespace));
      argument    = t.substr(t.find_last_of(whitespace) + 1, t.length() - t.find_last_of(whitespace) - 1);
    }

    if (instruction.back() == ':') {
      addLabel(instruction, i);
    } else {
      if (addInstruction(instruction, argument))
        i++;
      else
        return false;
    }

    std::cout << instruction << '=' << argument << std::endl;
  }

  return true;
}

bool Program::addInstruction(std::string instruction, std::string argument) {
  uint8_t i;
  int32_t a;
  if (instruction == "MOV") i = 0;
  else
  if (instruction == "PSH") i = 1;
  else
  if (instruction == "POP") i = 2;
  else
  if (instruction == "SWP") i = 3;
  else
  if (instruction == "ADD") i = 4;
  else
  if (instruction == "MUL") i = 5;
  else
  if (instruction == "DIV") i = 6;
  else
  if (instruction == "NEG") i = 7;
  else
  if (instruction == "AND") i = 8;
  else
  if (instruction == "BOR") i = 9;
  else
  if (instruction == "XOR") i = 10;
  else
  if (instruction == "IFZ") i = 11;
  else
  if (instruction == "IFN") i = 12;
  else
  if (instruction == "IFG") i = 13;
  else
  if (instruction == "JMP") i = 14;
  else
  if (instruction == "RET") i = 15;
  else
    return false;

  if (argument.empty()) {
    a = 0;
  } else
  if (instruction == "JMP") {
    a = fetchJump(argument);
  } else
  if (argument.at(0) == '$') {
    a = 1000000 + std::stoi( argument.substr(1, argument.length() - 1) );
  } else {
    a = std::stoi(argument);
  }

  set.push_back(Instruction {i, a});
  return true;
}

void Program::addLabel(std::string name, uint32_t p) {
  labels.push_back(Label {name, p});
}

uint32_t Program::fetchJump(std::string name) {
  for (int i = 0; i < jumpMap.size(); i++) {
    if (jumpMap[i].name == name)
      return jumpMap[i].id;
  }
  jumpMap.push_back(Jump {static_cast<uint32_t>(jumpMap.size()), name});
  return jumpMap.size() - 1;
}

//=============================================================================

void Program::update() {
  std::cout << "PROG UPDATE" << std::endl;
}

//=============================================================================

bool Program::isLoaded() {
  return loaded;
}

//=============================================================================

void Program::MOV() {
}

void Program::PSH() {
}

void Program::POP() {
}

void Program::SWP() {
}

void Program::ADD() {
}

void Program::MUL() {
}

void Program::DIV() {
}

void Program::NEG() {
}

void Program::AND() {
}

void Program::BOR() {
}

void Program::XOR() {
}

void Program::IFZ() {
}

void Program::IFN() {
}

void Program::IFG() {
}

void Program::JMP() {
}

void Program::RET() {
}
