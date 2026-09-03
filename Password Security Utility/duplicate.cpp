#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#define COLOR_WARN    "\033[33m"

// ==================== Declarations ====================
void invalidInput();
void exitsystem();
void relaunchInPowerShell(int argc, char* argv[]);
void setupConsoleWindow();
// ---
// ---
void displayMainMenu(int& mainMenuOption);

// ==================== Controllers ====================
int main(int argc, char* argv[])
{   
    relaunchInPowerShell(argc, argv);
    setupConsoleWindow();
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
    std::cout << "\n  =====================================\n"
              << "\033[33m" << "     🛡️ PASSWORD SECURITY UTILITY 🔒  \n" << "\033[0m"
              << "  =====================================\n"
              << "\n      1. Generate Password\n"
              << "      2. Check Password Strength\n"
              << "      3. Check Password Policy\n"
              << "      4. Compare Passwords\n"
              << "      5. Security Recommendations\n"
              << "      6. Security Challenge\n"
              << "      7. Exit\n"
              << "\n    Select option: ";
    std::cin >> mainMenuOption;
}

// ==================== Core Logics ====================

// ==================== Utilities ====================
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
        std::string command = "conhost.exe powershell.exe -NoExit -Command \"& '" + exePath + "' --in-ps\"";
        std::system(command.c_str());
        std::exit(0);
    }
}

void exitsystem()
{
    std::cout << "\nApplication Exited Successfuly.\n";
}

void invalidInput()
{
    std::cout << "\nInvalid Input. Try Again.\n";
}