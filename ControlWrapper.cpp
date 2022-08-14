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

#include "ControlWrapper.hpp"

ControlWrapper::ControlWrapper(vcap_dev* vd, vcap_ctrl_info info) : QObject(nullptr), vd_(vd), info_(info) {
}

void ControlWrapper::setValue(int value) {
    if (vcap_set_ctrl(vd_, info_.id, value) == -1) {
        std::cout << std::string(vcap_get_error(vd_)) << std::endl;
    } else {
        emit changed();
    }
}
