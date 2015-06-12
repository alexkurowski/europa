#include "state.h"

State::State() {
  std::cout << "State constructor" << std::endl;
}
State::~State() {}

void State::update() {
  std::cout << "State update" << std::endl;
}

void State::draw() {}
