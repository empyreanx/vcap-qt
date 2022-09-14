// SPDX-License-Identifier: MIT

#include "IteratorDeleter.hpp"

IteratorDeleter::IteratorDeleter()
{
}

void IteratorDeleter::operator()(vcap_iterator* itr)
{
    vcap_free_iterator(itr);
}
