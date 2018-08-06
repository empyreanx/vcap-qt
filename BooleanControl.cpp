/*
    Copyright (c) 2015 James McLean

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include "BooleanControl.hpp"

BooleanControl::BooleanControl(vcap_fg* fg, vcap_ctrl_desc desc) : ControlWrapper(fg, desc) {
    int32_t value;

    if (vcap_get_ctrl(fg, desc.id, &value) == -1)
        std::cout << std::string(vcap_get_error()) << std::endl;

    checkBox_.setChecked(!!value);

    connect(&checkBox_, SIGNAL(clicked(bool)), this, SLOT(setValue(bool)));
}

void BooleanControl::setValue(bool value) {
    if (vcap_set_ctrl(fg_, desc_.id, value ? 1 : 0) == -1) {
        std::cout << std::string(vcap_get_error()) << std::endl;
    } else {
        emit changed();
    }
}

void BooleanControl::checkStatus() {
    int status = vcap_ctrl_status(fg_, desc_.id);
    bool enabled = checkBox_.isEnabled();

    if (status == VCAP_CTRL_OK) {
        if (!enabled) {
            int32_t checked;
            vcap_get_ctrl(fg_, desc_.id, &checked);
            checkBox_.setChecked(!!checked);
        }

        checkBox_.setDisabled(false);
    }

    if (status == VCAP_CTRL_READ_ONLY || status == VCAP_CTRL_INACTIVE)
        checkBox_.setDisabled(true);
}
