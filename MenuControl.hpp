// SPDX-License-Identifier: MIT

#ifndef MENU_CONTROL_HPP
#define MENU_CONTROL_HPP

#include <QObject>
#include <QComboBox>

#include "ControlWrapper.hpp"
#include "MainWindow.hpp"
#include "Utils.hpp"

class MainWindow;

class MenuControl : public ControlWrapper
{
    Q_OBJECT

public:
    MenuControl(vcap_device* vd, vcap_control_info info);
    virtual ~MenuControl() {}

    QWidget* widget() {
        return &comboBox_;
    }

    void check();
    void update();

public slots:
    void setValue(int value);

private:
    QComboBox comboBox_;
};

#endif // MENU_CONTROL_HPP
