#pragma once

#include <ostream>

#ifdef HAVE_OPENSSL
  #include <openssl/md5.h>
#else
  #include <cryptopp/hex.h>
  #include <cryptopp/filters.h>
  #include <cryptopp/md5.h>
#endif // HAVE_OPENSSL

namespace MD5Hash{

    constexpr int MD5_HASH_LENGTH = 16;

    struct Hash {
        uint64_t _p1{0}, _p2{0};

        Hash() = default;

        char get_hex_char(int pos) const {
            int c;
            if (pos < 16) {
                c = (_p1 >> ((15 - pos) * 4)) & 0xf;
            } else {
                c = (_p2 >> ((31 - pos) * 4)) & 0xf;
            }
            return (c < 10) ? ('0' + c) : ('a' + (c - 10));
        }

        uint8_t get_hex_value(int pos) const {
            if (pos < 16) {
                return (_p1 >> ((15 - pos) * 4)) & 0xf;
            } else {
                return (_p2 >> ((31 - pos) * 4)) & 0xf;
            }
        }

        void set_hash(const uint8_t* hash_str) {
            for (int i = 0; i < 8; i++) {
                _p1 |= static_cast<uint64_t>(hash_str[i])     << (56 - i * 8);
                _p2 |= static_cast<uint64_t>(hash_str[i + 8]) << (56 - i * 8);
            }
        }

        Hash(const uint8_t* hash_str) {
            set_hash(hash_str);
        }

        std::string to_string() const {
            static constexpr char hexmap[] = "0123456789abcdef";
            std::string s(32, '\0');

            uint64_t parts[2] = {_p1, _p2};
            for (int j = 0; j < 2; ++j) {
                uint64_t v = parts[j];
                for (int i = 0; i < 8; ++i) {
                    uint8_t byte = (v >> (56 - i * 8)) & 0xFF;
                    s[j * 16 + i * 2]     = hexmap[byte >> 4];
                    s[j * 16 + i * 2 + 1] = hexmap[byte & 0x0F];
                }
            }
            return s;
        }


        friend std::ostream& operator<< (std::ostream& stream, const Hash& h){
            stream << h.to_string();
            return stream;
        }
        
    };

    #ifdef HAVE_OPENSSL
    Hash md5(const std::string& input) {
        unsigned char digest[MD5_DIGEST_LENGTH]; // MD5_DIGEST_LENGTH = 16
        MD5(reinterpret_cast<const unsigned char*>(input.data()), input.size(), digest);

        return Hash(digest);
    }

    #else

    Hash md5(const std::string& input) {
        CryptoPP::MD5 hash;
        CryptoPP::byte digest[CryptoPP::MD5::DIGESTSIZE]; // 16 bytes

        // Calculate MD5 hash
        hash.CalculateDigest(digest, reinterpret_cast<const CryptoPP::byte*>(input.data()), input.size());

        // Convert the first 4 bytes of digest into a uint32_t (big-endian like your OpenSSL code)
        return Hash(digest);
    }

    #endif // HAVE_OPENSSL
};