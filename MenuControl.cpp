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

#include "MenuControl.hpp"

#include <QDebug>
#include "Utils.hpp"

MenuControl::MenuControl(vcap_dev* vd, vcap_ctrl_info info) : ControlWrapper(vd, info) {
    vcap_menu_item item;
    vcap_menu_itr itr = vcap_new_menu_itr(vd, info.id);

    while (vcap_menu_itr_next(&itr, &item)) {
        comboBox_.addItem(reinterpret_cast<char*>(item.name), item.index);
    }

    update();

    connect(&comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
}

void MenuControl::setValue(int index) {
    int value = comboBox_.itemData(index).toInt();

    if (vcap_set_ctrl(vd_, info_.id, value) == -1) {
        std::cout << std::string(vcap_get_error(vd_)) << std::endl;
    } else {
        emit changed();
    }
}

void MenuControl::check() {
    vcap_ctrl_status status = 0;

    if (vcap_get_ctrl_status(vd_, info_.id, &status) == VCAP_ERROR) {
        std::cout << std::string(vcap_get_error(vd_)) << std::endl;
        return;
    }

    bool enabled = comboBox_.isEnabled();

    if (status == VCAP_CTRL_OK) {
        if (!enabled)
            update();

        comboBox_.setDisabled(false);
    } else {
        comboBox_.setDisabled(true);
    }
}

void MenuControl::update() {
    int32_t value;

    if (vcap_get_ctrl(vd_, info_.id, &value) == -1)
        std::cout << std::string(vcap_get_error(vd_)) << std::endl;

    int index = 0;

    for (int i = 0; i < comboBox_.count(); i++) {
        if (comboBox_.itemData(i).toUInt() == value) {
            index = i;
            break;
        }
    }

    comboBox_.blockSignals(true);
    comboBox_.setCurrentIndex(index);
    comboBox_.blockSignals(false);
}
