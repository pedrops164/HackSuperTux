// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include "pch.h"
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "mem.h"
#include "proc.h"
#include "console.h"

bool Hook(void* toHook, void* ourFunct, int len) {
    if (len < 5) {
        return false;
    }

    DWORD curProtection;
    VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

    memset(toHook, 0x90, len);

    DWORD relativeAdress = ((DWORD)ourFunct - (DWORD)toHook) - 5;

    *(BYTE*)toHook = 0xE9; //0xE9 -> jmp
    *(DWORD*)((DWORD)toHook + 1) = relativeAdress; //passa para a frente do nop e escreve o relative adress

    DWORD temp;
    VirtualProtect(toHook, len, curProtection, &temp);

    return true;
}

DWORD jumpBackAddressSpeed2x;
void __declspec(naked) speed2xInjection() {
    __asm {
        mov dword ptr [esp], 0x3CF5C28F //0.03   || initial value was 0.015
        jmp [jumpBackAddressSpeed2x]
    }
}

DWORD jumpBackAddressJump2x;
void __declspec(naked) jump2xInjection() {
    __asm {
        mov dword ptr [esi+0x000001B4], 0x3F000000 // 0.5 || initial value was 1.0
        jmp [jumpBackAddressJump2x]
    }
}

DWORD WINAPI HackModule(HMODULE hModule) {
    //Create Console
    Console console;
    console.startConsole();

    //get module base
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"supertux2.exe");

    int hookSpeed2xLength = 5;
    DWORD hookAddressSpeed2x = (DWORD)moduleBase + 0xCACAD;
    jumpBackAddressSpeed2x = hookAddressSpeed2x + hookSpeed2xLength;

    int hookJump2xLength = 10;
    DWORD hookAddressJump2x = (DWORD)moduleBase + 0xC6A56;
    jumpBackAddressJump2x = hookAddressJump2x + hookJump2xLength;

    bool bHealth = false, flyingUp = false, flyingDown = false, flyingHacks = false;
    bool speed2x = false, jump2x = false;


    //hack loop
    while (true) {
        //key input
        if (flyingHacks) {
            uintptr_t x_speed_adr = mem::FindDMAAddy((uintptr_t)moduleBase + 0x2AB760, { 0xC, 0x4, 0x1A8 });
            uintptr_t y_speed_adr = mem::FindDMAAddy((uintptr_t)moduleBase + 0x2AB760, { 0xC, 0x4, 0x1AC });

            if ((GetAsyncKeyState(VK_UP) & 0xF0000000) == (GetAsyncKeyState(VK_DOWN) & 0xF0000000)) {
                //Hacks are on, but no key is being pressed. In this case, we want to stay in place instead of falling
                //mem::Nop((BYTE*)y_speed_adr, 4);
                *(float*)y_speed_adr = 0.0;
            }
            else if (GetAsyncKeyState(VK_UP) & 0xF0000000) {
                //flying up
                //0xC3FA0000 = -500
                //mem::Patch((BYTE*)y_speed_adr, (BYTE*)"\xC3\xFA\x00\x00", 4);
                *(float*)y_speed_adr = -500.0;
            }
            else if (GetAsyncKeyState(VK_DOWN) & 0xF0000000) {
                //flying down
                //0x43FA0000 = 500
                //mem::Patch((BYTE*)y_speed_adr, (BYTE*)"\x43\xFA\x00\x00", 4);
                *(float*)y_speed_adr = 500.0;
            }

            if ((GetAsyncKeyState(VK_LEFT) & 0xF0000000) == (GetAsyncKeyState(VK_RIGHT) & 0xF0000000)) {
                //Hacks are on, but no key is being pressed. In this case, we want to stay in place instead of falling
                //mem::Nop((BYTE*)y_speed_adr, 4);
                *(float*)x_speed_adr = 0.0;
            }
            else if (GetAsyncKeyState(VK_LEFT) & 0xF0000000) {
                //flying left
                //0xC3FA0000 = -500
                //mem::Patch((BYTE*)x_speed_adr, (BYTE*)"\xC3\xFA\x00\x00", 4);
                *(float*)x_speed_adr = -500.0;
            }
            else if (GetAsyncKeyState(VK_RIGHT) & 0xF0000000) {
                //flying right
                //0x43FA0000 = 500
                //mem::Patch((BYTE*)x_speed_adr, (BYTE*)"\x43\xFA\x00\x00", 4);
                *(float*)x_speed_adr = 500.0;
            }
        }

        if (GetAsyncKeyState(VK_F7) & 1) {
            //toggle flying hacks
            flyingHacks = !flyingHacks;
            console.toggleFlyingHacks();
        }
        if (GetAsyncKeyState(VK_F8) & 1) {
            //toggle speed 2x hack
            speed2x = !speed2x;
            console.toggleSpeed2xHacks();
            if (speed2x) {
                Hook((void*)hookAddressSpeed2x, speed2xInjection, hookSpeed2xLength);
            }
            else {
                mem::Patch((BYTE*)hookAddressSpeed2x, (BYTE*)"\xF3\x0F\x11\x04\x24", hookSpeed2xLength);
            }
        }
        if (GetAsyncKeyState(VK_F9) & 1) {
            //toggle jump 2x hack
            jump2x = !jump2x;
            console.toggleJump2xHacks();
            if (jump2x) {
                Hook((void*)hookAddressJump2x, jump2xInjection, hookJump2xLength);
            }
            else {
                mem::Patch((BYTE*)hookAddressJump2x, (BYTE*)"\xC7\x86\xB4\x01\x00\x00\x00\x00\x80\x3F", 10);
            }
        }
        if (GetAsyncKeyState(VK_F10) & 1) {
            break;
        }

        Sleep(5);
    }
    console.endConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;

    //cleanup & eject
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackModule, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

