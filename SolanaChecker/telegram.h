#pragma once

#include <string>
#include <vector>
#include <fstream>


static class telegram
{
public:
	static std::string token;
	static std::string chat_id;

	static bool initialize();
	static bool save();
	static bool send_message_bot(std::string message);

};

