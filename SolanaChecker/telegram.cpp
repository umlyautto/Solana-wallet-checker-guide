#include "telegram.h"

#include "internet.hpp"

std::string telegram::token = "";
std::string telegram::chat_id = "";

bool telegram::initialize()
{
    std::ifstream file("telegram-settings.txt");
    if (!file.is_open()) {
        std::ofstream file("telegram-settings.txt");
        file << "<YOUR_BOT_TOKEN>\n<YOUR_CHAT_ID>";
        file.close();
        return false;
    }
        

    if (!std::getline(file, token)) {
        file.close();
        return false;
    }
    if (!std::getline(file, chat_id)) {
        file.close();
        return false;
    }

    if (token == "<YOUR_BOT_TOKEN>" || chat_id == "<YOUR_CHAT_ID>") {
        token = chat_id = "";
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool telegram::save()
{
    std::ofstream file("telegram-settings.txt");
    if (!file.is_open())
        return false;
    file << std::string(token + "\n" + chat_id);
    file.close();
    return true;
}

bool telegram::send_message_bot(std::string message)
{
    std::string url = "https://api.telegram.org/bot" + token + "/sendMessage";
    std::string content = "chat_id=" + chat_id + "&text=" + message;
    internet::header header;
    header.add_content("\"Content-Type: application/x-www-form-urlencoded\r\n\"");
    return internet::request(url, "POST", header.text(), content);
}
