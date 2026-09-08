#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <ctime>
#include <windows.h>
#include <mmsystem.h>
#include <aclapi.h>
#include <sddl.h>
#include <bcrypt.h>
#include <cctype>

namespace fs = std::filesystem;

#ifdef _MSC_VER
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "winmm.lib")
#endif

const std::string VAULT_DIR = "data";
const std::string VAULT_FILE = "data/vault.dat";

// ==================== Declarations ====================
// ------ Utilities ------
void invalidInput();
void exitsystem();
void relaunchInPowerShell(int argc, char* argv[]);
void setConsoleFontSizeAndSize(short fontSize, short widthInCols, short heightInRows);
void setupConsoleWindow();
void showStaticLoading(const std::string& message = "Processing", int dots = 6, int delayMs = 600);
void getValidatedInput(int& input, int minimumValue, int maximumValue);
void playSoundEffect(const std::string& soundFile);
void pauseScreen(const std::string& message = "Press Enter to return to the Main Menu");
std::string stringToHex(const std::string& input);
std::string xorCipher(const std::string& text, const std::string& key);
std::string hexToString(const std::string& input);
std::string generateRandomKey(size_t length = 16);
bool secureRandomBytes(void* buffer, size_t length);
bool restrictFileToCurrentUser(const std::string& filePath);
bool saveToLocalVault(const std::string& serviceName, const std::string& encryptedPayload);
void viewLocalVault();
bool deleteVaultEntry();
void secureWipeString(std::string& str);
// ------ Main Menu ------
void showMainMenuUI(int& mainMenuOption);
bool handleMainMenu(int& mainMenuOption, int& passLength, std::string& passToCheck, std::string& passCheckPolicy, int& selectPassCrypt);
// ------ Generate Password ------
void showGenPassUI(int& passLength);
void runGenPassFeat(int& passLength);
std::string generatePassword(int passLength);
bool genPassValidation(const std::string& password, int passLength);
// ------ Check Password Strength ------
void showCheckPassStrengthUI(std::string& passToCheck);
bool runCheckPassStrength(std::string& passToCheck);
void showCheckPassResult(const std::string& strengthClass, bool hasMinLength, 
                         bool hasUpper, bool hasLower, bool hasDigit, bool hasSpecial);
bool checkPasswordValidation(const std::string& passToCheck);
void checkPassEvaluation(const std::string& passToCheck, bool& hasMinLength, 
                         bool& hasUpper, bool& hasLower, bool& hasDigit, bool& hasSpecial);
void getStrengthClassification(std::string& strengthClass, bool hasMinLength, bool hasUpper, bool hasLower, bool hasDigit, bool hasSpecial);
// ------ Check Password Policy ------
void showCheckPassPolicyUI(std::string& passCheckPolicy);
void showPassPolicyResultUI(const std::string& PolicyClass, bool hasMinLength, 
                         bool hasUpper, bool hasLower, bool hasDigit, bool hasSpecial);
bool runCheckPassPolicy(std::string& passCheckPolicy);
bool checkPassPolicyValidation(const std::string& passCheckPolicy);
void PassPolicyEvaluation(const std::string& passCheckPolicy, bool& hasMinLength, 
                         bool& hasUpper, bool& hasLower, bool& hasDigit, bool& hasSpecial);
void getPolicyClassification(std::string& PolicyClass, bool hasMinLength, bool hasUpper, bool hasLower, bool hasDigit, bool hasSpecial);
// ------ Password Cryptography ------
void showPassCryptUI(int& selectPassCrypt);
void showPassEncryptUI(std::string& serviceName, std::string& passToEncrypt);
void showPassDecryptUI(std::string& passToDecrypt, std::string& keyToDecrypt);
void showPassEncryptResultUI(const std::string& serviceName, const std::string& generatedKey, const std::string& encryptedPass);
void showPassDecryptResultUI(std::string& decryptedPass);
bool runPassEncrypt(std::string& passToEncrypt, std::string& generatedKey, std::string& encryptedPass);
bool runPassDecrypt(std::string& passToDecrypt, std::string& keyToDecrypt, std::string& decryptedPass);
void runPasscryptFeat(int& selectPassCrypt);

