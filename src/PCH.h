#pragma once

#define UNICODE
#define _UNICODE
#define WIN32_MEAN_AND_LEAN

#define TOML_EXCEPTIONS 0

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

namespace logger = SKSE::log;
using namespace std::literals;

namespace stl
{
template <class T>
void write_thunk_call()
{
    auto&                           trampoline = SKSE::GetTrampoline();
    REL::Relocation<std::uintptr_t> hook{T::id, T::offset};
    T::func = trampoline.write_call<5>(hook.address(), T::thunk);
}
} // namespace stl

using EventResult = RE::BSEventNotifyControl;

template <typename T>
using StrMap = std::map<std::string, T, std::less<>>;
using StrSet = std::set<std::string, std::less<>>;
