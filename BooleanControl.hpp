// SPDX-License-Identifier: MIT

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