// ==================== MAIN ====================
int main(int argc, char* argv[])
{   
    relaunchInPowerShell(argc, argv);
    setupConsoleWindow();
    int mainMenuOption{};
    int passLength{};
    int selectPassCrypt{};
    bool runLoop = true;
    std::string passToCheck{};
    std::string passCheckPolicy{};
    do
    {
        showMainMenuUI(mainMenuOption);
        runLoop = handleMainMenu(mainMenuOption, passLength, passToCheck, passCheckPolicy, selectPassCrypt);
    } while (runLoop);
    return 0;
}

void showMainMenuUI(int& mainMenuOption)
{
    system("cls");
    std::cout << "\n\033[34m  =====================================\033[0m\n"
              << "\033[33m     🛡️ PASSWORD SECURITY UTILITY 🔒  \033[0m\n"
              << "\033[34m  =====================================\033[0m\n"
              << "\n      1. Generate Password\n"
              << "      2. Check Password Strength\n"
              << "      3. Check Password Policy\n"
              << "      4. Password Cryptography\n"
              << "      5. Security Recommendations\n"
              << "      6. Security Challenge\n"
              << "      7. Exit\n"
              << "\n\033[32m    Select option: \033[0m";
    getValidatedInput(mainMenuOption, 1, 7);
}

bool handleMainMenu(int& mainMenuOption, int& passLength, std::string& passToCheck, std::string& passCheckPolicy, int& selectPassCrypt)
{
    if (mainMenuOption == 1)
    {
        runGenPassFeat(passLength);
        return true;
    }
    else if (mainMenuOption == 2)
    {
        runCheckPassStrength(passToCheck);
        return true;
    }
    else if (mainMenuOption == 3)
    {
        runCheckPassPolicy(passCheckPolicy);
        return true;
    }
    else if (mainMenuOption == 4)
    {
        runPasscryptFeat(selectPassCrypt);
        return true;
    }
    else if (mainMenuOption == 5 || mainMenuOption == 6)
    {
        std::cout << "\n    Feature coming soon.\n";
        pauseScreen();
        return true;
    }
    else if (mainMenuOption == 7)
    {
        showStaticLoading("Exiting System ");
        exitsystem();
        return false;
    }
    else
    {
        invalidInput();
        return true;
    }
}

// ==================== GENERATE PASSWORD ====================
void showGenPassUI(int& passLength)
{
    system("cls");
    std::cout << "\n\033[33m  ============== 🔑 PASSWORD GENERATOR ⚙️ ==============\033[0m\n"
              << "\n      Enter desired password length (1-99): ";
    getValidatedInput(passLength, 1, 99);
}

void runGenPassFeat(int& passLength)
{
    while (true)
    {
        showGenPassUI(passLength);
        if (passLength == -1)
        {
            invalidInput();
            continue;
        }
        showStaticLoading("Generating Password ");
        std::string password = generatePassword(passLength);
        bool isValid = genPassValidation(password, passLength);
        if (!password.empty()) SecureZeroMemory(password.data(), password.length());
        if (!isValid)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }
        pauseScreen();
        break;
    }
}

bool genPassValidation(const std::string& password, int passLength)
{
    if (!password.empty() && static_cast<int>(password.length()) == passLength)
    {
        std::cout << "\n\033[32m      Password generated successfully.🎉\033[0m\n";
        std::cout << "\n      Generated Password: " << password << '\n';
        return true;
    }
    else
    {
        std::cout << "\n\033[33m     ⚠️ \033[0m\033[31mFailed to generate password. Please try again\033[0m\n";
        return false;
    }
}

bool secureRandomBytes(void* buffer, size_t length)
{
    NTSTATUS status = BCryptGenRandom(
        NULL, 
        static_cast<PUCHAR>(buffer), 
        static_cast<ULONG>(length), 
        BCRYPT_USE_SYSTEM_PREFERRED_RNG
    );
    return BCRYPT_SUCCESS(status);
}

