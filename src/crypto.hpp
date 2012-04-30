#ifndef CRYPTO_HPP
#define CRYPTO_HPP


#include <string>

namespace SamsungDForumIr {
    class Crypto;
}

class SamsungDForumIr::Crypto
{
private:
	static const std::string UNKNOWN_ERROR;

    static unsigned char m_key[];
    static unsigned char m_iv[];

public:
    static const std::string Encrypt(const std::string &plainText);
    static const std::string Decrypt(const std::string &cipherText);
    static const std::string GenHash(const std::string &text);
};


#endif /* CRYPTO_HPP */


