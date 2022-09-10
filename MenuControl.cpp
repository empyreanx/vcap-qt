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

MenuControl::MenuControl(vcap_device* vd, vcap_control_info info) : ControlWrapper(vd, info) {
    vcap_menu_item item;
    vcap_iterator* itr = vcap_menu_iterator(vd, info.id);

    while (vcap_next_menu_item(itr, &item)) {
        comboBox_.addItem(reinterpret_cast<char*>(item.label.str), item.index);
    }

    //TODO: check error

    vcap_free_iterator(itr);

    update();

    connect(&comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
}

void MenuControl::setValue(int index) {
    int value = comboBox_.itemData(index).toInt();

    if (vcap_set_control(vd_, info_.id, value) == VCAP_ERROR)
        throw std::runtime_error(vcap_get_error(vd_));
    else
        emit changed();
}

void MenuControl::check() {
    vcap_control_status status;

    if (vcap_get_control_status(vd_, info_.id, &status) == VCAP_ERROR)
        throw std::runtime_error(vcap_get_error(vd_));

    bool enabled = comboBox_.isEnabled();

    if (!status.read_only && !status.write_only && !status.disabled && !status.inactive) {
        if (!enabled)
                update();

           comboBox_.setDisabled(false);
       } else {
           comboBox_.setDisabled(true);
      }
}

void MenuControl::update() {
    int32_t value;

    if (vcap_get_control(vd_, info_.id, &value) == VCAP_ERROR)
        throw std::runtime_error(vcap_get_error(vd_));

    int index = 0;

    for (int i = 0; i < comboBox_.count(); i++)
    {
        if (comboBox_.itemData(i).toUInt() == value)
        {
            index = i;
            break;
        }
    }

    comboBox_.blockSignals(true);
    comboBox_.setCurrentIndex(index);
    comboBox_.blockSignals(false);
}