std::string generatePassword(int passLength)
{
    const std::string characters =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!@#$%^&*";

    std::vector<unsigned char> randomIndices(passLength);
    if (!secureRandomBytes(randomIndices.data(), passLength)) return "";
    std::string generatedPassword;
    generatedPassword.reserve(passLength);
    for (int i = 0; i < passLength; ++i) generatedPassword += characters[randomIndices[i] % characters.length()];
    return generatedPassword;
}

// ==================== CHECK PASSWORD STRENGTH ====================
void showCheckPassStrengthUI(std::string& passToCheck)
{
    system("cls");
    std::cout << "\n\033[33m  ======== 💪 CHECK PASSWORD STRENGTH 🛡️ ========\033[0m\n"
              << "\n   Enter password to check strength: ";
    std::getline(std::cin, passToCheck);
}

bool runCheckPassStrength(std::string& passToCheck)
{
    while (true)
    {
        showCheckPassStrengthUI(passToCheck);
        if (passToCheck.empty())
        {
            invalidInput();
            continue;
        }
        showStaticLoading("Checking Your Password's Strength ");
        checkPasswordValidation(passToCheck);
        SecureZeroMemory(passToCheck.data(), passToCheck.length());
        pauseScreen();
        return true;
    }
}

void showCheckPassResult(const std::string& strengthClass, bool hasMinLength, 
                         bool hasUpper, bool hasLower, bool hasDigit, bool hasSpecial)
{
    std::string minLenMark = hasMinLength ? "\033[32m✔\033[0m" : "\033[31m✖\033[0m";
    std::string upperMark  = hasUpper     ? "\033[32m✔\033[0m" : "\033[31m✖\033[0m";
    std::string lowerMark  = hasLower     ? "\033[32m✔\033[0m" : "\033[31m✖\033[0m";
    std::string digitMark  = hasDigit     ? "\033[32m✔\033[0m" : "\033[31m✖\033[0m";
    std::string specMark   = hasSpecial   ? "\033[32m✔\033[0m" : "\033[31m✖\033[0m";
    std::cout << "\n      Password Analysis: \n"
              << "      " << minLenMark << "  Length Requirement\n"
              << "      " << upperMark  << "  Uppercase Letter\n"
              << "      " << lowerMark  << "  Lowercase Letter\n"
              << "      " << digitMark  << "  Number\n"
              << "      " << specMark   << "  Special Character\n"
              << "\n      Password Strength: " << strengthClass << '\n';
}

bool checkPasswordValidation(const std::string& passToCheck)
{
    if (passToCheck.empty())
    {
        std::cout << "\n\033[33m     ⚠️ \033[0m\033[31mFailed to analyze password. Please try again\033[0m\n";
        return false;
    }
    bool hasMinLength = false, hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    checkPassEvaluation(passToCheck, hasMinLength, hasUpper, hasLower, hasDigit, hasSpecial);
    std::string strengthClass;
    getStrengthClassification(strengthClass, hasMinLength, hasUpper, hasLower, hasDigit, hasSpecial);
    showCheckPassResult(strengthClass, hasMinLength, hasUpper, hasLower, hasDigit, hasSpecial);
    return true;
}

void checkPassEvaluation(const std::string& passToCheck, bool& hasMinLength, 
                         bool& hasUpper, bool& hasLower, bool& hasDigit, bool& hasSpecial)
{
    hasMinLength = (passToCheck.length() >= 8u);
    for (unsigned char ch : passToCheck)
    {
        if (std::isupper(ch)) hasUpper = true;
        else if (std::islower(ch)) hasLower = true;
        else if (std::isdigit(ch)) hasDigit = true;
        else if (std::ispunct(ch)) hasSpecial = true;
    }
}

void getStrengthClassification(std::string& strengthClass, bool hasMinLength, bool hasUpper, bool hasLower, bool hasDigit, bool hasSpecial)
{
    int score = (hasMinLength?1:0) + (hasUpper?1:0) + (hasLower?1:0) + (hasDigit?1:0) + (hasSpecial?1:0);
    if (score == 5) strengthClass = "\033[1;32mVery Strong\033[0m";
    else if (score == 4) strengthClass = "\033[1;32mStrong\033[0m";
    else if (score == 3) strengthClass = "\033[1;33mModerate\033[0m";
    else if (score == 2) strengthClass = "\033[1;31mWeak\033[0m";
    else strengthClass = "\033[1;31mVery Weak\033[0m";
}

