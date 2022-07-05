#pragma once

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAdress(DWORD procId, const wchar_t* modName);
