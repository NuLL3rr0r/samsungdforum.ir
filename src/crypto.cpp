#include <cryptopp/aes.h>
#include <cryptopp/ccm.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include "crypto.hpp"
#include "log.hpp"

using namespace std;
using namespace CryptoPP;
using namespace SamsungDForumIr;

const string Crypto::UNKNOWN_ERROR = "** Unknown Error!!";

unsigned char Crypto::m_key[] = { 0x53, 0x34, 0x6d, 0x53, 0x55, 0x4e, 0x47, 0x37, 0x2d, 0x76, 0x44, 0x33, 0x76, 0x4a, 0x2e, 0x3f };
unsigned char Crypto::m_iv[] = { 0x23, 0x40, 0x44, 0x33, 0x56, 0x66, 0x30, 0x52, 0x75, 0x4d, 0x73, 0x4d, 0x34, 0x52, 0x37, 0x71 };

const string Crypto::Encrypt(const string &plainText)
{
   string cipher, encoded;

   try {
        CBC_Mode<AES>::Encryption enc;
        enc.SetKeyWithIV(m_key, sizeof(m_key), m_iv, sizeof(m_iv));

        cipher.clear();
        StringSource(plainText, true, new StreamTransformationFilter(enc, new StringSink(cipher)));

        encoded.clear();
        StringSource(cipher, true, new HexEncoder(new StringSink(encoded)));
    }
    catch(CryptoPP::Exception &ex) {
        ERR("Encryption", ex.what(), plainText);
    }
    catch (...) {
        ERR("Encryption", UNKNOWN_ERROR, plainText);
    }

    return encoded;
}

const string Crypto::Decrypt(const string &cipherText)
{
    string cipher, recovered;

    try {
        CBC_Mode<AES>::Decryption dec;
        dec.SetKeyWithIV(m_key, sizeof(m_key), m_iv, sizeof(m_iv));

        cipher.clear();
        StringSource(cipherText, true, new HexDecoder(new StringSink(cipher)));

        recovered.clear();
        StringSource s(cipher, true, new StreamTransformationFilter(dec, new StringSink(recovered)));
    }
    catch(CryptoPP::Exception &ex) {
        ERR("Decryption", ex.what(), cipherText);
    }
    catch (...) {
        ERR("Decryption", UNKNOWN_ERROR, cipherText);
    }

    return recovered;
}

const string Crypto::GenHash(const string &text)
{
    string digest;
    SHA1 hash;

    StringSource(text, true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));

    return digest;
}


