// SPDX-License-Identifier: MIT

#include "Utils.hpp"

QImage colorToQImage(int width, int height, std::uint8_t* data)
{
    QImage qImage(data, width, height, QImage::Format_RGB888);
    return qImage;
}

QImage grayscaleToQImage(int width, int height, std::uint8_t* data)
{
    QImage qImage(data, width, height, QImage::Format_Indexed8);

    QVector<QRgb> colorTable;

    for (int i = 0; i < 256; i++) {
        colorTable.push_back(qRgb(i, i, i));
    }

    qImage.setColorTable(colorTable);

    return qImage;
}
