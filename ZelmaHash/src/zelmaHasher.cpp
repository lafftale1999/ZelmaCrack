#include "../include/zelmaHasher.h"
#include <iostream>
#include <iomanip>
#include <sstream>

std::string makeSHA256(const std::string &password){
  unsigned char hash[SHA256_DIGEST_LENGTH];

  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, password.c_str(), password.size());
  SHA256_Final(hash, &sha256);

  std::stringstream ss;

  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>( hash[i] );
  }
  return ss.str();
}

std::string makeMD5(const std::string& str) {
    unsigned char hash[MD5_DIGEST_LENGTH];

    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, str.c_str(), str.size());
    MD5_Final(hash, &md5);

    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    
    return ss.str();
}