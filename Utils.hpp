/*
    Copyright (c) 2018 James McLean

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <vector>

#include <QImage>
#include <QVector>
#include <QRgb>
#include <QString>

QImage colorToQImage(int width, int height, std::uint8_t* data);
QImage grayscaleToQImage(int width, int height, std::uint8_t* data);

#endif // UTILS_HPP
