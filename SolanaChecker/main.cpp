#include <iostream>
#include <Windows.h>
#include <thread>

#include "bip39.h"
#include "solana_utils.h"
#include "telegram.h"

float sol_price = 0;

void set_cmd_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void print_logo() {
    set_cmd_color(FOREGROUND_RED | FOREGROUND_BLUE);
    printf("***************************************************************************\n");
    printf("*   _____       _                    _____ _               _              *\n");
    printf("*  / ____|     | |                  / ____| |             | |             *\n");
    printf("* | (___   ___ | | __ _ _ __   __ _| |    | |__   ___  ___| | _____ _ __  *\n");
    printf("*  \\___ \\ / _ \\| |/ _` | '_ \\ / _` | |    | '_ \\ / _ \\/ __| |/ / _ \\ '__| *\n");
    printf("*  ____) | (_) | | (_| | | | | (_| | |____| | | |  __/ (__|   <  __/ |    *\n");
    printf("* |_____/ \\___/|_|\\__,_|_| |_|\\__,_|\\_____|_| |_|\\___|\\___|_|\_\\___|_|     *\n");
    printf("*                                                                         *\n");
    printf("***************************************************************************\n");
    set_cmd_color(7);
}


int show_menu() {
point1:
    set_cmd_color(FOREGROUND_RED | FOREGROUND_BLUE);
    printf("===========================================================================\n");
    set_cmd_color(7);

    set_cmd_color(2);
    printf("[1]");
    set_cmd_color(7);
    printf(" - check SOL address balance\n");

    set_cmd_color(2);
    printf("[2]");
    set_cmd_color(7);
    printf(" - check SOL token for scam\n");

    set_cmd_color(2);
    printf("[3]");
    set_cmd_color(7);
    printf(" - tracking solana address\n");

    set_cmd_color(2);
    printf("[4]");
    set_cmd_color(7);
    printf(" - wallet data from mnemonic\n");

    set_cmd_color(2);
    printf("[5]");
    set_cmd_color(7);
    printf(" - generate one SOL wallet\n");

    set_cmd_color(2);
    printf("[6]");
    set_cmd_color(7);
    printf(" - start searching for SOL wallets with balance\n");

    set_cmd_color(2);
    printf("[7]");
    set_cmd_color(7);
    printf(" - support the developer\n");

    set_cmd_color(2);
    printf("[8]");
    set_cmd_color(7);
    printf(" - exit\n");

    set_cmd_color(FOREGROUND_RED | FOREGROUND_BLUE);
    printf("===========================================================================\n");
    set_cmd_color(7);

    std::cout << "select an action using the key: ";

    int action;
    std::cin >> action;
    return action;
}

void generate_one_wallet();
void mnemonic_info(std::string mnemonic);
void wallet_tracker(std::string address);
void start_search_sol_wallets();

