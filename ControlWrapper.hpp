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

#ifndef CONTROL_WRAPPER_HPP
#define CONTROL_WRAPPER_HPP

#include <iostream>
#include <memory>

#include <QObject>
#include <QWidget>

#include <vcap/vcap.h>

class ControlWrapper : public QObject {
    Q_OBJECT

public:
    explicit ControlWrapper(vcap_dev* vd, vcap_ctrl_desc desc);
    virtual ~ControlWrapper() {}

    virtual void check() = 0;
    virtual void update() = 0;

    virtual QWidget* widget() = 0;

signals:
    void changed();

public slots:
    void setValue(int value);

protected:
    vcap_dev* vd_;
    vcap_ctrl_desc desc_;
};

#endif // CONTROLWRAPPER_HPP
