#pragma once


#include <string>
#include <vector>
#include <random>
#include <fstream>

static class bip39
{
public:
	static std::vector<std::string> bip39_words;

	static bool initialize();

	static std::string generate_mnemonic(int length);


};

