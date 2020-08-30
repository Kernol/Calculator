//
// Created by asuran on 30.08.20. //todo: edit
//

#ifndef CALCULATOR_CALCULATOR_H
#define CALCULATOR_CALCULATOR_H

#include <iostream>
#include <stack>
#include <sstream>
#include <unordered_map>

using res_t = size_t; //TODO: change to floating point type

/*
* TODO:
* Errors handling
* Descriptions to methods
* For unit tests add defines to make methods public
* Add qualifiers to methods
* (((Converts expression to Reverse Polish notation)))
*/

namespace calculator //todo: needed?
{
    class Calculator
    {
        std::stack<double> _resStack; /// Result stack
        std::stack<char> _opStack; /// Operators stack

        static const std::unordered_map<char, uint16_t> _precedence;

        bool _addOpToStack(char ch);

        bool _addOpToResult();

        double _readNumber() const; //todo: think of noexcept, parse also negatives

    public:
        Calculator() = default;

        Calculator(const Calculator &c) = delete;

        Calculator(Calculator &&c) = delete;

        Calculator &operator=(const Calculator &c) = delete;

        Calculator &operator=(Calculator &&c) = delete;

        double run();
    };
}

#endif //CALCULATOR_CALCULATOR_H
