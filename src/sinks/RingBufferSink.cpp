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
#include <chobo/log/sinks/RingBufferSink.h>
#include <chobo/log/Scope.h>

using namespace chobo::log;
using namespace std;

RingBufferSink::RingBufferSink(size_t ringSize, size_t msgReserve)
    : m_ringBuffer(ringSize)
{
    if (msgReserve)
    {
        for (auto& elem : m_ringBuffer)
        {
            elem.msg.reserve(msgReserve);
        }
    }
}

void RingBufferSink::Record(
    ScopeData& scope,
    const chrono::time_point<chrono::system_clock>& timestamp,
    Severity::Type severity,
    const char* file,
    int line,
    const char* msg)
{
    auto& elem = m_ringBuffer[m_tail];

    elem.scopeName = scope.name;
    elem.scopeUserData = scope.userData;
    elem.timestamp = timestamp;
    elem.severity = severity;
    elem.file = file;
    elem.line = line;
    elem.msg = msg;

    if (m_tail == m_head)
    {
        ++m_head;
        if (m_head >= m_ringBuffer.size())
            m_head = 0;
    }

    ++m_tail;
    if (m_tail >= m_ringBuffer.size())
    {
        m_tail = 0;
    }
}
