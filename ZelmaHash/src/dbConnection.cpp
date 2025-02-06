#include "../include/dbConnection.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

UpdateMessage saveUser(User &user)
{
    std::ofstream file(CUSTOMER_DB_ADRESS, std::ios::app);

    if(!file.is_open())
    {
        throw std::invalid_argument("File could not be opened");
    }

    file << user.getUserEmail() + ' ' + user.getPassword() + ' ' + user.getSalt() << std::endl;

    file.close();

    return SUCCESS;
}

std::optional<User> getUser(std::string userEmail)
{
    std::ifstream file(CUSTOMER_DB_ADRESS);
    
    if(!file.is_open())
    {
        throw std::invalid_argument("File could not be opened");
    }

    std::string temp;
    bool userFound = false;

    while(std::getline(file, temp))
    {
        if (temp.find(userEmail) != std::string::npos)
        {
            userFound = true;
            break;
        }
    }

    if(userFound)
    {
        // name, password, salt
        std::vector<std::string> userInformation = {"", "", ""};

        int index = 0;

        for(char c : temp)
        {
            if(c != ' ') userInformation[index].push_back(c);
            else index++;
        }

        return User(userInformation[0], userInformation[1], userInformation[2]);
    }

    return std::nullopt;
}

LogInMessage logInUser(std::string userEmail, std::string password)
{
    auto user = getUser(userEmail);

    if(user)
    {
        if(user->getPassword().length() == 32)
        {
            if(user->getPassword() == makeMD5(password + user->getSalt()))
                return LOG_IN;
        }

        else if (user->getPassword().length() == 64)
        {
            if(user->getPassword() == makeSHA256(password + user->getSalt()))
                return LOG_IN;
        }

        else return WRONG_PASSWORD;
    }

    else return WRONG_EMAIL;
}