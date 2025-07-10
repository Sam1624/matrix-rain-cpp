# Matrix Digital Rain in C++

![Matrix Rain Demo](./images/screenshot1.jpg)

A simple console application for POSIX-compliant systems (Linux, macOS) that simulates the "digital rain" effect from The Matrix movie.

This project was created as a learning exercise with the help of an AI assistant (ChatGPT by OpenAI) to explore console graphics in C++.

For the Russian version, see [README.ru.md](README.ru.md).

## Features
- **Linux/POSIX focused**: Clean, simple code without cross-platform boilerplate.
- **No external libraries**: Uses only the C++ standard library and standard POSIX headers.
- **Visuals**: Uses ANSI escape codes for colors and cursor positioning.

## How to Compile and Run
You'll need a C++ compiler like `g++`.

# Compile the source code
```bash
git clone https://github.com/Sam1624/matrix-rain-cpp.git
cd matrix-rain-cpp
g++ main.cpp -o matrix_rain -std=c++17 -pthread
```
# Run the application
```bash
./matrix_rain
```
