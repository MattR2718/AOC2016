#include <iostream>
#include <string>
#include <ranges>
#include <numeric>
#include <vector>
#include <algorithm>
#include <unordered_map>


#include <ctre.hpp>


#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/md5.h>


std::string md5(const std::string& input) {
    CryptoPP::MD5 hash;
    std::string digest;

    CryptoPP::StringSource ss(input, true,
        new CryptoPP::HashFilter(hash,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest), false)));
    return digest;
}


// std::string md5(const std::string& input) {
//     CryptoPP::MD5 hash;
//     byte digest[CryptoPP::MD5::DIGESTSIZE];
//     hash.CalculateDigest(digest, (const byte*)input.data(), input.size());

//     std::string output;
//     output.resize(CryptoPP::MD5::DIGESTSIZE * 2);

//     CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(output), false);
//     encoder.Put(digest, sizeof(digest));
//     encoder.MessageEnd();

//     return output;
// }

int main() {
    std::string linetxt;

    
    std::string p1 = "", p2 = "........";
    int p2c = 0;

    std::getline(std::cin, linetxt);

    char numbuf[32];
    auto make_hash_input = [&](int index) {
        auto len = sprintf(numbuf, "%d", index);
        return linetxt + std::string(numbuf, len);
    };
    
    int i = -1;
    std::string hash = "";
    do{
      i++;
      hash = md5(make_hash_input(i));  
      if(ctre::match<"00000[a-z0-9]+">(hash)) {
        if(p1.length() < 8){
          p1 += hash[5];
          std::cout << "i: " << i << " hash: " << hash << " P1: " << p1 << "\n";
        }

        if(p2[hash[5] - '0'] == '.'){
          p2[hash[5] - '0'] = hash[6];
          p2c++;
          std::cout << "i: " << i << " hash: " << hash << " P2: " << p2 << " P2C: " << p2c << "\n";
        }
        
        
      }

      if(i % 1000000 == 0){ std::cout<<i<<'\n'; }

      //std::cout<<"i: " << i << " hash: " << hash << " P1: " << p1 << "\n";
    } while(p1.length() < 8 || p2c < 8);

    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
}


/*

Part 1: 4543c154
Part 2: 1050cbbd

real	1m21.623s
user	1m21.007s
sys	0m0.017s



*/