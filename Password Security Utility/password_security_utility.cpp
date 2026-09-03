#include <iostream>
#include <string>

// ==================== Declarations ====================
void invalidInput();
void exitsystem();
// ---
// ---
void displayMainMenu(int& mainMenuOption);

// ==================== Controllers ====================
int main()
{   
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
    std::cout << "\n  =====================================\n"
              << "        PASSWORD SECURITY UTILITY      \n"
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
void exitsystem()
{
    std::cout << "\nApplication Exited Successfuly.\n";
}

void invalidInput()
{
    std::cout << "\nInvalid Input. Try Again.\n";
}