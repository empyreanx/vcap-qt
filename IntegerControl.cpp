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

IntegerControl::IntegerControl(vcap_device* vd, vcap_control_info info) : ControlWrapper(vd, info), slider_(Qt::Horizontal) {
    slider_.setMinimum(info.min);
    slider_.setMaximum(info.max);
    slider_.setTickInterval(info.step);

    update();

    connect(&slider_, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
}

void IntegerControl::check() {
    vcap_control_info info;

    if (vcap_get_control_info(vd_, info_.id, &info) == VCAP_ERROR) {
        std::cout << std::string(vcap_get_error(vd_)) << std::endl;
        return;
    }

    bool enabled = slider_.isEnabled();

    if (!info.read_only && !info.write_only && !info.disabled && !info.inactive) {
        if (!enabled)
                update();

        slider_.setDisabled(false);
    } else {
        slider_.setDisabled(true);
    }
}

void IntegerControl::update() {
    int32_t value;

    if (vcap_get_control(vd_, info_.id, &value) == -1)
        std::cout << std::string(vcap_get_error(vd_)) << std::endl;

    slider_.blockSignals(true);
    slider_.setValue(value);
    slider_.blockSignals(false);
}
