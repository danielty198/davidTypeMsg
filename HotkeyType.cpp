#include <windows.h>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

const string message = "/all Hello everyone, have a fantastic evening!";

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

// Send a key
void sendKey(WORD vk) {
    INPUT input[2] = {};
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = vk;
    input[1].type = INPUT_KEYBOARD;
    input[1].ki.wVk = vk;
    input[1].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(2, input, sizeof(INPUT));
}

// Type a string
void typeString(const string& text, int delayMs = 5) {
    for (char c : text) {
        SHORT vk = VkKeyScanA(c);
        if (vk == -1) continue;
        bool shift = vk & 0x0100;
        if (shift) keybd_event(VK_SHIFT, 0, 0, 0);
        sendKey(vk & 0xFF);
        if (shift) keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}

// Hotkey listener loop
void HotkeyLoop() {
    while (ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
        bool ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
        bool eight = (GetAsyncKeyState(0x38) & 0x8000);

        if (ctrl && eight) {
            while ((GetAsyncKeyState(VK_CONTROL) & 0x8000) || (GetAsyncKeyState(0x38) & 0x8000)) {
                this_thread::sleep_for(chrono::milliseconds(10));
            }

            this_thread::sleep_for(chrono::milliseconds(50));

            sendKey(VK_RETURN);
            this_thread::sleep_for(chrono::milliseconds(100));
            typeString(message, 2);
            sendKey(VK_RETURN);
            this_thread::sleep_for(chrono::milliseconds(200));
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

// Service control handler
void WINAPI ServiceCtrlHandler(DWORD ctrlCode) {
    switch(ctrlCode) {
        case SERVICE_CONTROL_STOP:
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED;
            ServiceStatus.dwCheckPoint    = 0;
            ServiceStatus.dwWaitHint      = 0;
            SetServiceStatus(hStatus, &ServiceStatus);
            break;
        default:
            break;
    }
}

// Service main function
void WINAPI ServiceMain(DWORD argc, LPTSTR *argv) {
    ServiceStatus.dwServiceType = SERVICE_WIN32;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;
    ServiceStatus.dwWaitHint = 0;

    hStatus = RegisterServiceCtrlHandler(TEXT("HotkeyService"), (LPHANDLER_FUNCTION)ServiceCtrlHandler);
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus, &ServiceStatus);

    HotkeyLoop(); // Runs until service is stopped
}

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        {TEXT("HotkeyService"), (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}
    };
    StartServiceCtrlDispatcher(ServiceTable);
    return 0;
}