int main(int argc, char** argv)
{
    bip39::initialize();
    telegram::initialize();

    if (argc > 1) {
        if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "-seach") == 0) {
            start_search_sol_wallets();
        }
        else if (strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "-track") == 0) {
            if (telegram::token.empty() || telegram::chat_id.empty()) {
                printf("[-] error. fill in the data in the 'telegram-settings.txt' file. [-]");
                return 0;
            }
            if (argc > 2) {
                std::string address(argv[2]);
                printf("[+] wallet tracker started [+]\n");
                wallet_tracker(address);
            }
            else {
                printf("[-] invalid parameters [-]\nUse: -track <ADDRESS>\n");
                return 0;
            }
        }
        else if (strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "-gen")) {
            int n = 1;
            if (argc > 2)
                n = std::atoi(argv[2]);
            for (int i = 0; i < n; ++i)
                generate_one_wallet();
            return 0;
        }
        else if (strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "-mnemonic") == 0) {
            if (argc > 2) {
                std::string mnemonic(argv[2]);
                mnemonic_info(mnemonic);
            }
            else {
                printf("[-] invalid parameters [-]\nUse: -mnemonic <MNEMONIC>\n");
            }
            return 0;
        }
        else if (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-balance") == 0) {
            if (argc > 2) {
                std::string address(argv[2]);
                float balance = get_solana_balance(address);

                set_cmd_color(2);
                printf("\n\n===========================================================================\n");
                printf("[check balance]\n");
                set_cmd_color(7);
                printf("%s  | %f SOL ($%f)\n", address.c_str(), balance, balance * sol_price);
                set_cmd_color(2);
                printf("===========================================================================\n\n");
                set_cmd_color(7);
            }
            else {
                printf("[-] invalid parameters [-]\nUse: -balance <ADDRESS>\n");
            }
            return 0;
        }


    }


    print_logo();

    sol_price = get_solana_price();
    std::string title = "[SolanaChecker] - Menu | SOL price: $" + std::to_string(sol_price);
    SetConsoleTitleA(title.c_str());

    while (true) {
        int option = show_menu();
        printf("\n");

        if (option == 1) {
            printf("enter the wallet address: ");
            std::string address;
            std::cin >> address;

            float balance = get_solana_balance(address);

            set_cmd_color(2);
            printf("\n\n===========================================================================\n");
            printf("[check balance]\n");
            set_cmd_color(7);
            printf("%s  | %f SOL ($%f)\n", address.c_str(), balance, balance * sol_price);
            set_cmd_color(2);
            printf("===========================================================================\n\n");
            set_cmd_color(7);
        }
        else if (option == 2) {

            printf("enter the token address: ");
            std::string token_address;
            std::cin >> token_address;

            int risk = check_sol_token_for_scam(token_address);
            printf("\n\n===========================================================================\n");
            printf("[check token]\n");
            printf("Token: [%s] -- scam risk: ", token_address.c_str());

            if (risk < 2)
                set_cmd_color(2);
            else if (risk <= 5)
                set_cmd_color(6);
            else
                set_cmd_color(4);

            printf("(%i / 7)\n", risk);
            set_cmd_color(7);
            printf("===========================================================================\n\n");

        }
        else if (option == 3) {
            if (telegram::token == "<YOUR BOT TOKEN>" || telegram::chat_id == "<YOUR CHAT_ID>") {
                printf("please enter your Telegram bot token: ");
                std::cin >> telegram::token;
                printf("enter your chat id: ");
                std::cin >> telegram::chat_id;
                printf("your Telegram settings saved");
            }

            printf("enter the target address: ");

            if (telegram::token.empty() || telegram::chat_id.empty()) {
                printf("[-] error. fill in the data in the 'telegram-settings.txt' file. [-]");
                continue;
            }

            std::string address;
            std::cin >> address;

            std::thread* thread = new std::thread(wallet_tracker, address);
            set_cmd_color(2);
            printf("[+} tracking wallet address thread started [+]");
            set_cmd_color(7);

        }
        else if (option == 4) {
            printf("enter mnemonic: ");
            char buffer[256];
            memset(buffer, 0, 256);
            std::cin.ignore();
            std::cin.getline(buffer, 256);
            std::string mnemonic(buffer);
     
            mnemonic_info(mnemonic);
        }

        else if (option == 5) {
            generate_one_wallet();
        }
        else if (option == 6) {
            start_search_sol_wallets();
        }
        else if (option == 7) {
           printf("---------------------------------------------------------------------------------\n");
           printf("| my crypto wallets:                                                            |\n");
           printf("| Solana:                        32vRA8jC1YgjC6g3RCZLPWmijvP3cP5bPWbFvmyKYK5o   |\n");
           printf("| Bitcoin:                       bc1q7knjmcr0q7hku9lhppyxlhsr6f67la8gh67hey     |\n");
           printf("| ETH                            0x446Bc3DF288bE2D54ea87C128C1E12CB12aD1e04     |\n");
           printf("| Litecoin:                      ltc1qy5dn25ftkn4tmgn4aysfl3nnkrr7vchmzx9dd4    |\n");
           printf("| USDT TRC20:                    TSQN7BXkVt3aBCVwcxSBCaLJLymoNGegkN             |\n");
           printf("---------------------------------------------------------------------------------\n");
        }
        else if (option == 8) {
            break;
        }
        else {
            printf("invalid option\n");
        }

        printf("\n\n");

    }


    return 0;
}


void generate_one_wallet() {
    std::string mnemonic = bip39::generate_mnemonic(12);
    std::vector<unsigned char> seed = mnemonic_to_seed(mnemonic);
    std::array<unsigned char, 64> private_key;
    generate_private_key(seed, private_key);
    std::string wallet_address = generate_solana_address(mnemonic);
    float wallet_balance = get_solana_balance(wallet_address);
    std::string private_key_encoded = base58_encode(private_key.data(), private_key.size());


    set_cmd_color(2);
    printf("\n\n===========================================================================\n");
    printf("[generate wallet]\n");
    set_cmd_color(7);
    printf("mnemonic: %s\nprivate:  %s\naddress:  %s\nbalance:  %f SOL ($%f)", mnemonic.c_str(), private_key_encoded.c_str(), wallet_address.c_str(), wallet_balance, (wallet_balance * sol_price));
    set_cmd_color(2);
    printf("\n===========================================================================\n\n");
    set_cmd_color(7);
}

