// SPDX-License-Identifier: MIT

#ifndef INTEGER_CONTROL_HPP
#define INTEGER_CONTROL_HPP

#include <QObject>
#include <QSlider>

#include <vcap.h>

#include "ControlWrapper.hpp"

class IntegerControl : public ControlWrapper
{
    Q_OBJECT

public:
    IntegerControl(vcap_device* vd, vcap_control_info info);
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
