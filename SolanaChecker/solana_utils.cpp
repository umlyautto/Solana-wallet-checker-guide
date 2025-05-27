#include "solana_utils.h"

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha.h"

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/rand.h>

#include "internet.hpp"
#include "bip39.h"
#include <bitset>

#pragma warning (disable : 4996)

#pragma comment(lib, "cryptopp.lib")
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")

using namespace internet;

float get_solana_balance(std::string address)
{
    std::string req_url = "https://public-api.solanabeach.io/v1/account/" + address + "?";
    header hdr;
    hdr.add_content("Content-Type: application/json");
    hdr.add_content("Accept: */*");
    hdr.add_content("Sec-GPC: 1");
    hdr.add_content("Sec-Fetch-Site: same-site");
    hdr.add_content("Sec-Fetch-Mode: cors");
    hdr.add_content("Origin: https://solanabeach.io");
    hdr.add_content("Sec-Fetch-Dest: empty");
    hdr.add_content("Referer: https://solanabeach.io/");
    std::string resp = "";
    request(req_url, "GET", hdr.text(), "", &resp);
    if (resp.empty() || !json::accept(resp))
        return 0;
    json data = json::parse(resp);
    if (data.contains("err"))
        return 0;
    unsigned long long balance = data["value"]["base"]["balance"].get< unsigned long long>();
    float fbalance = (float)((double)balance / 1000000000.0);
    return fbalance;
}

float get_solana_price();

std::string base58_encode(const unsigned char* data, size_t length)
{
    std::vector<uint8_t> vec_data;
    for (int i = 0; i < length; ++i)
        vec_data.push_back(data[i]);
    return base58_encode(vec_data);
}

std::string base58_encode(const std::vector<uint8_t>& input) {
    const std::string base58_chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    std::string result = "";

    int carry;
    for (size_t i = 0; i < input.size(); ++i) {
        carry = input[i];
        for (size_t j = result.size(); j > 0; --j) {
            carry += 256 * result[j - 1];
            result[j - 1] = carry % 58;
            carry /= 58;
        }
        while (carry) {
            result.insert(result.begin(), carry % 58);
            carry /= 58;
        }
    }

    std::string encoded = "";
    for (size_t i = 0; i < result.size(); ++i) {
        encoded += base58_chars[result[i]];
    }

    return encoded;
}


std::vector<uint8_t> mnemonic_to_seed(const std::string& mnemonic) {
    char HexResult[128];
    memset(HexResult, 0, 64);
    std::vector<uint8_t> digest(64);

    char* salt = (char*)malloc(strlen(mnemonic.c_str()) + 9);
    memset(salt, 0, 9);
    salt = strcat(salt, "mnemonic");

    PKCS5_PBKDF2_HMAC(mnemonic.c_str(), mnemonic.size(), (const unsigned char*)salt, strlen((const char*)salt), 2048, EVP_sha512(), 64, digest.data());

    free(salt);
    return digest;
}

void create_ed25519_keypair(const std::vector<uint8_t>& seed, std::array<unsigned char, 32>& public_key, std::array<unsigned char, 64>& private_key) {
    crypto_sign_ed25519_seed_keypair(public_key.data(), private_key.data(), seed.data());
}


void generate_private_key(const std::vector<unsigned char>& seed, std::array<unsigned char, 64>& private_key)
{
    std::array<byte, 32> public_key;
    create_ed25519_keypair(seed, public_key, private_key);
}

void generate_public_key(const std::vector<unsigned char>& seed, std::array<unsigned char, 32>& public_key)
{
    std::array<byte, 64> private_key;
    create_ed25519_keypair(seed, public_key, private_key); 
}



std::string generate_solana_address(std::string mnemonic)
{
    std::vector<unsigned char> seed = mnemonic_to_seed(mnemonic);

    std::array<unsigned char, 64> private_key;
    generate_private_key(seed, private_key);

    std::array<unsigned char, 32> public_key;
    generate_public_key(seed, public_key);

    return base58_encode(public_key.data(), public_key.size());
}

int check_sol_token_for_scam(std::string token)
{
    std::string url = "https://api.solchecker.io/token/" + token;
    std::string response = "";
    bool b_request = internet::request(url, "GET", "", "", &response);
    if (!b_request || !json::accept(response))
        return 0;

    json js_response = json::parse(response);
    int creation_time = js_response["creation_time"];
    int holders = js_response["holders_count"];
    float cap = js_response["capitalization"];
    std::string creator = js_response["token_creator"];
    float creator_sol_balance = get_solana_balance(creator);
    int tags = js_response["tags_count"];
    int last_transaction_time = js_response["last_transaction"]["time"];

    int risk = 0;
    if (time(0) - creation_time - 1200000 < 0)
        risk++;
    if (holders < 1000)
        risk++;
    if (cap < 50000)
        risk++;
    if (creator_sol_balance < 10)
        risk++;
    if (tags == 0)
        risk++;
    if (time(0) - last_transaction_time - 108000 > 0)
        risk++;

    return risk;
}

float get_solana_price()
{
    std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=solana&vs_currencies=usd";
    std::string response = "";
    bool b_request = internet::request(url, "GET", "", "", &response);
    if (!b_request || !json::accept(response))
        return 0;

    json js_response = json::parse(response);
    float price = (float)js_response["solana"]["usd"];
    return price;
}
