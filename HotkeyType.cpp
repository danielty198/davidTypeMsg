#include <windows.h>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

const string message = "/all Hello everyone, I hope y’all are having an absolutely fantastic and wonderful evening. Good luck have fun, love y’all so much. Stay safe please. Okay? UWU ❤️";

// Send a key with optional shift
void sendKey(WORD vk, bool shift = false) {
    INPUT inputs[4] = {};
    int count = 0;

    if (shift) {
        inputs[count].type = INPUT_KEYBOARD;
        inputs[count].ki.wVk = VK_SHIFT;
        count++;
    }

    inputs[count].type = INPUT_KEYBOARD;
    inputs[count].ki.wVk = vk;
    count++;

    inputs[count].type = INPUT_KEYBOARD;
    inputs[count].ki.wVk = vk;
    inputs[count].ki.dwFlags = KEYEVENTF_KEYUP;
    count++;

    if (shift) {
        inputs[count].type = INPUT_KEYBOARD;
        inputs[count].ki.wVk = VK_SHIFT;
        inputs[count].ki.dwFlags = KEYEVENTF_KEYUP;
        count++;
    }

    SendInput(count, inputs, sizeof(INPUT));
}

// Type a string
void typeString(const string& text, int delayMs = 5) {
    for (char c : text) {
        SHORT vk = VkKeyScanA(c);
        if (vk == -1) continue;

        bool shift = (vk & 0x0100) != 0;
        sendKey(vk & 0xFF, shift);
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    while (true) {
        bool ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
        bool eight = (GetAsyncKeyState(0x38) & 0x8000) != 0;

        if (ctrl && eight) {
            // Wait for keys to be released
            while ((GetAsyncKeyState(VK_CONTROL) & 0x8000) || (GetAsyncKeyState(0x38) & 0x8000)) {
                this_thread::sleep_for(chrono::milliseconds(10));
            }

            this_thread::sleep_for(chrono::milliseconds(50));
            typeString(message, 5); // small delay between letters
            sendKey(VK_RETURN);
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }

    return 0;
}
