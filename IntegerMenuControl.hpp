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

#ifndef INTEGER_MENU_CONTROL_HPP
#define INTEGER_MENU_CONTROL_HPP

#include <QObject>
#include <QComboBox>

#include "ControlWrapper.hpp"
#include "Utils.hpp"

class IntegerMenuControl : public ControlWrapper {
    Q_OBJECT

public:
    IntegerMenuControl(vcap_fg* fg, vcap_ctrl_desc desc);
    virtual ~IntegerMenuControl() {}

    QWidget* widget() {
        return &comboBox_;
    }

    virtual void checkStatus();

private:
    QComboBox comboBox_;
};

#endif // INTEGER_MENU_CONTROL_HPP
