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

IntegerMenuControl::IntegerMenuControl(vcap_vd* vd, vcap_ctrl_desc desc) : ControlWrapper(vd, desc) {
    vcap_menu_item item;
    vcap_menu_itr* itr = vcap_new_menu_itr(vd, desc.id);

    while (vcap_menu_itr_next(itr, &item)) {
        comboBox_.addItem(QString::number(item.value));
    }

    vcap_free(itr);

    update();

    connect(&comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
}

void IntegerMenuControl::check() {
    int status = vcap_ctrl_status(vd_, desc_.id);
    bool enabled = comboBox_.isEnabled();

    if (status == VCAP_CTRL_OK) {
        if (!enabled)
            update();

        comboBox_.setDisabled(false);
    }

    if (status == VCAP_CTRL_READ_ONLY || status == VCAP_CTRL_INACTIVE)
        comboBox_.setDisabled(true);
}

void IntegerMenuControl::update() {
    int32_t value;

    if (vcap_get_ctrl(vd_, desc_.id, &value) == -1)
        std::cout << std::string(vcap_get_error()) << std::endl;

    comboBox_.blockSignals(true);
    comboBox_.setCurrentIndex(value);
    comboBox_.blockSignals(false);
}
