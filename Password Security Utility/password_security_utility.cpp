#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <mmsystem.h>
#include <thread>
#include <chrono>
#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")
#endif

// ==================== Declarations ====================
void invalidInput();
void exitsystem();
void relaunchInPowerShell(int argc, char* argv[]);
void setupConsoleWindow();
void showStaticLoading(const std::string& message = "Processing", int dots = 6, int delayMs = 600);
void getValidatedInput(int& input, int minimumValue, int maximumValue);
void playSoundEffect(const std::string& soundFile);
// ---
// ---
void displayMainMenu(int& mainMenuOption);

// ==================== Controllers ====================
int main(int argc, char* argv[])
{   
    relaunchInPowerShell(argc, argv);
    setupConsoleWindow();
    playSoundEffect("assets/welcome.wav");
    int mainMenuOption{};
    bool runLoop = true;
    do
    {
        displayMainMenu(mainMenuOption);
        if (mainMenuOption == 1)
        {

        }
        else if (mainMenuOption == 2)
        {

        }
        else if (mainMenuOption == 3)
        {

        }
        else if (mainMenuOption == 4)
        {

        }
        else if (mainMenuOption == 5)
        {

        }
        else if (mainMenuOption == 6)
        {

        }
        else if (mainMenuOption == 7)
        {
            showStaticLoading("Exiting System");
            exitsystem();
            return 0;
        }
        else
        {
            invalidInput();
        }
    } while (runLoop);
    return 0;
}

void displayMainMenu(int& mainMenuOption)
{
    system("cls");
    std::cout << "\n\033[34m  =====================================\033[0m\n"
              << "\033[33m" << "     🛡️ PASSWORD SECURITY UTILITY 🔒  \n" << "\033[0m"
              << "\033[34m  =====================================\033[0m\n"
              << "\n      1. Generate Password\n"
              << "      2. Check Password Strength\n"
              << "      3. Check Password Policy\n"
              << "      4. Compare Passwords\n"
              << "      5. Security Recommendations\n"
              << "      6. Security Challenge\n"
              << "      7. Exit\n"
              << "\n\033[32m    Select option: \033[0m";
    getValidatedInput(mainMenuOption, 1, 7);
}

// ==================== Core Logics ====================

// ==================== Utilities ====================
void playSoundEffect(const std::string& soundFile)
{
    // SND_FILENAME: path points to a file | SND_ASYNC: plays in background
    PlaySoundA(soundFile.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

void getValidatedInput(int& input, int minimumValue, int maximumValue)
{
    if (std::cin >> input)
    {
        if (input < minimumValue || input > maximumValue)
        {
            std::cin.clear();
            input = -1;
        }
    }
    else
    {
        std::cin.clear();
        input = -1;
    }
    std::cin.ignore(10000, '\n');
}

void showStaticLoading(const std::string& message, int dots, int delayMs)
{
    const std::string listMargin = "    ";
    std::cout << "\n" << listMargin << "⌛ \033[36m" << message << "\033[0m";
    std::cout.flush();
    for (int i = 0; i < dots; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        std::cout << "\033[36m.\033[0m" << std::flush;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "\n";
}

void setupConsoleWindow()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    SetConsoleTitleA("Password Security v1.0");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
        cfi.dwFontSize.Y = 28;
        cfi.dwFontSize.X = 0;
        wcscpy_s(cfi.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
        SMALL_RECT tinyRect = { 0, 0, 1, 1 };
        SetConsoleWindowInfo(hOut, TRUE, &tinyRect);
        COORD bufferSize = { 60, 20 };
        SetConsoleScreenBufferSize(hOut, bufferSize);
        SMALL_RECT windowSize = { 0, 0, 59, 19 };
        SetConsoleWindowInfo(hOut, TRUE, &windowSize);
    }
    system("cls");
}

void relaunchInPowerShell(int argc, char* argv[])
{
    if (argc == 1 || std::string(argv[argc - 1]) != "--in-ps") 
    {
        HWND hwnd = GetConsoleWindow();
        if (hwnd != NULL) ShowWindow(hwnd, SW_HIDE);
        std::string exePath = argv[0];
        std::string command = "conhost.exe powershell.exe -Command \"& '" + exePath + "' --in-ps\"";
        std::system(command.c_str());
        std::exit(0);
    }
}

void exitsystem()
{
    system("cls");
    playSoundEffect("assets/thank_you.wav");
    std::cout << "\n\033[32m    Application Exited Successfully. Goodbye!\033[0m \033[31m💖\033[0m\n";
    std::cout << " \n";
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
}

void invalidInput()
{
    MessageBeep(MB_ICONWARNING);
    std::cout << "\n\033[31m    Invalid Input. Try Again.\033[0m\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}