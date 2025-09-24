#include <windows.h>
#include <string>
#include <iostream>
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

// Helper: type a string (ASCII only; Unicode emojis will require more work)
void typeString(const string& text, int delayMs = 5) {
    for (char c : text) {
        SHORT vk = VkKeyScanA(c); // map char to virtual key
        if (vk == -1) continue;   // skip unsupported chars

        // Check if SHIFT is required
        bool shift = vk & 0x0100;
        if (shift) keybd_event(VK_SHIFT, 0, 0, 0);

        sendKey(vk & 0xFF);

        if (shift) keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);

        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

int main() {
    cout << "Listening for Ctrl + 8..." << endl;

    while (true) {
        // Check if Ctrl is pressed
        bool ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000);

        // Check if '8' on main keyboard is pressed
        bool eight = (GetAsyncKeyState(0x38) & 0x8000);

        if (ctrl && eight) {
            cout << "Hotkey detected! Typing message..." << endl;

            // Step 1: open chat
            sendKey(VK_RETURN);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Step 2: type message
            typeString(message, 2); // 2ms delay between chars

            // Step 3: send message
            sendKey(VK_RETURN);

            cout << "Message sent!" << endl;

            // Wait until keys are released to avoid repeating
            while ((GetAsyncKeyState(VK_CONTROL) & 0x8000) || (GetAsyncKeyState(0x38) & 0x8000)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
