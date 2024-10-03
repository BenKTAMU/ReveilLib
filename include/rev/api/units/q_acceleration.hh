/*
 * This code is a modified version of OkapiLib's units library, which in turn is a modified version of
 * Benjamin Jurke's work in 2015. You can read his blog post
 * here:
 * https://benjaminjurke.com/content/articles/2015/compile-time-numerical-unit-dimension-checking/
 *
 * You can find the OkapiLib code this was taken from here:
 * https://github.com/purduesigbots/OkapiLib
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "rev/api/units/q_length.hh"
#include "rev/api/units/q_time.hh"
#include "rev/api/units/r_quantity.hh"

namespace rev {
QUANTITY_TYPE(0, 1, -2, 0, QAcceleration)

constexpr QAcceleration mps2 = meter / (second * second);
constexpr QAcceleration G = 9.80665 * mps2;

inline namespace literals {
constexpr QAcceleration operator"" _mps2(long double x) {
  return QAcceleration(x);
}
constexpr QAcceleration operator"" _mps2(unsigned long long int x) {
  return QAcceleration(static_cast<double>(x));
}
constexpr QAcceleration operator"" _G(long double x) {
  return static_cast<double>(x) * G;
}
constexpr QAcceleration operator"" _G(unsigned long long int x) {
  return static_cast<double>(x) * G;
}
} // namespace literals
} // namespace rev
