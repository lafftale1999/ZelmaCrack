#include "../include/userInterface.h"
#include <cstdlib>
#include <regex>
#include <thread>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <string>
#include <iomanip>
#include "../include/zelmaCracker.h"

void clearScreen()
{
    system("cls");
}

int mainMenu()
{
    int userInput = getIntInput("Welcome to ZelmaHash!\nEnter a number to start!", "> ", {"Create new user", "Log in user", "Crack password", "Crack Database", "Exit program"});
    
    return userInput;
}

int newUser()
{
    clearScreen();

    std::cout << "Create new user." << std::endl;

    std::string email = inputEmail();
    if(email == "x") return 0;

    std::string password = inputPassword();
    if(password == "x") return 0;

    HashMethod method = inputMethod();

    User user(email, password, method);
    saveUser(user);

    std::cout << "User created!" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    
    return 0;
}

std::string inputEmail()
{
    std::cout << "Enter your email or (x) to go back!" << std::endl;

    std::string email;
    
    while(true)
    {
        std::cin >> email;

        if(getUser(email))
        {
            std::cout << "You already have an account!" << std::endl;
            continue;
        }

        if(email == "x") return "x";
        if(checkEmail(email)) return email;

        std::cout << "Enter a valid email adress!" << std::endl;
    }
}

std::string inputPassword()
{
    std::cout << "Enter your password or (x) to go back!" << std::endl;
    std::cout << "Password rules:" << std::endl;
    std::cout << "- 8 characters long or more" << std::endl;
    std::cout << "- 1 or more uppercase characters" << std::endl;
    std::cout << "- 1 or more lowercase characters" << std::endl;
    std::cout << "- 1 or more digits" << std::endl;
    std::cout << "- 1 or more special characters" << std::endl;
    std::cout << std::endl;

    std::string password;

    while(true)
    {   
        std::cout << "> ";

        try
        {
            std::cin >> password;

            if(password == "x") return "x";
            if (checkPassword(password) == PASSWORD_PASS) return password;
        }
        
        catch(const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            std::cin.clear();
        }
    }
}

HashMethod inputMethod()
{   
    int userInput = getIntInput("ZelmaHash\nChoose Hashing Algorithm", "> ", {"MD5", "SHA256"});
    
    return userInput == 1 ? USE_MD5 : USE_SHA256;
    
}

bool checkEmail(std::string email)
{
    std::regex emailPattern(R"(^[a-zA-Z0-9][a-zA-Z0-9._%+-]{0,63}@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,10}$)");

    return std::regex_match(email, emailPattern);
}

CheckPasswordMessage checkPassword(std::string password)
{
    if (password.length() < 8) throw std::invalid_argument("Please enter 8 or more characters!");
    else if (!std::regex_search(password, std::regex("[A-Z]"))) throw std::invalid_argument("Must containt atleast 1 uppercase character!");
    else if (!std::regex_search(password, std::regex("[a-z]"))) throw std::invalid_argument("Must containt atleast 1 lowercase character!");
    else if (!std::regex_search(password, std::regex("[0-9]"))) throw std::invalid_argument("Must containt atleast 1 digit!");
    else if (!std::regex_search(password, std::regex(R"([\W_])"))) throw std::invalid_argument("Must containt atleast 1 special character!");

    return PASSWORD_PASS;
}

