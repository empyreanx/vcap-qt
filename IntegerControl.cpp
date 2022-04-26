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

#include "IntegerControl.hpp"

IntegerControl::IntegerControl(vcap_vd* vd, vcap_ctrl_desc desc) : ControlWrapper(vd, desc), slider_(Qt::Horizontal) {
    slider_.setMinimum(desc.min);
    slider_.setMaximum(desc.max);
    slider_.setTickInterval(desc.step);

    update();

    connect(&slider_, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
}

void IntegerControl::check() {
    int status = vcap_ctrl_status(vd_, desc_.id);
    bool enabled = slider_.isEnabled();

    if (status == VCAP_CTRL_OK) {
        if (!enabled)
            update();

        slider_.setDisabled(false);
    }

    if (status == VCAP_CTRL_READ_ONLY || status == VCAP_CTRL_INACTIVE)
        slider_.setDisabled(true);
}

void IntegerControl::update() {
    int32_t value;

    if (vcap_get_ctrl(vd_, desc_.id, &value) == -1)
        std::cout << std::string(vcap_get_error()) << std::endl;

    slider_.blockSignals(true);
    slider_.setValue(value);
    slider_.blockSignals(false);
}
