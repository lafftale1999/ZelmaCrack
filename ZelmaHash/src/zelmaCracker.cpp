#include "../include/zelmaCracker.h"
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <future>

ZelmaCracker::ZelmaCracker(std::string hashedPassword, std::string salt, std::string bfi, int searchSize)
{
    this->hashed = hashedPassword;
    this->salt = salt;
    
    init(bfi, searchSize);

    std::cout << "\nPrrrbt?? =^.^=\n";
    auto start = std::chrono::high_resolution_clock::now();
    ZelmaMessage message = crackPassword();
    auto end = std::chrono::high_resolution_clock::now();

    switch (message)
    {
    case CRACKED:
        std::cout << "\nThe password has been cracked: " + cracked << std::endl;
        break;
    
    case NOT_CRACKED:
        std::cout << "\nThe password could not be cracked" << std::endl;
        break;
    }

    std::cout << "Mjaomjao " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds" << std::endl;
}

ZelmaCracker::ZelmaCracker(std::string bfi, int searchSize)
{   
    init(bfi, searchSize);

    if(users.size() != 0)
    {   
        std::cout << "\nPrrrbt?? =^.^=\n";
        auto start = std::chrono::high_resolution_clock::now();

        for(auto user : users)
        {
            hashed = user.getPassword();
            salt = user.getSalt();

            if(hashed.length() == 32) this->method = USE_MD5;
            else if(hashed.length() == 64) this->method = USE_SHA256;

            ZelmaMessage message = crackPassword();
            switch (message)
            {
            case CRACKED:
                std::cout << "The password has been cracked: " + cracked << std::endl;
                addToList();
                continue;;
            
            case NOT_CRACKED:
                std::cout << "The password could not be cracked" << std::endl;
                continue;;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Mjaomjao " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds" << std::endl;
    }

    else std::cout << "No users in file" << std::endl;
}

void ZelmaCracker::init(std::string bfi, int searchSize)
{
    bfl[0] = AS_IS;
    if(bfi.find('2') != std::string::npos) bfl[1] = TRY_NUMBERS;
    if(bfi.find('3') != std::string::npos) bfl[2] = TRY_ADDONS;
    if(bfi.find('4') != std::string::npos) bfl[3] = TRY_LEETSPEAK;

    switch(searchSize)
    {
        case 1:
            filePath = "data/knownPasswords/100.txt";
            break;

        case 2:
            filePath = "data/knownPasswords/1000.txt";
            break;

        case 3:
            filePath =  "data/knownPasswords/10000.txt";
            break;

        case 4:
            filePath =  "data/knownPasswords/100000.txt";
            break;

        case 5:
            filePath =  "data/knownPasswords/1000000.txt";
            break;

        default:
            filePath = "data/knownPasswords/1000.txt";
            break;
    }

    loadCommonAddons();
    loadLeetSpeak();
    loadFile();
    loadUsers();
}

ZelmaMessage ZelmaCracker::crackPassword() {   
    std::vector<std::future<ZelmaMessage>> futures;

    // Amount of threads we can run on our hardware
    size_t numThreads = std::thread::hardware_concurrency();
    size_t chunkSize = commonPasswords.size() / numThreads;

    auto it = commonPasswords.begin();

    // create parallell threads that try the different combinations
    for (size_t i = 0; i < numThreads; ++i) {
        auto start = it;
        std::advance(it, chunkSize);

        futures.push_back(std::async(std::launch::async, [this, start, it]() {
            for (auto passIt = start; passIt != it; ++passIt) {
                const std::string &password = *passIt;

                if (bfl[0] == AS_IS && checkPassword(password) == CRACKED) return CRACKED;
                if (bfl[1] == TRY_NUMBERS && checkNumbers(password) == CRACKED) return CRACKED;
                else if (bfl[2] == TRY_ADDONS && checkAddons(password) == CRACKED) return CRACKED;
                else if (bfl[3] == TRY_LEETSPEAK && checkLeet(password) == CRACKED) return CRACKED;
            }
            return NOT_CRACKED;
        }));
    }

    // wait for all threads
    for (auto& future : futures) {
        if (future.get() == CRACKED) return CRACKED;
    }

    return NOT_CRACKED;
}

ZelmaMessage ZelmaCracker::checkNumbers(const std::string &password)
{
    std::string temp;

    for (int i = 0; i < 100 ; i++) {
        temp = password;
        temp.append(std::to_string(i));
        if (checkPassword(temp) == CRACKED) return CRACKED;

        if (bfl[2] == TRY_ADDONS)
        {
            if (checkAddons(temp) == CRACKED) return CRACKED;
        }

        else if (bfl[3] == TRY_LEETSPEAK)
        {
            if (checkLeet(temp) == CRACKED) return CRACKED;
        }
    }

    return NOT_CRACKED;
}

ZelmaMessage ZelmaCracker::checkAddons(const std::string &password)
{
    std::string temp;
    for (const std::string& addon : commonAddons) {
        temp = password + addon;
        if (checkPassword(temp) == CRACKED) return CRACKED;

        if (bfl[3] == TRY_LEETSPEAK && checkLeet(temp) == CRACKED) return CRACKED;
    }

    return NOT_CRACKED;
}

ZelmaMessage ZelmaCracker::checkLeet(std::string password)
{   
    // Optimera leetspeak-ersättningar
    for (char& c : password) {
        if(!leetLetters.contains(std::toupper(c))) continue;

        auto it = leetSpeakDict.find(std::toupper(c));
        if (it != leetSpeakDict.end()) {
            for (const std::string& leet : it->second) {
                char original = c;
                c = leet[0];

                if (checkPassword(password) == CRACKED) return CRACKED;

                c = original;
            }
        }
    }

    return NOT_CRACKED;
}

void ZelmaCracker::loadFile()
{   
    std::ifstream file(filePath);

    if(!file.is_open())
    {
        throw std::invalid_argument("No file was found: " + filePath);
    }

    std::string temp;

    while(std::getline(file, temp))
    {
        size_t pos = temp.find(' ');
        
        std::string pass = temp.substr(0, pos);

        commonPasswords.insert(pass);
    }

    file.close();

    std::cout << "- Password file loaded\n";
}

ZelmaMessage ZelmaCracker::checkPassword(std::string password) {
    std::string hashedPassword = (method == USE_MD5) ? makeMD5(password + salt) : makeSHA256(password + salt);

    if (hashedPassword == hashed) {
        cracked = password;
        return CRACKED;
    }

    if (std::islower(password[0])) {
        password[0] = std::toupper(password[0]);
        hashedPassword = (method == USE_MD5) ? makeMD5(password + salt) : makeSHA256(password + salt);
        if (hashedPassword == hashed) {
            cracked = password;
            return CRACKED;
        }
    }
    
    return NOT_CRACKED;
}


void ZelmaCracker::loadCommonAddons()
{
    std::ifstream file("data/patterns/commonAddons.txt");

    if(!file.is_open())
    {
        throw std::invalid_argument("Could not open file! [loadCommonAddons()]");
    }

    std::string temp;
    while(std::getline(file, temp))
    {
        commonAddons.push_back(temp);
    }

    file.close();
    std::cout << "- Addons file loaded\n";
}

void ZelmaCracker::loadLeetSpeak() {
    std::ifstream file("data/patterns/leetSpeak.txt");
    if (!file.is_open()) throw std::invalid_argument("Could not open leet file");

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char key;
        std::string valuesStr;
        iss >> key;
        std::getline(iss, valuesStr);

        std::vector<std::string> values;
        std::istringstream valueStream(valuesStr);
        std::string value;

        while (std::getline(valueStream, value, ',')) {
            values.push_back(value);
        }

        leetSpeakDict[std::toupper(key)] = values;
        leetLetters.insert(std::toupper(key));
    }

    std::cout << "- Leetspeak file loaded\n";
}

int ZelmaCracker::addToList()
{
    std::ofstream file("data/crackedUsers/passwords.txt", std::ios::app);

    if(!file.is_open()) throw std::invalid_argument("Could not open file! [addToList()]");

    file << this->cracked + " " + this->salt + " " + this->hashed << std::endl;
    
    file.close();

    return 0;
}

void ZelmaCracker::loadUsers()
{
    std::ifstream file("data/users/users.txt");

    if(!file.is_open()) throw std::invalid_argument("Could not open file! [loadUsers()]");
    
    std::string line;
    while(std::getline(file, line))
    {
        std::istringstream stream(line);
        std::string userName, password, salt;

        stream >> userName >> password >> salt;

        users.emplace_back(userName, password, salt);
    }

    file.close();
}