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

#ifndef INTEGER_CONTROL_HPP
#define INTEGER_CONTROL_HPP

#include <QObject>
#include <QSlider>

#include <vcap/vcap.h>

#include "ControlWrapper.hpp"

class IntegerControl : public ControlWrapper {
    Q_OBJECT

public:
    IntegerControl(vcap_fg* fg, vcap_ctrl_desc desc);
    virtual ~IntegerControl() {}

    QWidget* widget() {
        return &slider_;
    }

    void check();
    void update();

private:
    QSlider slider_;
};

#endif // INTEGER_CONTROL_HPP
