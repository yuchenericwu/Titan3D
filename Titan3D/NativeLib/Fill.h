#pragma once

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
