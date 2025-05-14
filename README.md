# LLM Chat - C++ Client for LM Studio

A lightweight command-line interface to interact with local large language models through the LM Studio API server, written in modern C++.

## Features

* Simple and interactive chat interface using C++
* Communicates directly with LM Studio's OpenAI-compatible API
* JSON parsing with [nlohmann/json](https://github.com/nlohmann/json)
* HTTP requests via [libcurl](https://curl.se/libcurl/)
* Displays assistant's content only

## Prerequisites

* A C++17 (or newer) compiler
* [libcurl](https://curl.se/libcurl/) installed
* [nlohmann/json](https://github.com/nlohmann/json) (single header file `json.hpp`)
* [LM Studio](https://lmstudio.ai/) with a model loaded and the API server running

## Installation

1. Clone this repository or copy the source code:

```bash
git clone https://github.com/jero98772/llm-chat-cpp
cd llm-chat-cpp
```

2. Make sure `libcurl` and `json.hpp` are available in your environment.

3. Build the application:

```bash
g++ -std=c++17 -lcurl -o llm-chat main.cpp
```

## Usage

Run the application from the command line:

```bash
./llm-chat
```

You'll be prompted to enter your message. Type `exit` to quit.

### Example Output

```
> Hello, who are you?
Assistant: I'm your local AI assistant running on LM Studio. How can I help you today?
```

## Customization

You can change the following parameters directly in the code:

* **API Endpoint:**
  Change the `url` string (default: `http://localhost:1234/v1/chat/completions`)

* **Model Name:**
  Update the `model` string (default: `"llama-3.2-1b-instruct"`)

* **API Key:**
  Change the `api_key` string (default: `"lm-studio"`)

## Setting Up LM Studio

1. Download and install [LM Studio](https://lmstudio.ai/)
2. Start LM Studio and load your preferred model
3. Go to the “Local Server” tab
4. Click **Start Server**
5. Verify the API endpoint matches the one in your code (`http://localhost:1234` by default)

## Project Structure

* `main.cpp` – The main chat interface source file
* `README.md` – Project documentation

## License

This project is licensed under the [GPLv3](LICENSE) License.

