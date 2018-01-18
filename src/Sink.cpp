//
// chobo-log
// Copyright (c) 2016-2018 Chobolabs Inc.
// http://www.chobolabs.com/
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Src.h"
#include <chobo/log/Sink.h>

using namespace chobo::log;

const char* Sink::GetFileOfPath(const char* path)
{
    const char* file = path;
    const char* ptr = path;

    while (*ptr)
    {
        if (*ptr == '/' || *ptr == '\\')
        {
            file = ptr + 1;
        }

        ++ptr;
    }

    return file;
}