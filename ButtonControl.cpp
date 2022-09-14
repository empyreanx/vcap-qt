// SPDX-License-Identifier: MIT

#include "ButtonControl.hpp"

ButtonControl::ButtonControl(vcap_device* vd, vcap_control_info info) : ControlWrapper(vd, info), button_("Execute")
{
    connect(&button_, SIGNAL(clicked(bool)), this, SLOT(push()));
}

void ButtonControl::push()
{
    if (vcap_set_control(vd_, info_.id, 0) != VCAP_OK)
        throw std::runtime_error(vcap_get_error(vd_));
}

void ButtonControl::check()
{
    vcap_control_status status;

    if (vcap_get_control_status(vd_, info_.id, &status) != VCAP_OK)
        throw std::runtime_error(vcap_get_error(vd_));

    if (!status.read_only && !status.write_only && !status.disabled && !status.inactive)
        button_.setDisabled(false);
    else
        button_.setDisabled(true);
}

void ButtonControl::update()
{
}
