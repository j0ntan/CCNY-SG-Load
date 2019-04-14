#ifndef DISPLAY_MESSAGE_H
#define DISPLAY_MESSAGE_H

#include <stdint.h>
#include "Size.h"
#include "Duration.h"

namespace Display {
struct Message {
  char str[Display::ROWS][Display::COLS + 1];
  const Display::Duration duration;
};
}  // namespace Display

#endif  // DISPLAY_MESSAGE_H
