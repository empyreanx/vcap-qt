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
