#pragma once



#include <sodium.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <array>
#include <string>
#include <vector>
#include <sstream>


float get_solana_balance(std::string address);
float get_solana_price();

std::string base58_encode(const unsigned char* data, size_t length);
std::string base58_encode(const std::vector<uint8_t>& input);

std::vector<unsigned char> mnemonic_to_seed(const std::string& mnemonic);

void generate_private_key(const std::vector<unsigned char>& seed, std::array<unsigned char, 64>& private_key);
void generate_public_key(const std::vector<unsigned char>& seed, std::array<unsigned char, 32>& public_key);
std::string generate_solana_address(std::string mnemonic);
int check_sol_token_for_scam(std::string token);

void test123();