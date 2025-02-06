#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include "user.h"
#include <optional>
#define CUSTOMER_DB_ADRESS "data/users/users.txt"

typedef enum
{
    FAIL,
    SUCCESS
}UpdateMessage;

typedef enum
{
    WRONG_EMAIL,
    WRONG_PASSWORD,
    LOG_IN
} LogInMessage;

UpdateMessage saveUser(User &user);
std::optional<User> getUser(std::string userEmail);
LogInMessage logInUser(std::string userEmail, std::string password);

#endif