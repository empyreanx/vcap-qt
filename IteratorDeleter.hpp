// SPDX-License-Identifier: MIT

#ifndef ITERATOR_DELETER_HPP
#define ITERATOR_DELETER_HPP

#include <vcap.h>

class IteratorDeleter
{
public:
    IteratorDeleter();

    void operator()(vcap_iterator* itr);
};

#endif // ITERATOR_DELETER_HPP
