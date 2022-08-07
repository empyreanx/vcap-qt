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

#include "BooleanControl.hpp"

BooleanControl::BooleanControl(vcap_dev* vd, vcap_ctrl_desc desc) : ControlWrapper(vd, desc) {
    update();
    connect(&checkBox_, SIGNAL(clicked(bool)), this, SLOT(setValue(bool)));
}

void BooleanControl::setValue(bool value) {
    if (vcap_set_ctrl(vd_, desc_.id, value ? 1 : 0) == -1) {
        std::cout << std::string(vcap_get_error()) << std::endl;
    } else {
        emit changed();
    }
}

void BooleanControl::check() {
    int status = vcap_ctrl_status(vd_, desc_.id);
    bool enabled = checkBox_.isEnabled();

    if (status == VCAP_CTRL_OK) {
        if (!enabled)
            update();

        checkBox_.setDisabled(false);
    }

    if (status == VCAP_CTRL_READ_ONLY || status == VCAP_CTRL_INACTIVE)
        checkBox_.setDisabled(true);
}

void BooleanControl::update() {
    int32_t value;

    if (vcap_get_ctrl(vd_, desc_.id, &value) == -1)
        std::cout << std::string(vcap_get_error()) << std::endl;

    checkBox_.blockSignals(true);
    checkBox_.setChecked(!!value);
    checkBox_.blockSignals(false);
}
