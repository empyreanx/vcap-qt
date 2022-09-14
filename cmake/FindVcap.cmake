##==============================================================================
## Vcap - A Video4Linux2 capture library
##
## Copyright (C) 2018 James McLean
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
##==============================================================================

set(VCAP_FOUND 0)

find_path(VCAP_INCLUDE_DIR
    NAMES vcap.h
    DOC "Vcap include directory"
)

find_library(VCAP_LIBRARY
    NAMES vcap
    DOC "Vcap library"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VCAP DEFAULT_MSG VCAP_LIBRARY VCAP_INCLUDE_DIR)

if(NOT VCAP_FOUND)
    message(WARNING "Vcap not found!")
endif()

mark_as_advanced(VCAP_INCLUDE_DIR VCAP_LIBRARY)
