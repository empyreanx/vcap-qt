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

#ifndef BUTTON_CONTROL_HPP
#define BUTTON_CONTROL_HPP

#include <iostream>

#include <QObject>
#include <QPushButton>

#include <vcap/vcap.h>

#include "ControlWrapper.hpp"

class ButtonControl : public ControlWrapper {
    Q_OBJECT

public:
    ButtonControl(vcap_vd* vd, vcap_ctrl_desc desc);
    virtual ~ButtonControl() {}

    QWidget* widget() {
        return &button_;
    }

    void check();
    void update();

private slots:
    void push();

private:
    QPushButton button_;
};

#endif // BUTTON_CONTROL_HPP
