#ifndef DISPLAY_DURATION_H
#define DISPLAY_DURATION_H

namespace Display {
enum class Duration : unsigned int {
  STATIC = 0,
  SHORTER = 200,
  SHORT = 500,
  NORMAL = 1200,
  LONG = 1800
};
}

#endif  // DISPLAY_DURATION_H
