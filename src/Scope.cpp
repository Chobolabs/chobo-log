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
#include <chobo/log/Scope.h>
#include <chobo/log/Sink.h>
#include <chobo/log/LogManager.h>

#include <iostream>
#include <cassert>

using namespace std;

namespace chobo {
namespace log
{

ScopeData::ScopeData(const char* name, int id)
    : name(name)
    , id(id)
{
}

namespace
{
class vecstreambuf : public streambuf
{
public:
    vecstreambuf(size_t reserve)
    {
        m_data.resize(reserve);
        setp(m_data.data(), m_data.data() + reserve);
    }

    virtual int_type overflow(int_type ch) override
    {
        auto oldSize = m_data.size();
        auto newSize = oldSize * 2;
        m_data.resize(newSize);
        setp(m_data.data() + oldSize, m_data.data() + newSize);
        *pptr() = char(ch);
        pbump(1);

        return ch;
    }

    streampos seekpos(streampos sp, ios_base::openmode which) override
    {
        setp(m_data.data() + int(sp), m_data.data() + m_data.size());
        return sp;
    }

    const char* str() const
    {
        return m_data.data();
    }

private:
    vector<char> m_data;
};
}

std::string BasicScope::m_globalTag = "Chobo";

BasicScope::BasicScope(const char* name, int id, size_t initialBufSize)
    : ScopeData(name, id)
    , stream(new vecstreambuf(initialBufSize))
{
}

BasicScope::~BasicScope()
{
    auto buf = stream.rdbuf(nullptr);
    delete buf;
}

void BasicScope::AddSink(Sink& r)
{
    m_sinks.push_back(&r);
}

void BasicScope::Begin(Severity::Type severity, const char* file, int line)
{
    m_severity = severity;
    m_file = file;
    m_line = line;
}

void BasicScope::End()
{
    assert(IsEnabled());
    assert(m_severity < Severity::Count); // end with no begin

    // adding an ends, because we keep resetting the stream position to save allocations
    // if we don't add it, putting a shorter string after a longer string will contain the end of the string too
    stream << ends;

    auto now = std::chrono::system_clock::now();

    auto buf = static_cast<vecstreambuf*>(stream.rdbuf());
    auto msg = buf->str();

    for (auto sink : m_sinks)
    {
        sink->Record(*this, now, m_severity, m_file, m_line, msg);
    }

    stream.seekp(0);

    m_severity = Severity::Count;
    m_file = nullptr;
    m_line = 0;
}

LocalScope::LocalScope(const char* name, int id, size_t initialBufSize)
    : BasicScope(name, id, initialBufSize)
{
    LogManager::GetInstance().RegisterScope(*this);
}

LocalScope::~LocalScope()
{
    LogManager::GetInstance().UnregisterScope(*this);
}

GlobalScope::GlobalScope(const char* name, int id, size_t initialBufSize)
    : SyncScope(name, id, initialBufSize)
{
    LogManager::GetInstance().RegisterScope(*this);
}

GlobalScope::~GlobalScope()
{
    LogManager::GetInstance().UnregisterScope(*this);
}

DefaultScope::DefaultScope()
    : GlobalScope("Default", 0)
{}

DefaultScope& DefaultScope::Get()
{
    return m_instance;
}

DefaultScope& DefaultScope::GetInstance()
{
    static DefaultScope ds;
    return ds;
}

DefaultScope& DefaultScope::m_instance = DefaultScope::GetInstance();

}
}
