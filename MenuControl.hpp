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

#ifndef MENU_CONTROL_HPP
#define MENU_CONTROL_HPP

#include <QObject>
#include <QComboBox>

#include "ControlWrapper.hpp"
#include "Utils.hpp"

class MenuControl : public ControlWrapper {
    Q_OBJECT

public:
    MenuControl(vcap_fg* fg, vcap_ctrl_desc desc);
    virtual ~MenuControl() {}

    QWidget* widget() {
        return &comboBox_;
    }

    void check();
    void update();

private:
    QComboBox comboBox_;
};

#endif // MENU_CONTROL_HPP