void mnemonic_info(std::string mnemonic) {
    std::vector<unsigned char> seed = mnemonic_to_seed(mnemonic);
    std::array<unsigned char, 64> private_key;
    generate_private_key(seed, private_key);
    std::string wallet_address = generate_solana_address(mnemonic);
    float wallet_balance = get_solana_balance(wallet_address);
    std::string private_key_encoded = base58_encode((const byte*)private_key.data(), 64);



    set_cmd_color(2);
    printf("\n\n===========================================================================\n");
    printf("[check mnemonic]\n");
    set_cmd_color(7);
    printf("mnemonic: %s\nprivate:  %s\naddress:  %s\nbalance:  %f SOL ($%f)", mnemonic.c_str(), private_key_encoded.c_str(), wallet_address.c_str(), wallet_balance, (wallet_balance * sol_price));
    set_cmd_color(2);
    printf("\n===========================================================================\n\n");
    set_cmd_color(7);
}

void start_search_sol_wallets() {

    unsigned long long checked_mnemonics = 0;
    float found_balance = 0.0f;
    int speed = 0, speed_check = 0, mnemonics_by_sec = 0;

    while (true) {
        std::string mnemonic = bip39::generate_mnemonic(12);
        std::vector<unsigned char> seed = mnemonic_to_seed(mnemonic);
        std::array<unsigned char, 64> private_key;
        generate_private_key(seed, private_key);
        std::string wallet_address = generate_solana_address(mnemonic);
        float wallet_balance = get_solana_balance(wallet_address);
        std::string private_key_encoded = base58_encode(private_key.data(), 32);

      //  printf("mnemonic: %s\naddress: %s\nbalance: %f SOL\n", mnemonic.c_str(), wallet_address.c_str(), wallet_balance);
        set_cmd_color(2);
        printf("mnemonic: ");
        set_cmd_color(7);
        printf("%s\n", mnemonic.c_str());
        set_cmd_color(2);
        printf("private:  ");
        set_cmd_color(7);
        printf("%s\n", private_key_encoded.c_str());
        set_cmd_color(2);
        printf("address:  ");
        set_cmd_color(7);
        printf("%s\n", wallet_address.c_str());
        set_cmd_color(2);
        printf("balance:  ");
        set_cmd_color(FOREGROUND_RED | FOREGROUND_BLUE);
        printf("%f SOL\n");
        set_cmd_color(7);


        printf("===========================================================================\n");

        if (found_balance > 0) {
            std::string save_text = "mnemonic: " + mnemonic + "\nprivate key: " + private_key_encoded + 
                "\naddress: " + wallet_address + "\nbalance: " + std::to_string(wallet_balance) + "SOL\n";

            HANDLE hfile = CreateFileA("found-wallets.txt", FILE_ALL_ACCESS, NULL, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (!hfile) {
                std::cout << save_text << "\n";
                return;
            }

            if (!WriteFile(hfile, save_text.c_str(), save_text.size(), nullptr, nullptr)) {
                std::cout << save_text;
            }

            CloseHandle(hfile);
            found_balance += wallet_balance * sol_price;
            if (!telegram::token.empty() && !telegram::chat_id.empty()) {
                std::string telegram_msg = "[Found Solana Address With Balance]\nAddress: " + wallet_address + "\nbalance: " + std::to_string(wallet_balance) + " ($" + (std::to_string(wallet_balance * sol_price)) + ")"
                    + "\n mnemonic: || " + mnemonic + " ||";
            }
            
        }

        if (time(0) - speed_check > 1) {
            speed = mnemonics_by_sec;
            mnemonics_by_sec = 0;
            speed_check = time(0);
        }

        checked_mnemonics++;
        mnemonics_by_sec++;

        std::string new_title = "[SolanaChecker] - Seaching for SOL wallets / [Checked mnemonics: " + std::to_string(checked_mnemonics) + " | Found balance: $" + std::to_string(found_balance) +
            " | Speed: " + std::to_string(speed) + "/s]";
        SetConsoleTitleA(new_title.c_str());

    }

}


void wallet_tracker(std::string address) {
    float start_balance = get_solana_balance(address);

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        float balance = get_solana_balance(address);
        if (start_balance != balance) {
            float num = balance - start_balance;
            std::string message = "new transaction in " + address + ", " + ((num > 0) ? "+" : "") + std::to_string(num);
            telegram::send_message_bot(message);
            start_balance = balance;
        }
    }
}