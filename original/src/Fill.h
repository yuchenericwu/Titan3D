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

Fill operator&&(Fill lhs, Fill rhs);

#endif // FILL_H
