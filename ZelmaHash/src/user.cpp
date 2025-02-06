#include "../include/user.h"
#include <stdexcept>
#include <regex>

User::User(std::string userEmail, std::string password, std::string salt):
email(userEmail), password(password), salt(salt){}

User::User(std::string userEmail, std::string password, HashMethod hashMethod)
{
    int e = this->setUserEmail(userEmail);
    int p = this->setPassword(password, hashMethod);

    if(!e && !p) std::invalid_argument("Account not created");

    std::cout << "Account created" << std::endl;
}

std::string User::getUserEmail()
{
    return email;
}

std::string User::getPassword()
{
    return password;
}

std::string User::getSalt()
{
    return salt;
}

int User::setUserEmail(std::string email)
{   
    bool isValid = checkEmail(email);

    if(!isValid) throw std::invalid_argument("Invalid email. Please check your adress: " + email);
    else this->email = email;

    return 1;
}

int User::setPassword(std::string password, HashMethod hashMethod)
{
    CheckPasswordMessage isValid = checkPassword(password);

    switch(isValid)
    {
        case TOO_SHORT:
            throw std::invalid_argument("The length of the password need to larger than 8 characters!");

        case NO_LOWER_CASE:
            throw std::invalid_argument("The password must contain 1 lowercase character!");

        case NO_UPPER_CASE:
            throw std::invalid_argument("The password must contain 1 uppercase character!");

        case NO_SPECIAL_CHARACTER:
            throw std::invalid_argument("The password must contain 1 special character!");

        case NO_NUMBER:
            throw std::invalid_argument("The password must contain 1 number!");

        case PASSWORD_PASS:
            this->setSalt(5);
            if(hashMethod == USE_MD5) this->password = makeMD5(password + this->salt);
            else if(hashMethod == USE_SHA256) this->password = makeSHA256(password + this->salt);
            break;
    }

    return 1;
}

void User::setSalt(int saltLength)
{
    std::string saltString = "qwertyuiopasdfghjklzxcvbnm!#%&/()=?@${[]}1234567890";
    std::string tempString;

    for(int i = 0; i < saltLength; i++)
    {
        int rnd = rand() % saltString.length();
        tempString.push_back(saltString[rnd]);
    }

    this->salt = tempString;
}

bool User::checkEmail(std::string email)
{
    std::regex emailPattern(R"(^[a-zA-Z0-9][a-zA-Z0-9._%+-]{0,63}@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,10}$)");

    return std::regex_match(email, emailPattern);
}

CheckPasswordMessage User::checkPassword(std::string password)
{
    if (password.length() < 8) return TOO_SHORT;
    else if (!std::regex_search(password, std::regex("[A-Z]"))) return NO_UPPER_CASE;
    else if (!std::regex_search(password, std::regex("[a-z]"))) return NO_LOWER_CASE;
    else if (!std::regex_search(password, std::regex("[0-9]"))) return NO_NUMBER;
    else if (!std::regex_search(password, std::regex(R"([\W_])"))) return NO_SPECIAL_CHARACTER;

    return PASSWORD_PASS;
}