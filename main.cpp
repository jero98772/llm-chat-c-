#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

std::string chat(CURL* curl, const std::string& url, const std::string& api_key, const std::string& model, const std::string& user_input) {
    std::string request_body = R"({
        "model": ")" + model + R"(",
        "messages": [
            { "role": "user", "content": ")" + user_input + R"(" }
        ],
        "stream": false
    })";

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());

    std::string response_string;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);

    if (res != CURLE_OK) {
        return "Error: curl_easy_perform() failed: " + std::string(curl_easy_strerror(res));
    }

    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code != 200) {
        return "Error: API request failed with status code: " + std::to_string(response_code) + "\n" + response_string;
    }

    try {
        json response_json = json::parse(response_string);
        if (response_json.contains("choices") && !response_json["choices"].empty()) {
            const auto& first_choice = response_json["choices"][0];
            if (first_choice.contains("message") && first_choice["message"].contains("content")) {
                return first_choice["message"]["content"].get<std::string>();
            } else {
                return "Error: Could not find 'content' in the assistant's message.\nRaw Response: " + response_string;
            }
        } else {
            return "Error: 'choices' array is empty or not found.\nRaw Response: " + response_string;
        }
    } catch (const json::parse_error& e) {
        return "Error: Failed to parse JSON response: " + std::string(e.what()) + "\nRaw Response: " + response_string;
    }
}

int main() {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return 1;
    }

    std::string url = "http://localhost:1234/v1/chat/completions";
    std::string api_key = "lm-studio";
    std::string model = "llama-3.2-1b-instruct";

    std::cout << "\nChat with LM Studio (Assistant Content Only)";
    std::cout << "\nType your message and press Enter (type 'exit' to quit).\n";

    std::string user_input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, user_input);

        if (user_input == "exit") {
            break;
        }

        if (user_input.empty()) {
            continue;
        }

        std::string response = chat(curl, url, api_key, model, user_input);
        std::cout << "Assistant: " << response << "\n";
    }

    curl_easy_cleanup(curl);
    std::cout << "\nChat ended.\n";
    return 0;
}
