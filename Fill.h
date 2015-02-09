#ifndef FILL_H
#define FILL_H

enum class Fill : char {
  EMPTY,
  SOLID,
  OUTER_EDGE,
  INNER_EDGE
};

enum class FillState : char {
  EMPTY,
  FILL_START,
  FILLING,
  FILL_END
};

#endif // FILL_H
