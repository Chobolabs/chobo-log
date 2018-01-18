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

#include "chobo/log/Sink.h"
#include <deque>
#include <string>

class TestSink : public chobo::log::Sink
{
public:
    TestSink() = default;
    TestSink(const TestSink&) = delete;
    TestSink& operator=(const TestSink&) = delete;

    virtual void Record(
        chobo::log::ScopeData& scope,
        const std::chrono::time_point<std::chrono::system_clock>& timestamp,
        chobo::log::Severity::Type severity,
        const char* file,
        int line,
        const char* msg) override;

    struct RecordElement
    {
        chobo::log::ScopeData* scope;
        std::chrono::time_point<std::chrono::system_clock> timestamp;
        chobo::log::Severity::Type severity;
        const char* file;
        int line;
        std::string msg;
    };

    RecordElement pop()
    {
        RecordElement e = record.front();
        record.pop_front();
        return e;
    }

    std::deque<RecordElement> record;

    static TestSink& instance();
};