// ==================== CHECK PASSWORD POLICY ====================
void showCheckPassPolicyUI(std::string& passCheckPolicy)
{
    system("cls");
    std::cout << "\n\033[33m  ======== ⚖️ CHECK PASSWORD POLICY 📜 ========\033[0m\n"
              << "\n  Enter password to check policy: ";
    std::getline(std::cin, passCheckPolicy);
}

void showPassPolicyResultUI(const std::string& PolicyClass, bool hasMinLength, 
                         bool hasUpper, bool hasLower, bool hasDigit, bool hasSpecial)
{
    std::string minLenMark = hasMinLength ? "\033[32m✔\033[0m" : "\033[31m✖\033[0m";
    std::string upperMark  = hasUpper     ? "\033[32m✔\033[0m" : "\033[31m✖\033[0m";
    std::string lowerMark  = hasLower     ? "\033[32m✔\033[0m" : "\033[31m✖\033[0m";
    std::string digitMark  = hasDigit     ? "\033[32m✔\033[0m" : "\033[31m✖\033[0m";
    std::string specMark   = hasSpecial   ? "\033[32m✔\033[0m" : "\033[31m✖\033[0m";

    std::cout << "\n      Password Policy Analysis: \n"
              << "      " << minLenMark << "  Minimum Length: 8 characters\n"
              << "      " << upperMark  << "  Uppercase Letter\n"
              << "      " << lowerMark  << "  Lowercase Letter\n"
              << "      " << digitMark  << "  Number\n"
              << "      " << specMark   << "  Special Character\n"
              << " \n"
              << "      ────────────────────────────\n"
              << "        Policy Status: " << PolicyClass << '\n'
              << "      ────────────────────────────\n";
}

bool runCheckPassPolicy(std::string& passCheckPolicy)
{
    while (true)
    {
        showCheckPassPolicyUI(passCheckPolicy);
        if (passCheckPolicy.empty())
        {
            invalidInput();
            continue;
        }
        showStaticLoading("Checking Password Policy ");
        checkPassPolicyValidation(passCheckPolicy);
        SecureZeroMemory(passCheckPolicy.data(), passCheckPolicy.length());
        pauseScreen();
        return true;
    }
}

bool checkPassPolicyValidation(const std::string& passCheckPolicy)
{
    if (passCheckPolicy.empty()) return false;
    bool hasMinLength = false, hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    PassPolicyEvaluation(passCheckPolicy, hasMinLength, hasUpper, hasLower, hasDigit, hasSpecial);
    std::string PolicyClass;
    getPolicyClassification(PolicyClass, hasMinLength, hasUpper, hasLower, hasDigit, hasSpecial);
    showPassPolicyResultUI(PolicyClass, hasMinLength, hasUpper, hasLower, hasDigit, hasSpecial);
    return true;
}

void PassPolicyEvaluation(const std::string& passCheckPolicy, bool& hasMinLength, 
                         bool& hasUpper, bool& hasLower, bool& hasDigit, bool& hasSpecial)
{
    hasMinLength = (passCheckPolicy.length() >= 8u);
    for (unsigned char ch : passCheckPolicy)
    {
        if (std::isupper(ch)) hasUpper = true;
        else if (std::islower(ch)) hasLower = true;
        else if (std::isdigit(ch)) hasDigit = true;
        else if (std::ispunct(ch)) hasSpecial = true;
    }
}

void getPolicyClassification(std::string& PolicyClass, bool hasMinLength, bool hasUpper, bool hasLower, bool hasDigit, bool hasSpecial)
{
    int score = (hasMinLength?1:0) + (hasUpper?1:0) + (hasLower?1:0) + (hasDigit?1:0) + (hasSpecial?1:0);
    PolicyClass = (score == 5) ? "\033[32m✔  PASSED\033[0m" : "\033[31m✖  FAILED\033[0m";
}

