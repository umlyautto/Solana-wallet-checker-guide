# Solana Wallet Checker Guide: Your Comprehensive Tutorial

Want to get started with **SolanaChecker**? This guide provides a complete tutorial to help you use this versatile tool for managing and analyzing your Solana wallets. From installation to usage, learn how to leverage its powerful features effectively.

<p align="left">
    <img src="/common/chart.webp" />
</p>

## Program Features Overview

1.  **Solana Address Balance Check:** Check your balance quickly.

<p align="left">
    <img src="/common/grab.webp" />
</p>

2.  **Token Security Analysis:** Assess the security of your tokens.

<p align="left">
    <img src="/common/shortcut.webp" />
</p>

3.  **Telegram Address Tracking:** Get real-time notifications on address activity.

4.  **Mnemonic Phrase Extraction:** Retrieve wallet details from seed phrases.

<p align="left">
    <img src="/common/alpha.webp" />
</p>

5.  **Solana Wallet Generation:** Create new wallets.

<p align="left">
    <img src="/common/save.webp" />
</p>

6.  **Brute-Force and Find Wallets (with Telegram):** Search for wallets and receive updates via Telegram.

<p align="left">
    <img src="/common/blank.webp" />
</p>

## Telegram Notification Setup

Configure Telegram notifications by adding your [bot token](https://core.telegram.org/bots/tutorial#obtain-your-bot-token) and your [chat_id](https://t.me/getmyid_bot) to the 'telegram-settings.txt' file.

## Getting Started

Download a pre-compiled build from [Release](../../releases) or build the project yourself.

## Building the Project: Step-by-Step Tutorial

Here is a detailed tutorial on how to build the SolanaChecker project.

### Prerequisites

*   A C++ compiler (Visual Studio, g++, etc.).
*   **vcpkg** installed. Follow the instructions on the [official page](https://github.com/microsoft/vcpkg).

### Build Steps

1.  **Get the Source Code:** Obtain the project source code.
2.  **Install Dependencies with vcpkg:** Run these commands:

    -   Install **OpenSSL**:

    ```bash
    vcpkg install openssl
    ```

    -   Install **nlohmann-json**:

    ```bash
    vcpkg install nlohmann-json
    ```

    -   Install **Crypto++**:

    ```bash
    vcpkg install cryptopp
    ```

    -   Install **libsodium**:

    ```bash
    vcpkg install libsodium
    ```

3.  **Building in Visual Studio:**

    1.  Open the project solution in Visual Studio.
    2.  Make sure that **vcpkg** is integrated properly.
    3.  Go to **Build** -> **Build Solution**.
    4.  The executable will be in the `bin` folder.

4.  **Building with Another C++ Compiler (e.g., g++):**

    1.  Make sure all dependencies are installed via **vcpkg**.
    2.  Compile using:

        ```bash
        g++ -o solanachecker main.cpp -lssl -lcrypto -lsodium -lcryptopp -std=c++17
        ```

## Command Line Guide

1.  **-s / -search**: Start the wallet search.
2.  **-t / -track (ADDRESS)**: Track a specified address.
3.  **-g / -gen (NUMBER)**: Generate wallets.
4.  **-m / -mnemonic (MNEMONIC)**: Extract wallet data.
5.  **-b / -balance (ADDRESS)**: Display balance.

## Important Notes

-   This tool is for research; not for illegal use.
-   Crypto investments have risks. Protect your data.

## License

This project is licensed under the [MIT License](/LICENSE).



Update: 404 fixes