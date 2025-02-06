#ifndef MY_USER_H
#define MY_USER_H

#include <iostream>
#include "zelmaHasher.h"

typedef enum
{
    TOO_SHORT,
    NO_UPPER_CASE,
    NO_LOWER_CASE,
    NO_NUMBER,
    NO_SPECIAL_CHARACTER,
    PASSWORD_PASS
} CheckPasswordMessage;

class User
{
    std::string email;
    std::string password;
    std::string salt;

public:
    User(std::string userEmail, std::string password, std::string salt);
    User(std::string userEmail, std::string password, HashMethod hashMethod);
    std::string getUserEmail();
    std::string getPassword();
    std::string getSalt();

    int setUserEmail(std::string email);
    int setPassword(std::string password, HashMethod hashMethod);
    void setSalt(int saltLength);

    bool checkEmail(std::string email);
    CheckPasswordMessage checkPassword(std::string password);
};

#endif