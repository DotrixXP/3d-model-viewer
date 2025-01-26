#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>

class Log {
  public:
    template <typename... Args>
    static void Error(Args... args) {
        (std::cout << ... << args) << std::endl;
    }

    template <typename... Args>
    static void Warn(Args... args) {
        (std::cout << ... << args) << std::endl;
    }

    template <typename... Args>
    static void Info(Args... args) {
        (std::cout << ... << args) << std::endl;
    }
};
