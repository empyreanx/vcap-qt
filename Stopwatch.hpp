// SPDX-License-Identifier: MIT

#ifndef STOPWATCH_H
#define STOPWATCH_H

#include "libs/pico_time.h"

class Stopwatch
{
public:
    Stopwatch();

    void reset();
    void start();
    double stop();
private:
    ptime_t last;
};

#endif // STOPWATCH_H
