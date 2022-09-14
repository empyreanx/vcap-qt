// SPDX-License-Identifier: MIT

#ifndef CONTROL_WRAPPER_HPP
#define CONTROL_WRAPPER_HPP

#include <iostream>
#include <memory>

#include <QObject>
#include <QWidget>

#include <vcap.h>

class ControlWrapper : public QObject
{
    Q_OBJECT

public:
    explicit ControlWrapper(vcap_device* vd, vcap_control_info info);
    virtual ~ControlWrapper() {}

    virtual void check() = 0;
    virtual void update() = 0;

    virtual QWidget* widget() = 0;

signals:
    void changed();

public slots:
    virtual void setValue(int value);

protected:
    vcap_device* vd_;
    vcap_control_info info_;
};

#endif // CONTROLWRAPPER_HPP
