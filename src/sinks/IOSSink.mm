//
// chobo-log
// Copyright (c) 2016-2018 Chobolabs Inc.
// http://www.chobolabs.com/
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//

#include "../Src.h"
#include <chobo/log/sinks/IOSSink.h>
#include <chobo/log/LogManager.h>

#import <cstdio>
#import <Foundation/Foundation.h>

using namespace chobo::log;
using namespace std;

void IOSSink::Record(
    ScopeData& scope,
    const chrono::time_point<chrono::system_clock>& timestamp,
    Severity::Type severity,
    const char* file,
    int line,
    const char* msg)
{
    const char* sev = Severity::ToString(severity);
    NSLog(@"%@: %@", [NSString stringWithUTF8String:sev], [NSString stringWithUTF8String:msg]);
}
