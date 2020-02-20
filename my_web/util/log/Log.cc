//
// Created by kiosk on 2020/2/5.
//
#include "Log.h"

#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>
#include <stdexcept>

class Logger::Impl {
public:
    Impl();

    ~Impl();

    std::ostringstream& info();

    static void init(const std::string& filename, bool append);

    void flush();

    std::ostringstream& getStream();

    void prefixTimestamp();

private:
    static std::string m_timestampSeparator;

    static std::ofstream m_fout;

    static std::mutex m_mutex;

    std::ostringstream m_oss;
};

std::string Logger::Impl::m_timestampSeparator = ": ";

std::ofstream Logger::Impl::m_fout;

// Default streams

std::mutex Logger::Impl::m_mutex;

Logger::Impl::Impl() {
    Logger::Impl::m_mutex.lock();
}

Logger::Impl::~Impl() {
    flush();

    Logger::Impl::m_mutex.unlock();
}

std::ostringstream& Logger::Impl::getStream() {
    Impl::prefixTimestamp();
    m_oss << "Debug<>"
          << " ";
    return m_oss;
}

void Logger::Impl::prefixTimestamp() {
    std::string timeStr;
    time_t rawTime;
    time(&rawTime);
    timeStr = ctime(&rawTime);
    timeStr.erase(timeStr.length() - 1);

    if (!timeStr.empty()) {
        m_oss << timeStr << m_timestampSeparator;
    }
}

void Logger::Impl::flush() {

    if (m_oss.str().empty()) {
        return;
    }
    if (Impl::m_fout.is_open()) {
        Impl::m_fout << m_oss.str() << std::endl;
        Impl::m_fout.flush();
    }
    auto stream = &std::cout;
    *stream << m_oss.str() << std::endl;
    stream->flush();
}

void Logger::Impl::init(const std::string& filename, bool append) {
    if (!filename.empty()) {
        Impl::m_fout.open(filename, append ? std::ofstream::out | std::ofstream::app : std::ofstream::out);
        if (!Impl::m_fout.is_open()) {
            throw std::runtime_error("ERROR!!: Couldn't open '" + filename + "' for write.\n");
        }
    }
}

std::ostringstream& Logger::Impl::info() {
    return getStream();
}

Logger::Logger() : m_impl(new Logger::Impl) {
}

void Logger::init(const std::string& filename, bool append) {
    Impl::init(filename, append);
}

std::ostringstream& Logger::info() {
    return m_impl->info();
}

Logger::~Logger() = default;
