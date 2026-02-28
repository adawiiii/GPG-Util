#include<iostream>
#include<Windows.h>
#include<chrono>
#include<future>
#include <thread>

enum CommandType {
    KILL,
    LAUNCH
};

DWORD run_command(std::string command) {
    STARTUPINFOA si = {sizeof(si)};
    PROCESS_INFORMATION pi;

    if(CreateProcessA(
        nullptr,
        &command[0],
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        nullptr,
        &si,
        &pi
    )) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exit_code;
        GetExitCodeProcess(pi.hProcess, &exit_code);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return exit_code;
    } else {
        return 1;
    }
}

std::future<DWORD> async_powershell_command(std::string command) {
    return std::async(std::launch::async, run_command, command);
}

void wait_loop(CommandType type, int attempt, std::future<DWORD>& promise) {
    switch (type) {
        case KILL: {
            while (promise.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
                std::cout << "Killing GPG (" << attempt << ").  \r";
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                std::cout << "Killing GPG (" << attempt << ").. \r";
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                std::cout << "Killing GPG (" << attempt << ")...\r";
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
            DWORD k_ret = promise.get();
            if (k_ret == 0) {
                std::cout << "Killed GPG (" << attempt << ")    " << std::endl;
            } else {
                std::cout << "Error killing GPG (" << attempt << "): " << k_ret << std::endl;
            }
            break;
        }
        case LAUNCH: {
            while (promise.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
                std::cout << "Launching GPG (" << attempt << ").  \r";
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                std::cout << "Launching GPG (" << attempt << ").. \r";
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                std::cout << "Launching GPG (" << attempt << ")...\r";
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
            DWORD l_ret = promise.get();
            if (l_ret == 0) {
                std::cout << "Launched GPG (" << attempt << ")    " << std::endl;
            } else {
                std::cout << "Error launching GPG (" << attempt << "): " << l_ret << std::endl;
            }
            break;
        }
        default:
            break;
    }
    return;
}

int main() {
    std::string kill_command = "powershell -NoProfile -Command \"gpgconf --kill gpg-agent\"";
    std::string launch_command = "powershell -NoProfile -Command \"gpgconf --launch gpg-agent\"";

    std::future<DWORD> kill = async_powershell_command(kill_command);
    wait_loop(KILL, 1, kill);
    kill = async_powershell_command(kill_command);
    wait_loop(KILL, 2, kill);
    kill = async_powershell_command(kill_command);
    wait_loop(KILL, 3, kill);

    std::future<DWORD> launch = async_powershell_command(launch_command);    
    wait_loop(LAUNCH, 1, launch);
    launch = async_powershell_command(launch_command);    
    wait_loop(LAUNCH, 2, launch);

    std::cout << "GPG Cleared" << std::endl;
    return 0;
}