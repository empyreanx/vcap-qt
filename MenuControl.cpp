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

MenuControl::MenuControl(vcap_fg* fg, vcap_ctrl_desc desc) : ControlWrapper(fg, desc) {
    vcap_menu_item item;
    vcap_menu_itr* itr = vcap_new_menu_itr(fg, desc.id);

    while (vcap_menu_itr_next(itr, &item)) {
        comboBox_.addItem(reinterpret_cast<char*>(item.name));
    }

    vcap_free(itr);

    connect(&comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
}

void MenuControl::checkStatus() {
    int status = vcap_ctrl_status(fg_, desc_.id);
    bool enabled = comboBox_.isEnabled();

    if (status == VCAP_CTRL_OK) {
        if (!enabled) {
            int32_t index;
            vcap_get_ctrl(fg_, desc_.id, &index);
            comboBox_.setCurrentIndex(index);
        }

        comboBox_.setDisabled(false);
    }

    if (status == VCAP_CTRL_READ_ONLY || status == VCAP_CTRL_INACTIVE)
        comboBox_.setDisabled(true);
}
