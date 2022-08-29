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
    vcap_control_status status = 0;

    if (vcap_get_control_status(vd_, info_.id, &status) == VCAP_ERROR) {
        std::cout << std::string(vcap_get_error(vd_)) << std::endl;
        return;
    }

    bool enabled = comboBox_.isEnabled();

    if (status == VCAP_CTRL_STATUS_OK) {
        if (!enabled)
            update();

        comboBox_.setDisabled(false);
    }

    if (status == VCAP_CTRL_STATUS_READ_ONLY ||
        status == VCAP_CTRL_STATUS_DISABLED  ||
        status == VCAP_CTRL_STATUS_INACTIVE)
        comboBox_.setDisabled(true);
}

void IntegerMenuControl::update() {
    int32_t value;

    if (vcap_get_control(vd_, info_.id, &value) == -1)
        std::cout << std::string(vcap_get_error(vd_)) << std::endl;

    comboBox_.blockSignals(true);
    comboBox_.setCurrentIndex(value);
    comboBox_.blockSignals(false);
}
