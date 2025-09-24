#include <windows.h>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

// Your message
const string message = "/all Hello everyone, I hope y’all are having an absolutely fantastic and wonderful evening. Good luck have fun, love y’all so much. Stay safe please. Okay? UWU ❤️";

// Helper: send a single key (virtual key code)
void sendKey(WORD vk) {
    INPUT input[2] = {};

    // Key down
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = vk;

    // Key up
    input[1].type = INPUT_KEYBOARD;
    input[1].ki.wVk = vk;
    input[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(2, input, sizeof(INPUT));
}

// Helper: type a string (ASCII only)
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

// WinMain entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    while (true) {
        bool ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
        bool eight = (GetAsyncKeyState(0x38) & 0x8000);

        if (ctrl && eight) {
            while ((GetAsyncKeyState(VK_CONTROL) & 0x8000) || (GetAsyncKeyState(0x38) & 0x8000)) {
                this_thread::sleep_for(chrono::milliseconds(10));
            }

            this_thread::sleep_for(chrono::milliseconds(50));
            typeString(message, 2);
            sendKey(VK_RETURN);
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }

    return 0;
}
