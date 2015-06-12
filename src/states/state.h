#pragma once

#include <iostream>
#include "../../lib/data.h"

class State {
  public:
    State();
    ~State();

    virtual void update();
    virtual void draw();

    virtual void leftClick(Position pos);
    virtual void rightClick(Position pos);
};
