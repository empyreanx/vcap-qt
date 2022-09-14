// SPDX-License-Identifier: MIT

#include "ControlWrapper.hpp"

ControlWrapper::ControlWrapper(vcap_device* vd, vcap_control_info info) : QObject(nullptr), vd_(vd), info_(info)
{
}

void ControlWrapper::setValue(int value)
{
    if (vcap_set_control(vd_, info_.id, value) != VCAP_OK)
        throw std::runtime_error(vcap_get_error(vd_));
    else
        emit changed();
}
