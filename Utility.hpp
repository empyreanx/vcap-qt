// SPDX-License-Identifier: MIT

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
