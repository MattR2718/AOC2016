#include <iostream>
#include <string>
#include <ranges>
#include <numeric>
#include <algorithm>
#include <unordered_map>

#include <thread>
#include <vector>
#include <atomic>

#include <ctre.hpp>


#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/md5.h>


// std::string md5(const std::string& input) {
//     CryptoPP::MD5 hash;
//     std::string digest;

//     CryptoPP::StringSource ss(input, true,
//         new CryptoPP::HashFilter(hash,
//             new CryptoPP::HexEncoder(
//                 new CryptoPP::StringSink(digest), false)));
//     return digest;
// }


std::string md5(const std::string& input) {
    CryptoPP::MD5 hash;
    CryptoPP::byte digest[CryptoPP::MD5::DIGESTSIZE];
    
    hash.CalculateDigest(digest, reinterpret_cast<const CryptoPP::byte*>(input.data()), input.size());

    std::string output;
    output.reserve(CryptoPP::MD5::DIGESTSIZE * 2); // hex string is double the digest size

    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(output), false);
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

    return output;
}


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

    constexpr int num_threads = 12;
    constexpr int max_index   = INT_MAX;

    std::atomic<int> p2c_a = 0;
    std::mutex p2_mutex;

    std::vector<std::pair<int, char>> p1_pairs;
    std::mutex p1_mutex;
    std::atomic<int> p1c_a = 0;

    auto worker = [&](int thread_id) {
      std::string hash = "";
      for (int i = thread_id; p2c_a < 8 && i < max_index; i += num_threads) {
        // Do work
        hash = md5(make_hash_input(i));  
        if(ctre::match<"00000[a-z0-9]+">(hash)) {
          if(p1c_a < 8){
            std::lock_guard<std::mutex> lock(p1_mutex);
            p1_pairs.emplace_back(i, hash[5]);
            p1c_a++;
          }

          std::unique_lock<std::mutex> lock(p2_mutex);
          if(p2[hash[5] - '0'] == '.'){
            p2[hash[5] - '0'] = hash[6];
            p2c_a++;
            std::cout << "i: " << i << " hash: " << hash << " P2: " << p2 << " P2C: " << p2c_a << "\n";
          }
          lock.unlock();
          
          
          
        }
      }
    };

    std::vector<std::jthread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back(worker, t);
    }

    for (auto &thread : threads) {
        thread.join();
    }

  

    std::ranges::sort(p1_pairs, [](const auto &a, const auto &b) {
        return a.first < b.first; // Sort by index
    });

    for (const auto &[index, value] : p1_pairs) {
        p1 += value;
    }

    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
}
