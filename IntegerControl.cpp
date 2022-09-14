// SPDX-License-Identifier: MIT

#include "IntegerControl.hpp"

IntegerControl::IntegerControl(vcap_device* vd, vcap_control_info info) : ControlWrapper(vd, info), slider_(Qt::Horizontal)
{
    slider_.setMinimum(info.min);
    slider_.setMaximum(info.max);
    slider_.setTickInterval(info.step);

    update();

    connect(&slider_, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
}

void IntegerControl::check()
{
    vcap_control_status status;

    if (vcap_get_control_status(vd_, info_.id, &status) != VCAP_OK)
        throw std::runtime_error(vcap_get_error(vd_));

    bool enabled = slider_.isEnabled();

    if (!status.read_only && !status.write_only && !status.disabled && !status.inactive)
    {
        if (!enabled)
                update();

        slider_.setDisabled(false);
    }
    else
    {
        slider_.setDisabled(true);
    }
}

void IntegerControl::update()
{
    int32_t value;

    if (vcap_get_control(vd_, info_.id, &value) != VCAP_OK)
        throw std::runtime_error(vcap_get_error(vd_));

    slider_.blockSignals(true);
    slider_.setValue(value);
    slider_.blockSignals(false);
}
