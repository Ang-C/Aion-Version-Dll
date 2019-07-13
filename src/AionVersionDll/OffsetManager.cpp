#include "OffsetManager.h"
#include <iostream>


bool compare(BYTE* data, int * signature, int len)
{
    for (int i = 0; i < len; i++) {
        if (signature[i] != -1 && (int)data[i] != signature[i]) {
            return false;
        }
    }
    return true;
}

DWORD findPattern(DWORD dwAddress, DWORD dwLen, int * signature, int sig_len)
{
    for (DWORD i = 0; i < dwLen; i++)
        if (compare((BYTE*)(dwAddress + i), signature, sig_len))
            return (DWORD)(dwAddress + i);
    return 0;
}


DWORD OffsetManager::addr_UI_SetCursorPos_callReturn = NULL;
DWORD OffsetManager::addr_CAM_SetCursorPos_callReturn = NULL;

int OffsetManager::signature_UI_SetCursorPos_call[SIG_LEN]  = { 0x8b, 0xc6, 0x2b, 0x43, 0x7c, 0x89, 0x43, 0x18, 0x8b, 0xc7 };
int OffsetManager::signature_CAM_SetCursorPos_call[SIG_LEN] = { 0xc6, 0x99, 0x2b, 0xc2, 0xd1, 0xf8, 0x03, 0xc6, 0x50, 0x51 };

int OffsetManager::offset_UI_SetCursorPos_call    = 0xD + 0xE;    // Offset from signature + call length
int OffsetManager::offset_CAM_SetCursorPos_call = 0xA + 0x5;


bool OffsetManager::findOffsets() {

    DWORD dll = (DWORD)GetModuleHandleA((char*)"d3d9.dll");
    if (dll != NULL) {
        addr_UI_SetCursorPos_callReturn = findPattern(dll, 0xFFFFFF, signature_UI_SetCursorPos_call, SIG_LEN)
                                + offset_UI_SetCursorPos_call;
        addr_UI_SetCursorPos_callReturn =
            (addr_UI_SetCursorPos_callReturn == offset_UI_SetCursorPos_call) ?	0 : addr_UI_SetCursorPos_callReturn;
    }

    dll = (DWORD)GetModuleHandleA((char*)"Game.dll");
    if (dll != NULL) {
        addr_CAM_SetCursorPos_callReturn = findPattern(dll, 0xFFFFFF, signature_CAM_SetCursorPos_call, SIG_LEN)
                                + offset_CAM_SetCursorPos_call;
        addr_CAM_SetCursorPos_callReturn = (addr_CAM_SetCursorPos_callReturn == offset_CAM_SetCursorPos_call) ? 0 : addr_CAM_SetCursorPos_callReturn;
    }

    return addr_UI_SetCursorPos_callReturn != NULL && addr_CAM_SetCursorPos_callReturn != NULL;
}

bool OffsetManager::isCamCall(DWORD returnAddr) {

    return addr_UI_SetCursorPos_callReturn != NULL    // Must be found by findOffets()
        && addr_UI_SetCursorPos_callReturn != returnAddr;
}