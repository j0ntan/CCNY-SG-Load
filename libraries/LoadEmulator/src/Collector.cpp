#include "Collector.h"

bool Collector::isCollecting() const {
    return _collectingStatus;
}

const String Collector::getInput(void) const {
    return _input;
}
const String Collector::getColMsg(void) const {
    return _collectorMsg;
}
const Display::Speed Collector::getSpeed(void) const {
    return _speed;
}
