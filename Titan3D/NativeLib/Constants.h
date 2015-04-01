#pragma once

namespace Constants {

  const double PRECISION = 25.4 / 600;
  const double X_SCALING = 1.16114237057;
  const size_t MAX_SIZE_X = 50.8 / PRECISION * X_SCALING;
  const size_t MAX_SIZE_Y = 50.8 / PRECISION;
  const size_t MAX_SIZE_Z = 44.8 / PRECISION;

  enum {
    PARSE,
    SHELL,
    FILL,
  };

} // namespace Constants