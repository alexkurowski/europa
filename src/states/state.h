#pragma once

#include <iostream>

class State {
  public:
    State();
    ~State();

    virtual void update();
    virtual void draw();
};
