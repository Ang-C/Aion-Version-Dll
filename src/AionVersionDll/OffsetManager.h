#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define SIG_LEN		10

class OffsetManager
{
public:
    // The offset of the next instruction after the functions' calls
    static DWORD addr_UI_SetCursorPos_callReturn;
    static DWORD addr_CAM_SetCursorPos_callReturn;

    static bool findOffsets();

    // The game client has two functions calling SetCursorPos():
    // From Game.dll: only for camera movements. This is when mousefix is needed
    // From d3d9: in all other mouse uses (UI buttons, items drag&drop, etc)
    static bool isCamCall(DWORD returnAddr);

private:
    // 8b c6 2b 43 7c 89 43 18 8b c7
    static int signature_UI_SetCursorPos_call[SIG_LEN];
    // c6 99 2b c2 d1 f8 03 c6 50 51
    static int signature_CAM_SetCursorPos_call[SIG_LEN];

    static int offset_UI_SetCursorPos_call;
    static int offset_CAM_SetCursorPos_call;
};