// ==================== PASSWORD CRYPTOGRAPHY ====================
void showPassCryptUI(int& selectPassCrypt)
{
    system("cls");
    std::cout << "\n\033[33m  ======== 🔐 PASSWORD CRYPTOGRAPHY 🛡️ ========\033[0m\n"
              << "\n      1. Encrypt Password\n"
              << "      2. Decrypt Password\n"
              << "      3. View Encrypted Vault\n"
              << "      4. Delete Vault Entry\n"
              << "      5. Return to Main Menu\n"
              << "\n  Select option: ";
    getValidatedInput(selectPassCrypt, 1, 5);
}

void showPassEncryptUI(std::string& serviceName, std::string& passToEncrypt)
{
    system("cls");
    std::cout << "\n\033[33m  ============ 🔐 PASSWORD ENCRYPTION 🛡️ ============\033[0m\n"
              << "\n  Enter Service Name (e.g., GitHub, Gmail) : ";
    std::getline(std::cin, serviceName);
    std::cout << "  Enter Password to Encrypt                : ";
    std::getline(std::cin, passToEncrypt);
}

void showPassDecryptUI(std::string& passToDecrypt, std::string& keyToDecrypt)
{
    system("cls");
    std::cout << "\n\033[33m  ============ 🔓 PASSWORD DECRYPTION 🛡️ ============\033[0m\n"
              << "\n  Enter Encrypted Password: ";
    std::getline(std::cin, passToDecrypt);
    std::cout << "  Enter Encryption Key: ";
    std::getline(std::cin, keyToDecrypt);
}

void showPassEncryptResultUI(const std::string& serviceName, const std::string& generatedKey, const std::string& encryptedPass)
{
    std::cout << "\n\033[32m      Password Encrypted successfully.🎉\033[0m\n"
              << "\n      Service            : " << serviceName << "\n"
              << "      Encryption Key     : " << generatedKey << "\n"
              << "      Encrypted Password : " << encryptedPass << "\n";
}

void showPassDecryptResultUI(std::string& decryptedPass)
{
    std::cout << "\n\033[32m      Password Decrypted successfully.🎉\033[0m\n"
              << "\n      Decrypted Password : " << decryptedPass << "\n";
}

bool runPassEncrypt(std::string& passToEncrypt, std::string& generatedKey, std::string& encryptedPass)
{
    std::string serviceName;
    showPassEncryptUI(serviceName, passToEncrypt);
    if (serviceName.empty() || passToEncrypt.empty())
    {
        invalidInput();
        return false;
    }
    showStaticLoading("Generating encryption key ");
    generatedKey = generateRandomKey(passToEncrypt.length());
    showStaticLoading("Encrypting password ");
    std::string rawEncrypted = xorCipher(passToEncrypt, generatedKey);
    encryptedPass = stringToHex(rawEncrypted);
    if (saveToLocalVault(serviceName, encryptedPass)) {
        showPassEncryptResultUI(serviceName, generatedKey, encryptedPass);
    } else {
        std::cout << "\n\033[31m      Encryption succeeded, but failed to write to vault file.\033[0m\n";
    }

    SecureZeroMemory(passToEncrypt.data(), passToEncrypt.length());
    pauseScreen();
    return true;
}

bool runPassDecrypt(std::string& passToDecrypt, std::string& keyToDecrypt, std::string& decryptedPass)
{
    showPassDecryptUI(passToDecrypt, keyToDecrypt);
    if (passToDecrypt.empty() || keyToDecrypt.empty())
    {
        invalidInput();
        return false;
    }
    showStaticLoading("Decrypting password ");
    std::string rawEncrypted = hexToString(passToDecrypt);
    if (rawEncrypted.empty())
    {
        std::cout << "\n\033[31m      Invalid hex format in encrypted password string.\033[0m\n";
        pauseScreen();
        return false;
    }
    decryptedPass = xorCipher(rawEncrypted, keyToDecrypt);
    showPassDecryptResultUI(decryptedPass);
    SecureZeroMemory(decryptedPass.data(), decryptedPass.length());
    SecureZeroMemory(keyToDecrypt.data(), keyToDecrypt.length());
    pauseScreen();
    return true;
}