int logIn()
{
    clearScreen();

    std::cout << "Log in as user." << std::endl;

    std::string email;
    std::string password;

    try
    {
        std::cout << "Email: ";
        std::cin >> email;

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        std::cout << "Password: ";
        std::cin >>password;    
    }
    catch(const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    LogInMessage login = logInUser(email, password);

    switch (login)
    {
    case WRONG_EMAIL:
        std::cout << "WRONG EMAIL!" << std::endl;
        break;

    case WRONG_PASSWORD:
        std::cout << "WRONG PASSWORD!" << std::endl;
        break;

    case LOG_IN:
        std::cout << "You are now logged in!" << std::endl;
        break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}

int runZelmaCracker(Victim victim)
{
    std::string password;
    std::string salt;

    if(victim == SINGLE)
    {
        clearScreen();
        std::cout << "ZelmaCracker. Try your password!" << std::endl;
        std::cout << "Enter your password: ";
        std::cin >> password;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        std::cout << "Enter your salt: ";
        std::cin >> salt;
    
        // hash the password
        int userInput = getIntInput("ZelmaCracker. Try your password!", "> ", {"MD5", "SHA256"});
        std::string method = userInput == 1 ? "MD5" : "SHA256";
        password = userInput == 1 ? makeMD5(password + salt) : makeSHA256(password + salt);
    }

    // decide brute-force level
    std::string userStrInput = getStringInput("ZelmaCracker. Try your password\n"
                                "Trying passwords without spice as standard. Trying with first letter lower and uppercase."
                                "Enter the numbers for spice. Ex - '13' for (+ numbers between 0-99 and + leetspeak)", "> ",
                                {"+ numbers between 0-99 (5.5paws/ms)",
                                "+ normal addons (slow 0.1paws/ms)",
                                "+ leetspeak (VERY slow 0.02paws/ms)"});


    // choose password file
    int userIntInput = getIntInput("ZelmaCracker. Try your password!", "> ",
                            {"Top 100",
                            "Top 1000",
                            "Top 10000",
                            "Top 100000",
                            "Top 1000000"});

    // calculate time to crack
    int file = userIntInput;
    int fileSize = std::pow(10, userIntInput + 1);
    float time;
    if(userStrInput.find('1') != std::string::npos) time = fileSize / 500;
    if(userStrInput.find('2') != std::string::npos) time += (float)fileSize / 5.5F;
    if(userStrInput.find('3') != std::string::npos) time += (float)fileSize / 0.1F;
    if(userStrInput.find('4') != std::string::npos) time += (float)fileSize / 0.02F;

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << time;
    std::string formattedTime = stream.str();

    // show userchoices
    userIntInput = getIntInput(
        "ZelmaCracker. Try your password\n"
        "Brute-force: " + userStrInput + "\n"
        "File size: " + std::to_string(fileSize) + "\n"
        "Max time / paws: " + formattedTime + " ms",
        "> ",
        {"Let Zelma lick this paw", "Abort pawlicking"}
    );

    clearScreen();

    if(userIntInput == 1 && victim == SINGLE)
    {
        ZelmaCracker zc(password,salt,userStrInput,file);
    }

    if(userIntInput == 1 && victim == DATABASE)
    {
        ZelmaCracker zc(userStrInput,file);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::cout << std::endl << "Press enter to go back to menu" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return 0;
}

int getIntInput(std::string menumessage, std::string prompt, std::vector<std::string> menuOptions)
{
    clearScreen();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::cout << menumessage << std::endl;
    std::cout << "---------------------------" << std::endl;

    for(int i = 0; i < menuOptions.size(); i++)
    {
        std::cout << i + 1 << ". " << menuOptions[i] << std::endl;
    }

    std::cout << prompt;

    int userInput;

    while(true)
    {
        try
        {
            std::cin >> userInput;

            if(std::cin.fail()) throw std::runtime_error("Please enter an integer between " + std::to_string(1) + " and " + std::to_string(menuOptions.size()));

            if(userInput > 0 && userInput <= menuOptions.size()) return userInput;

            std::cout << "Please enter a number between " << 1 << " and " << menuOptions.size() << std::endl;
        }
        catch(const std::exception &e)
        {
            std::cout << e.what() << std::endl;

            std::cin.clear();
        }
    }
}

std::string getStringInput(std::string menumessage, std::string prompt, std::vector<std::string> menuOptions)
{
    clearScreen();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::cout << menumessage << std::endl;
    std::cout << "---------------------------" << std::endl;

    for(int i = 0; i < menuOptions.size(); i++)
    {
        std::cout << i + 1 << ". " << menuOptions[i] << std::endl;
    }

    std::cout << prompt;

    std::string userInput;

    while(true)
    {
        try
        {
            std::cin >> userInput;

            if(std::cin.fail()) throw std::runtime_error("Please enter an number between " + std::to_string(1) + " and " + std::to_string(menuOptions.size()));

            if([](const std::string& str)
            {
                return std::all_of(str.begin(), str.end(), [](char c) {
                    return c >= '1' && c <= '4';
                });
            }(userInput)) return userInput;

            std::cout << "Please enter a number between " << 1 << " and " << menuOptions.size() << std::endl;
        }
        catch(const std::exception &e)
        {
            std::cout << e.what() << std::endl;

            std::cin.clear();
        }
    }
}