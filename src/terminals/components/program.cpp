#include "program.h"

Program::Program(Memory* m) {
  mem = m;
  loaded = false;
}

Program::~Program() {}

//=============================================================================

void Program::load(std::string filename) {
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

  pc  = 0;
  ret = 0;
}

bool Program::compile(std::vector<std::string>* str, uint32_t size) {
  set.clear();
  labels.clear();
  stack.clear();

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
      setLabelPosition(instruction, i);
    } else {
      if (addInstruction(instruction, argument))
        i++;
      else
        return false;
    }
  }

  return true;
}

bool Program::addInstruction(std::string instruction, std::string argument) {
  int32_t a;

  if (instruction.length() != 3)
    return false;

  if (argument.empty()) {
    a = -1000000;
  } else
  if (instruction == "JMP") {
    a = getLabelId(argument);
  } else
  if (argument.at(0) == '$') {
    const char* ch = argument.substr(1, argument.length() - 1).c_str();
    a = 1000000 + std::strtoul( ch, NULL, 16 );
  } else
  if (argument.at(0) == 'x') {
    const char* ch = argument.substr(1, argument.length() - 1).c_str();
    a = std::strtoul( ch, NULL, 16 );
  } else
  if (argument.at(0) == 'b') {
    const char* ch = argument.substr(1, argument.length() - 1).c_str();
    a = std::strtoul( ch, NULL, 2 );
  } else {
    a = std::stoi(argument);
  }

  set.push_back(Instruction {instruction, a});
  return true;
}

void Program::setLabelPosition(std::string name, uint32_t p) {
  name = name.substr(0, name.length() - 1);
  for (int i = 0; i < labels.size(); i++) {
    if (labels[i].name == name)
      labels[i].pos = p;
  }
  labels.push_back(Label {static_cast<uint32_t>(labels.size()), name, p});
}

uint32_t Program::getLabelId(std::string name) {
  for (int i = 0; i < labels.size(); i++) {
    if (labels[i].name == name)
      return labels[i].id;
  }
  labels.push_back(Label {static_cast<uint32_t>(labels.size()), name, 0});
  return labels.size() - 1;
}

uint32_t Program::getLabelPos(uint32_t id) {
  return labels[id].pos;
}

//=============================================================================

bool Program::argIsAddress() {
  return set[pc].argument >= 1000000;
}

bool Program::argIsNull() {
  return set[pc].argument <= -1000000;
}

uint32_t Program::argGetAddress() {
  return set[pc].argument - 1000000;
}

uint32_t Program::argument() {
  if (argIsNull())    return 0;
  if (argIsAddress()) return argGetAddress();
  return set[pc].argument;
}

//=============================================================================

void Program::stackEnsure(uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    if (stack.size() < i)
      stackPush(0);
  }
}

uint16_t Program::stackTop() {
  stackEnsure(1);
  return stack.at(0);
}

uint16_t Program::stackSecond() {
  stackEnsure(2);
  return stack.at(1);
}

void Program::stackPush(uint16_t value) {
  stack.insert(stack.begin(), value);
  stackLimit();
}

uint16_t Program::stackPop() {
  if (stack.size() < 1) return 0;
  t16 = stack.at(0);
  stackRemove();
  return t16;
}

void Program::stackSwap() {
  stackEnsure(2);
  t16 = stack.at(0);
  stack.at(0) = stack.at(1);
  stack.at(1) = t16;
}

void Program::stackRemove() {
  stack.erase(stack.begin());
}

void Program::stackLimit() {
  while (stack.size() > STACK_SIZE)
    stack.pop_back();
}

//=============================================================================

