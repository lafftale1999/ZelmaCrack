#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "dbConnection.h"
#include <vector>

typedef enum
{
    SINGLE,
    DATABASE
}Victim;

// UI
int mainMenu();
int newUser();
int runZelmaCracker(Victim victim);
int logIn();

std::string inputEmail();
std::string inputPassword();
HashMethod inputMethod();
bool checkEmail(std::string email);
CheckPasswordMessage checkPassword(std::string password);

int getIntInput(std::string menumessage, std::string prompt, std::vector<std::string> menuOptions);
std::string getStringInput(std::string menumessage, std::string prompt, std::vector<std::string> menuOptions);

#endif