void runPasscryptFeat(int& selectPassCrypt)
{
    std::string passToEncrypt, generatedKey, encryptedPass;
    std::string passToDecrypt, keyToDecrypt, decryptedPass;
    do
    {
        showPassCryptUI(selectPassCrypt);
        if (selectPassCrypt == 1) runPassEncrypt(passToEncrypt, generatedKey, encryptedPass);
        else if (selectPassCrypt == 2) runPassDecrypt(passToDecrypt, keyToDecrypt, decryptedPass);
        else if (selectPassCrypt == 3) 
        {
            viewLocalVault();
            pauseScreen();
        }
        else if (selectPassCrypt == 4) deleteVaultEntry();
        else if (selectPassCrypt == 5) return;
        else invalidInput();
    } while (selectPassCrypt != 5);
}

// ==================== Utilities ====================
void secureWipeString(std::string& str)
{
    if (!str.empty())
    {
        SecureZeroMemory(str.data(), str.capacity());
        str.clear();
    }
}
std::string xorCipher(const std::string& text, const std::string& key)
{
    if (key.empty()) return text;
    std::string output = text;
    for (size_t i = 0; i < text.length(); ++i) output[i] = text[i] ^ key[i % key.length()];
    return output;
}

std::string stringToHex(const std::string& input)
{
    static const char hexDigits[] = "0123456789ABCDEF";
    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hexDigits[c >> 4]);
        output.push_back(hexDigits[c & 15]);
    }
    return output;
}

std::string hexToString(const std::string& input)
{
    if (input.length() % 2 != 0) return "";
    std::string output;
    output.reserve(input.length() / 2);

    auto hexCharToInt = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        return -1;
    };

    for (size_t i = 0; i < input.length(); i += 2)
    {
        int h = hexCharToInt(input[i]);
        int l = hexCharToInt(input[i + 1]);
        if (h == -1 || l == -1) return ""; // Invalid hex input
        output.push_back(static_cast<char>((h << 4) | l));
    }
    return output;
}

std::string generateRandomKey(size_t length) 
{
    const std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::vector<unsigned char> randomIndices(length);
    if (!secureRandomBytes(randomIndices.data(), length)) return "";

    std::string key = "";
    for (size_t i = 0; i < length; ++i) key += charset[randomIndices[i] % charset.length()];
    return key;
}

void pauseScreen(const std::string& message)
{
    std::cout << "\n    " << message;
    std::string temp;
    std::getline(std::cin, temp);
}

void playSoundEffect(const std::string& soundFile)
{
    PlaySoundA(soundFile.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

void getValidatedInput(int& input, int minimumValue, int maximumValue)
{
    std::string line;
    if (std::getline(std::cin, line))
    {
        std::stringstream ss(line);
        if (ss >> input && ss.eof())
        {
            if (input >= minimumValue && input <= maximumValue) return;
        }
    }
    input = -1;
}

void showStaticLoading(const std::string& message, int dots, int delayMs)
{
    std::cout << "\n    ⌛ \033[36m" << message << "\033[0m";
    std::cout.flush();
    for (int i = 0; i < dots; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        std::cout << "\033[36m.\033[0m" << std::flush;
    }
    std::cout << "\n";
}

void setupConsoleWindow()
{
    setConsoleFontSizeAndSize(28, 60, 20);
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    SetConsoleTitleA("Password Security Utility v1.0.0");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
    system("cls");
}

void setConsoleFontSizeAndSize(short fontSize, short widthInCols, short heightInRows)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    if (GetCurrentConsoleFontEx(hOut, FALSE, &cfi))
    {
        cfi.dwFontSize.Y = fontSize;
        SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    }
    COORD bufferSize = { widthInCols, heightInRows };
    SetConsoleScreenBufferSize(hOut, bufferSize);
    SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(widthInCols - 1), static_cast<SHORT>(heightInRows - 1) };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
}