void Program::update() {
  for (cycle = 0; cycle < CYCLES_PER_UPDATE; cycle++) {
    if (pc < 0 || pc >= set.size()) break;

         if (set[pc].command == "MOV") MOV();
    else if (set[pc].command == "PSH") PSH();
    else if (set[pc].command == "POP") POP();
    else if (set[pc].command == "SWP") SWP();
    else if (set[pc].command == "ADD") ADD();
    else if (set[pc].command == "MUL") MUL();
    else if (set[pc].command == "DIV") DIV();
    else if (set[pc].command == "NEG") NEG();
    else if (set[pc].command == "AND") AND();
    else if (set[pc].command == "NOR") NOR();
    else if (set[pc].command == "IFZ") IFZ();
    else if (set[pc].command == "IFN") IFN();
    else if (set[pc].command == "IFG") IFG();
    else if (set[pc].command == "JMP") JMP();
    else if (set[pc].command == "RET") RET();

    pc++;
  }
}

//=============================================================================

bool Program::isLoaded() {
  return loaded;
}

//=============================================================================

void Program::MOV() {
  if (argIsAddress())
    mem->setAddress(argument());
  else if (argIsNull())
    mem->moveAddress(stackTop());
  else
    mem->moveAddress(argument());
}

void Program::PSH() {
  if (argIsNull())
    stackPush(mem->getByte());
  else
    stackPush(argument());
}

void Program::POP() {
  if (argIsNull())
    mem->setByte(stackPop());
  else
    mem->setByte(argument());
}

void Program::SWP() {
  stackSwap();
}

void Program::REM() {
  stackRemove();
}

void Program::ADD() {
  stackEnsure(1);

  if (argIsNull()) {
    stackEnsure(2);
    stack.at(0) += stack.at(1);
    stack.erase(stack.begin() + 1);
  } else {
    stack.at(0) += argument();
  }
}

void Program::SUB() {
  stackEnsure(1);

  if (argIsNull()) {
    stackEnsure(2);
    stack.at(0) = stack.at(1) - stack.at(0);
    stack.erase(stack.begin() + 1);
  } else {
    stack.at(0) -= argument();
  }
}

void Program::MUL() {
  stackEnsure(1);

  if (argIsNull()) {
    stackEnsure(2);
    stack.at(0) *= stack.at(1);
    stack.erase(stack.begin() + 1);
  } else {
    stack.at(0) *= argument();
  }
}

void Program::DIV() {
  stackEnsure(1);

  if (argIsNull()) {
    stackEnsure(2);
    stack.at(0) = stack.at(1) / stack.at(0);
    stack.erase(stack.begin() + 1);
  } else {
    stack.at(0) /= argument();
  }
}

void Program::NEG() {
  stackEnsure(1);
  stack.at(0) *= -1;
}

void Program::AND() {
  stackEnsure(1);

  if (argIsNull()) {
    stackEnsure(2);
    stack.at(0) = stack.at(1) & stack.at(0);
    stack.erase(stack.begin() + 1);
  } else {
    stack.at(0) &= argument();
  }
}

void Program::NOR() {
  stackEnsure(1);

  if (argIsNull()) {
    stackEnsure(2);
    stack.at(0) = ~(stack.at(1) | stack.at(0));
    stack.erase(stack.begin() + 1);
  } else {
    stack.at(0) = ~(stack.at(0) | argument());
  }
}

void Program::IFZ() {
  stackEnsure(1);
  if (stack.at(0) != 0) pc++;
}

void Program::IFN() {
  stackEnsure(1);
  if (stack.at(0) == 0) pc++;
}

void Program::IFG() {
  stackEnsure(1);
  if (argIsNull()) {
    stackEnsure(2);
    if (stack.at(1) <= stack.at(0)) pc++;
  } else {
    if (stack.at(0) <= argument()) pc++;
  }
}

void Program::IFL() {
  stackEnsure(1);
  if (argIsNull()) {
    stackEnsure(2);
    if (stack.at(1) >= stack.at(0)) pc++;
  } else {
    if (stack.at(0) >= argument()) pc++;
  }
}

void Program::JMP() {
  pc = getLabelPos(argument()) - 1;
}

void Program::JSR() {
  ret = pc;
  pc = getLabelPos(argument()) - 1;
}

void Program::RET() {
  pc = ret;
}

