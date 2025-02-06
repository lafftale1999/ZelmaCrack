#ifndef ZELMA_HASHER_H
#define ZELMA_HASHER_H

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <iostream>

typedef enum
{
    USE_MD5,
    USE_SHA256
} HashMethod;

std::string makeMD5(const std::string &password);
std::string makeSHA256(const std::string &password);

#endif