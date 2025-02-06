#ifndef ZELMA_CRACKER_H
#define ZELMA_CRACKER_H

#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <unordered_set>
#include "user.h"

#define MD5_FILE_PATH "data/knownPasswords/100MD5.txt"
#define SHA256_FILE_PATH "data/knownPasswords/100SHA256.txt"

typedef enum
{
    CRACKED,
    NOT_CRACKED
}ZelmaMessage;

typedef enum
{
    AS_IS,
    TRY_NUMBERS,
    TRY_ADDONS,
    TRY_LEETSPEAK
}BruteForceLevel;

class ZelmaCracker
{
    std::unordered_set<std::string> commonPasswords;
    std::unordered_map<char, std::vector<std::string>> leetSpeakDict;
    std::unordered_set<char> leetLetters;
    std::vector<std::string> commonAddons;

    std::vector<User> users;
    std::string cracked;
    std::string hashed;
    std::string salt;
    HashMethod method;

    BruteForceLevel bfl[4];
    std::string filePath;

public:
    ZelmaCracker(std::string hashedPassword, std::string salt, std::string bfi, int searchSize);
    ZelmaCracker(std::string bfi, int searchSize);

    ZelmaMessage crackPassword();
    void loadFile();

    ZelmaMessage checkPassword(std::string password);
    ZelmaMessage checkNumbers(const std::string &password);
    ZelmaMessage checkAddons(const std::string &password);
    ZelmaMessage checkLeet(std::string password);

    void loadLeetSpeak();
    void loadCommonAddons();
    void loadUsers();
    int addToList();

    void init(std::string bfi, int searchSize);
};

#endif