void relaunchInPowerShell(int argc, char* argv[])
{
    if (argc == 1 || std::string(argv[argc - 1]) != "--in-ps") 
    {
        HWND hwnd = GetConsoleWindow();
        if (hwnd != NULL) ShowWindow(hwnd, SW_HIDE);
        std::string exePath = argv[0];
        size_t pos = 0;
        while ((pos = exePath.find("'", pos)) != std::string::npos) 
        {
            exePath.replace(pos, 1, "''");
            pos += 2;
        }
        std::string command = "conhost.exe powershell.exe -NoProfile -ExecutionPolicy Bypass -Command \"& '" + exePath + "' --in-ps\"";
        std::system(command.c_str());
        std::exit(0);
    }
}

void exitsystem()
{
    system("cls");
    playSoundEffect("assets/thank_you.wav");
    std::cout << "\n\033[32m    Application Exited Successfully. Goodbye!\033[0m \033[31m💖\033[0m\n\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

void invalidInput()
{
    MessageBeep(MB_ICONWARNING);
    std::cout << "\n\033[31m    Invalid Input. Try Again.\033[0m\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}

bool restrictFileToCurrentUser(const std::string& filePath)
{
    HANDLE hToken = NULL;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) return false;
    
    DWORD dwSize = 0;
    GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
    std::vector<BYTE> buffer(dwSize);
    if (!GetTokenInformation(hToken, TokenUser, buffer.data(), dwSize, &dwSize))
    {
        CloseHandle(hToken);
        return false;
    }
    
    PTOKEN_USER pTokenUser = reinterpret_cast<PTOKEN_USER>(buffer.data());
    PSID pUserSid = pTokenUser->User.Sid;
    
    EXPLICIT_ACCESS_A ea{};
    ea.grfAccessPermissions = GENERIC_ALL;
    ea.grfAccessMode = SET_ACCESS;
    ea.grfInheritance = NO_INHERITANCE;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
    ea.Trustee.ptstrName = (LPSTR)pUserSid;
    
    PACL pNewAcl = NULL;
    if (SetEntriesInAclA(1, &ea, NULL, &pNewAcl) != ERROR_SUCCESS)
    {
        CloseHandle(hToken);
        return false;
    }
    
    DWORD dwRes = SetNamedSecurityInfoA(
        (LPSTR)filePath.c_str(),
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION | PROTECTED_DACL_SECURITY_INFORMATION,
        NULL, NULL, pNewAcl, NULL
    );
    
    LocalFree(pNewAcl);
    CloseHandle(hToken);
    return (dwRes == ERROR_SUCCESS);
}

void viewLocalVault()
{
    system("cls");
    std::cout << "\n\033[33m  ======== 📂 LOCAL ENCRYPTED VAULT ========\033[0m\n\n";
    std::ifstream inFile(VAULT_FILE);
    if (!inFile.is_open())
    {
        std::cout << "  \033[31mNo vault file found at 'data/vault.dat'.\033[0m\n";
        return;
    }
    std::string line;
    int recordCount = 0;
    while (std::getline(inFile, line))
    {
        size_t delimiterPos = line.find('|');
        if (delimiterPos != std::string::npos)
        {
            std::string service = line.substr(0, delimiterPos);
            std::string payload = line.substr(delimiterPos + 1);
            recordCount++;
            std::cout << "  \033[36m[" << recordCount << "] Service :\033[0m " << service << "\n"
                      << "      \033[90mPayload :\033[0m " << payload << "\n\n";
        }
    }
    if (recordCount == 0) std::cout << "  Vault file is currently empty.\n";
    inFile.close();
}

bool saveToLocalVault(const std::string& serviceName, const std::string& encryptedPayload)
{
    try 
    {
        if (!fs::exists(VAULT_DIR)) fs::create_directory(VAULT_DIR);
        std::ofstream outFile(VAULT_FILE, std::ios::app);
        if (!outFile.is_open()) return false;
        outFile << serviceName << "|" << encryptedPayload << "\n";
        outFile.close();
        restrictFileToCurrentUser(VAULT_FILE);
        return true;
    }
    catch (const std::exception&) {
        return false;
    }
}

bool deleteVaultEntry()
{
    system("cls");
    std::cout
        << "\n\033[33m"
        << "  ======== 🗑️ DELETE VAULT ENTRY ========"
        << "\033[0m\n\n";

    std::ifstream inFile(VAULT_FILE);

    if (!inFile.is_open())
    {
        std::cout
            << "  \033[31m"
            << "No vault file found at 'data/vault.dat'."
            << "\033[0m\n";

        pauseScreen();
        return false;
    }

    std::vector<std::string> records;
    std::string line;

    while (std::getline(inFile, line))
    {
        if (!line.empty())
        {
            size_t delimiterPos = line.find('|');
            if (delimiterPos != std::string::npos) records.push_back(line);
        }
    }

    inFile.close();

    if (records.empty())
    {
        std::cout << "  Vault file is currently empty.\n";
        pauseScreen();
        return false;
    }

    std::cout << "  Available Vault Entries:\n\n";

    for (size_t i = 0; i < records.size(); ++i)
    {
        size_t delimiterPos = records[i].find('|');
        std::string service = records[i].substr(0, delimiterPos);
        std::cout << "  \033[36m["
                  << i + 1
                  << "]\033[0m "
                  << service
                  << "\n";
    }

    std::cout << "\n  Select entry to delete: ";

    int selectedEntry;

    getValidatedInput(selectedEntry, 1, static_cast<int>(records.size()));

    if (selectedEntry == -1)
    {
        invalidInput();
        return false;
    }

    size_t selectedIndex = static_cast<size_t>(selectedEntry - 1);
    size_t delimiterPos = records[selectedIndex].find('|');
    std::string service = records[selectedIndex].substr(0, delimiterPos);
    std::string payload = records[selectedIndex].substr(delimiterPos + 1);
    system("cls");
    std::cout << "\n\033[33m"
              << "  ======== ⚠️ DELETE VAULT ENTRY ========"
              << "\033[0m\n\n"
              << "  You are about to permanently delete:\n\n"
              << "      Service : "
              << service
              << "\n"
              << "      Payload : "
              << payload
              << "\n\n"
              << "  \033[31m⚠️ WARNING\033[0m\n"
              << "  This action cannot be undone.\n\n"
              << "      1. Yes, Delete\n"
              << "      2. No, Cancel\n"
              << "\n  Select option: ";

    int confirmation;
    getValidatedInput(confirmation, 1, 2);

    if (confirmation == -1)
    {
        invalidInput();
        return false;
    }

    if (confirmation == 2)
    {
        std::cout << "\n\033[33m"
                  << "  ❎ DELETE CANCELLED"
                  << "\033[0m\n"
                  << "  No vault entries were changed.\n";
        pauseScreen();
        return false;
    }

    const std::string tempVaultFile = VAULT_FILE + ".tmp";
    std::ofstream outFile(tempVaultFile, std::ios::trunc);

    if (!outFile.is_open())
    {
        std::cout << "\n\033[31m"
                  << "  Failed to create temporary vault file."
                  << "\033[0m\n";
        pauseScreen();
        return false;
    }

    for (size_t i = 0; i < records.size(); ++i)
    {
        if (i != selectedIndex)
        {
            outFile << records[i] << '\n';
        }
    }

    outFile.close();

    try
    {
        fs::remove(VAULT_FILE);
        fs::rename(tempVaultFile, VAULT_FILE);
    }
    catch (const std::exception&)
    {
        if (fs::exists(tempVaultFile))
        {
            fs::remove(tempVaultFile);
        }

        std::cout << "\n\033[31m"
                  << "  Failed to update the vault file."
                  << "\033[0m\n";
        pauseScreen();
        return false;
    }

    // Re-apply current-user restriction
    restrictFileToCurrentUser(VAULT_FILE);
    std::cout << "\n\033[32m"
              << "  🗑️ ENTRY DELETED"
              << "\033[0m\n"
              << "\n  The "
              << service
              << " vault entry has been permanently removed.\n";
    pauseScreen();
    return true;
}