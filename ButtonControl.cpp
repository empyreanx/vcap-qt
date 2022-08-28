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

ButtonControl::ButtonControl(vcap_device* vd, vcap_control_info info) : ControlWrapper(vd, info), button_("Execute") {
    connect(&button_, SIGNAL(clicked(bool)), this, SLOT(push()));
}

void ButtonControl::push() {
    if (vcap_set_control(vd_, info_.id, 0) == -1)
        std::cout << std::string(vcap_get_error(vd_)) << std::endl;
}

void ButtonControl::check() {
    vcap_control_status status = 0;

    if (vcap_get_control_status(vd_, info_.id, &status) == VCAP_ERROR) {
        std::cout << std::string(vcap_get_error(vd_)) << std::endl;
        return;
    }

    if (status == VCAP_CTRL_STATUS_OK)
        button_.setDisabled(false);
    else
        button_.setDisabled(true);
}

void ButtonControl::update() {
}
