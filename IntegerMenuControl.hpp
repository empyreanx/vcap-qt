// SPDX-License-Identifier: MIT

#ifndef INTEGER_MENU_CONTROL_HPP
#define INTEGER_MENU_CONTROL_HPP

#include <QObject>
#include <QComboBox>

#include "ControlWrapper.hpp"

class IntegerMenuControl : public ControlWrapper
{
    Q_OBJECT

public:
    IntegerMenuControl(vcap_device* vd, vcap_control_info info);
    virtual ~IntegerMenuControl() {}

    QWidget* widget() {
        return &comboBox_;
    }

    void check();
    void update();

private:
    QComboBox comboBox_;
};

#endif // INTEGER_MENU_CONTROL_HPP
