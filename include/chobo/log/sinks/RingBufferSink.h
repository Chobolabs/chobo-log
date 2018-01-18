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

#include "../Config.h"
#include "../Sink.h"

#include <string>
#include <vector>

namespace chobo
{
namespace log
{

class CHOBO_LOG_API RingBufferSink : public Sink
{
public:
    RingBufferSink(size_t ringSize, size_t msgReserve = 0);

    virtual void Record(
        ScopeData& scope,
        const std::chrono::time_point<std::chrono::system_clock>& timestamp,
        Severity::Type severity,
        const char* file,
        int line,
        const char* msg) override;

    struct RecordElement
    {
        const char* scopeName;
        intptr_t scopeUserData;
        std::chrono::time_point<std::chrono::system_clock> timestamp;
        Severity::Type severity;
        const char* file;
        int line;
        std::string msg;
    };

    size_t GetRingSize() const { return m_ringBuffer.size(); }

    // last element is newest
    const RecordElement& GetElement(size_t index) const { return m_ringBuffer[(m_head + index) % GetRingSize()]; }

private:
    
    size_t m_head = 0, m_tail = 0;
    std::vector<RecordElement> m_ringBuffer;
};

}
}
