#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <WinInet.h>
#include <iomanip>
#include <nlohmann/json.hpp>

#pragma comment(lib, "wininet.lib")

using json = nlohmann::json;

namespace internet {

    class header {
    public:

        std::string text() {
            std::string hdr_text = this->str_header;
            return hdr_text;
        }

        void add_content(std::string content_name, std::string content_value) {
            std::string text = content_name + ": " + content_value + "\r\n";
            str_header += text;
        }
        void add_content(std::string content) {
            std::string text = content + "\r\n";
            str_header += text;
        }

    private:
        std::string str_header;
    };


   static BOOL request(std::string url, std::string method, std::string header, std::string content = "", std::string* response = nullptr, DWORD* code = nullptr) {
        std::string https_str = "https://";
        int n = url.find(https_str);
        if (n == std::string::npos)
            return false;
        url = url.substr(https_str.size());
        n = url.find("/");
        std::string server_name = url.substr(0, n);
        std::string object_name = url.substr(n);

        HINTERNET hIntSession = InternetOpenA((("Internet Default Agent")), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (hIntSession) {
            HINTERNET hHttpSession = InternetConnectA(hIntSession, server_name.c_str(),
                INTERNET_DEFAULT_HTTPS_PORT, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);
            if (hHttpSession) {
                HINTERNET hHttpRequest = HttpOpenRequestA(hHttpSession, method.c_str(), object_name.c_str(),
                    NULL, NULL, NULL,
                    (INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE |
                        INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE), 0);
                if (hHttpRequest) {
                    if (HttpSendRequestA(hHttpRequest,
                        header.c_str(), -1, (LPVOID)content.c_str(),
                        content.size())) {

                        DWORD dwStatusCode = 0;
                        {
                            CHAR szStatusCode[32] = { 0 };
                            DWORD dwStatusCodeSize = sizeof(szStatusCode) / sizeof(TCHAR);
                            if (HttpQueryInfoA(hHttpRequest, HTTP_QUERY_STATUS_CODE,
                                szStatusCode, &dwStatusCodeSize, NULL)) {
                                dwStatusCode = std::stoi(szStatusCode);
                            }

                        }

                        unsigned char* lpResponse = NULL;
                        DWORD dwTotalSize = 0;
                        {
                            unsigned char* lpBuffer = (unsigned char*)malloc(2048);
                            memset(lpBuffer, 0, 2048);
                            DWORD dwRead = 0;
                            while (InternetReadFile(hHttpRequest, lpBuffer, sizeof(lpBuffer), &dwRead) && dwRead) {
                                unsigned char* lpTmp = (unsigned char*)realloc(lpResponse, dwTotalSize + dwRead);
                                if (lpTmp) {
                                    lpResponse = lpTmp;
                                    memcpy(&lpResponse[dwTotalSize], lpBuffer, dwRead);
                                    dwTotalSize += dwRead;
                                }
                                dwRead = 0;
                            }

                            free(lpBuffer);
                        }
                        if (code)
                            *code = dwStatusCode;
                        if (response) {
                            *response = std::string((char*)lpResponse);
                            int n = response->find_last_of("}");
                            if (n == std::string::npos)
                                return false;
                            *response = response->substr(0, n + 1);
                        }

                        if (lpResponse) { free(lpResponse); }
                    }
                    InternetCloseHandle(hHttpRequest);
                }
                InternetCloseHandle(hHttpSession);
            }
            InternetCloseHandle(hIntSession);
        }
        return true;

    }


}

