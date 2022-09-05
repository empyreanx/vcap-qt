#include "Stopwatch.hpp"

#define PICO_TIME_IMPLEMENTATION
#include "pico_time.h"

Stopwatch::Stopwatch() : last(0) {

}

void Stopwatch::reset() {
    last = 0;
}

void Stopwatch::start() {
    last = pt_now();
}

double Stopwatch::stop() {
    if (last == 0)
        return 0;
    else
        return pt_to_sec(pt_now() - last);
}
