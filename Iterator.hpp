// SPDX-License-Identifier: MIT

#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "IteratorDeleter.hpp"

#include <vcap.h>
#include <memory>

using IteratorPtr = std::unique_ptr<vcap_iterator, IteratorDeleter>;

#endif // ITERATOR_HPP
