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

ButtonControl::ButtonControl(vcap_vd* vd, vcap_ctrl_desc desc) : ControlWrapper(vd, desc), button_("Execute") {
    connect(&button_, SIGNAL(clicked(bool)), this, SLOT(push()));
}

void ButtonControl::push() {
    if (vcap_set_ctrl(vd_, desc_.id, 0) == -1)
        std::cout << std::string(vcap_get_error()) << std::endl;
}

void ButtonControl::check() {
    int status = vcap_ctrl_status(vd_, desc_.id);

    if (status == VCAP_CTRL_OK)
        button_.setDisabled(false);

    if (status == VCAP_CTRL_READ_ONLY || status == VCAP_CTRL_INACTIVE)
        button_.setDisabled(true);
}

void ButtonControl::update() {
}
