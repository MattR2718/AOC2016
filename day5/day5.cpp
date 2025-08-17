#include <iostream>
#include <string>
#include <ranges>
#include <numeric>
#include <algorithm>
#include <unordered_map>

#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <cstring>

#include <ctre.hpp>

#ifdef HAVE_OPENSSL
  #include <openssl/md5.h>
#else
  #include <cryptopp/hex.h>
  #include <cryptopp/filters.h>
  #include <cryptopp/md5.h>
#endif // HAVE_OPENSSL


#include <sstream>
#include <iomanip>

#include <timer.h>

#ifdef HAVE_OPENSSL
uint32_t md5(const std::string& input) {
    unsigned char digest[MD5_DIGEST_LENGTH]; // MD5_DIGEST_LENGTH = 16
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), digest);

    return (static_cast<uint32_t>(digest[0]) << 24) |
                    (static_cast<uint32_t>(digest[1]) << 16) |
                    (static_cast<uint32_t>(digest[2]) << 8)  |
                    (static_cast<uint32_t>(digest[3]));
}

#else

uint32_t md5(const std::string& input) {
    CryptoPP::MD5 hash;
    CryptoPP::byte digest[CryptoPP::MD5::DIGESTSIZE]; // 16 bytes

    // Calculate MD5 hash
    hash.CalculateDigest(digest, reinterpret_cast<const CryptoPP::byte*>(input.data()), input.size());

    // Convert the first 4 bytes of digest into a uint32_t (big-endian like your OpenSSL code)
    return (static_cast<uint32_t>(digest[0]) << 24) |
           (static_cast<uint32_t>(digest[1]) << 16) |
           (static_cast<uint32_t>(digest[2]) << 8)  |
           (static_cast<uint32_t>(digest[3]));
}

#endif // HAVE_OPENSSL

// int to hex string
std::string to_hex_string(uint32_t value) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0') << std::setw(8) << value;
    return oss.str();
}

struct POS {
    uint8_t value;
    int32_t index;
};

struct ANS{
    POS ans[8];
};

int main() {
    Timer::ScopedTimer t("Day 5");

    std::string linetxt;

    
    std::string p1 = "", p2 = "........";
    int p2c = 0;

    std::getline(std::cin, linetxt);

    
    auto make_hash_input = [&](int index) {
        char numbuf[32];
        auto len = sprintf(numbuf, "%d", index);
        return linetxt + std::string(numbuf, len);
    };
    
    static unsigned int num_threads = std::thread::hardware_concurrency();
    //static unsigned int num_threads = 1;
    std::cout<<"Running on " << num_threads << " threads\n";
    constexpr int max_index   = INT_MAX;

    ANS p1_ans, p2_ans;
    // Initialise output values
    for(int i = 0; i < 8; i++){
        p1_ans.ans[i].index = -1;
        p2_ans.ans[i].index = -1;
        p1_ans.ans[i].value = 0;
        p2_ans.ans[i].value = 0;
    }


    int offset = 0;
    int batch_size = 1000000;

    std::mutex mut;

    auto worker = [&](int thread_id) {
        for (int i = thread_id + offset; i < offset + batch_size; i += num_threads) {
            uint32_t hash = md5(make_hash_input(i));

            if (!(hash & 0xfffff000)) {
                int six = (hash >> 8);
                int seven = (hash >> 4) & 0xf;

                std::lock_guard<std::mutex> lock(mut); // Lock here

                for (int j = 0; j < 8; j++) {
                    if (p1_ans.ans[j].index == -1) {
                        p1_ans.ans[j].index = i;
                        p1_ans.ans[j].value = six;
                        break;
                    } else if (p1_ans.ans[j].index > i) {
                        POS temp1 = POS{(uint8_t)six, static_cast<int32_t>(i)};
                        POS temp2 = p1_ans.ans[j];
                        do {
                            p1_ans.ans[j] = temp1;
                            temp1 = temp2;
                            temp2 = p1_ans.ans[j + 1 < 8 ? j + 1 : 7];
                            j++;
                        } while (j < 8);
                        break;
                    }
                }

                // Update p2_ans safely
                if (six < 8 && (p2_ans.ans[six].index == -1 || p2_ans.ans[six].index > i)) {
                    p2_ans.ans[six].index = i;
                    p2_ans.ans[six].value = seven;
                }
            }
        }
    };


    std::vector<std::jthread> threads;
    threads.reserve(num_threads);

    bool done = false;
    do{
        for (int t = 0; t < num_threads; ++t) {
            threads.emplace_back(worker, t);
        }

        for (auto &thread : threads) {
            thread.join();
        }

        threads.clear();

        // Check if answers have been found
        done = true;
        for(int i = 0; i < 8; i++){
            if(p1_ans.ans[i].index == -1 || p2_ans.ans[i].index == -1){
                done = false;
                break;
            }
        }
        // Read out p1 answer
        std::string p1_str = "";
        for(int i = 0; i < 8; i++){
            p1_str += p1_ans.ans[i].value < 10 ? std::to_string(p1_ans.ans[i].value) : std::string(1, 'a' + (p1_ans.ans[i].value - 10));
        }

        // Read out p2 answer
        std::string p2_str = "";
        for(int i = 0; i < 8; i++){
            p2_str += p2_ans.ans[i].value < 10 ? std::to_string(p2_ans.ans[i].value) : std::string(1, 'a' + (p2_ans.ans[i].value - 10));
        }

        offset += batch_size;

    } while (!done);

  

    // Read out p1 answer
    std::string p1_str = "";
    for(int i = 0; i < 8; i++){
        p1_str += p1_ans.ans[i].value < 10 ? std::to_string(p1_ans.ans[i].value) : std::string(1, 'a' + (p1_ans.ans[i].value - 10));
    }

    // Read out p2 answer
    std::string p2_str = "";
    for(int i = 0; i < 8; i++){
        p2_str += p2_ans.ans[i].value < 10 ? std::to_string(p2_ans.ans[i].value) : std::string(1, 'a' + (p2_ans.ans[i].value - 10));
    }

    std::cout << "Part 1: " << p1_str << "\n";
    std::cout << "Part 2: " << p2_str << "\n";
}