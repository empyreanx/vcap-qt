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

#ifndef BOOLEAN_CONTROL_HPP
#define BOOLEAN_CONTROL_HPP

#include <iostream>

#include <QObject>
#include <QCheckBox>

#include <vcap.h>

#include "ControlWrapper.hpp"

class BooleanControl : public ControlWrapper
{
    Q_OBJECT

public:
    BooleanControl(vcap_device* vd, vcap_control_info info);
    virtual ~BooleanControl() {}

    QWidget* widget() {
        return &checkBox_;
    }

    void check();
    void update();

private slots:
    void setValue(bool value);

private:
    QCheckBox checkBox_;
};

#endif // BOOLEAN_CONTROL_HPP
