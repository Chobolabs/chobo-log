//
// chobo-log
// Copyright (c) 2016-2018 Chobolabs Inc.
// http://www.chobolabs.com/
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

// perprocessor concatenation

#define _CHOBO_LOG_CAT(a, b) _CHOBO_LOG_INTERNAL_CAT(a, b)
#define _CHOBO_LOG_INTERNAL_CAT(a, b) a##b
