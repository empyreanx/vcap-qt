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

#include "IntegerMenuControl.hpp"

#include <QDebug>
#include "Utils.hpp"

IntegerMenuControl::IntegerMenuControl(vcap_device *vd, vcap_control_info info) : ControlWrapper(vd, info) {
    vcap_menu_item item;
    vcap_iterator* itr = vcap_menu_iterator(vd, info.id);

    while (vcap_next_menu_item(itr, &item)) {
        comboBox_.addItem(QString::number(item.label.num));
    }

    vcap_free_iterator(itr);

    update();

    connect(&comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
}

void IntegerMenuControl::check() {
    vcap_control_info info;

    if (vcap_get_control_info(vd_, info_.id, &info) == VCAP_ERROR) {
        std::cout << std::string(vcap_get_error(vd_)) << std::endl;
        return;
    }

    bool enabled = comboBox_.isEnabled();

    if (!info.read_only && !info.write_only && !info.disabled && !info.inactive) {
        if (!enabled)
            update();

        comboBox_.setDisabled(false);
    } else {
        comboBox_.setDisabled(true);
    }
}

void IntegerMenuControl::update() {
    int32_t value;

    if (vcap_get_control(vd_, info_.id, &value) == -1)
        std::cout << std::string(vcap_get_error(vd_)) << std::endl;

    comboBox_.blockSignals(true);
    comboBox_.setCurrentIndex(value);
    comboBox_.blockSignals(false);
}
