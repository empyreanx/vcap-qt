// SPDX-License-Identifier: MIT

#ifndef BUTTON_CONTROL_HPP
#define BUTTON_CONTROL_HPP

#include <iostream>

#include <QObject>
#include <QPushButton>

#include <vcap.h>

#include "ControlWrapper.hpp"

class ButtonControl : public ControlWrapper
{
    Q_OBJECT

public:
    ButtonControl(vcap_device* vd, vcap_control_info info);
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
