// Copyright 2020 Evgeny Zaytsev
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

#include "Calculator.h"

int main()
{
    using calculator::Calculator;
    std::cin.sync_with_stdio(false); // for correct std::cin.rdbuf()->in_avail()
    try {
        std::cout << "Type expression:\n";
        auto res = Calculator().run();
        switch (res.error())
        {
            case calculator::IncorrectInput: {
                if (std::cin.rdbuf()->in_avail() > 1) {
                    std::cout << "Некорректный ввод, строка содержит недопустимое выражение ";
                    for (auto i = std::cin.rdbuf()->in_avail(); i > 1; --i)
                        std::cout << static_cast<char>(std::cin.rdbuf()->sbumpc());
                    std::cout << std::endl;
                }
                else
                    std::cout << "Ошибка: пустой ввод / несоответствие скобок\n";
                break;
            }
            case calculator::DivisionByZero:
                std::cout << "Ошибка: деление на 0\n";
                break;
            case calculator::Overflow:
                std::cout << "Ошибка: переполнение\n";
                break;
            case calculator::ReadError:
                std::cout << "Ошибка: некорректный ввод\n";
                break;
            default: //Success
                std::cout.precision(2);
                std::cout << std::fixed << res.value() << std::endl;
                break;
        }
    }
    catch(std::exception& ex){
        std::cout << "Исключение: " << ex.what() << std::endl;
    }
    return 0;
}