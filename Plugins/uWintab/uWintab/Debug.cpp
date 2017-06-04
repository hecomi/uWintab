#pragma once

#include <Windows.h>
#include "Debug.h"


decltype(Debug::isInitialized_)  Debug::isInitialized_ = false;
decltype(Debug::mode_)           Debug::mode_ = Debug::Mode::File;
decltype(Debug::logFunc_)        Debug::logFunc_ = nullptr;
decltype(Debug::errFunc_)        Debug::errFunc_ = nullptr;
decltype(Debug::fs_)             Debug::fs_;
decltype(Debug::ss_)             Debug::ss_;
decltype(Debug::mutex_)          Debug::mutex_;


void Debug::Initialize()
{
    if (isInitialized_) return;
    isInitialized_ = true;

    if (mode_ == Mode::File)
    {
        fs_.open("uWindowCapture.log");
        Debug::Log("Start");
    }
}


void Debug::Finalize()
{
    if (!isInitialized_) return;
    isInitialized_ = false;

    if (mode_ == Mode::File)
    {
        Debug::Log("Stop");
        fs_.close();
    }
}


void OutputApiError(const char* apiName)
{
    const auto error = ::GetLastError();
    Debug::Error(apiName, "() failed with error code: ", error);
}


void OutputApiError(const char* func, const char* apiName)
{
    const auto error = ::GetLastError();
    Debug::Error(func, "() => ", apiName, "() failed with error code: ", error);
}