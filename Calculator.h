// Copyright 2020 Evgeny Zaytsev
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

#ifndef CALCULATOR_CALCULATOR_H
#define CALCULATOR_CALCULATOR_H

#include <iostream>
#include <stack>
#include <sstream>
#include <unordered_map>

namespace helper
{
    /*!
     * @class RetVal
     * Provides convenient return type
     */
    template <typename T = char, typename Error = uint16_t>
    class RetVal
    {
        Error _error;
        std::optional<T> _value;
    public:
        RetVal() noexcept               : _error(0), _value(std::nullopt) {}
        RetVal(Error e) noexcept        : _error(e), _value(std::nullopt) {}
        RetVal(Error e, const T& v)     : _error(e), _value(v) {}
        RetVal(Error e, T&& v) noexcept : _error(e), _value(std::move(v)) {}
        RetVal(const RetVal& rv)        : _error(rv._error), _value(rv.value()) {}
        RetVal(RetVal&& rv) noexcept    : _error(rv._error), _value(std::move(rv.value())) {}
        RetVal& operator= (const RetVal& rv)     { _error = rv._error; _value = rv._value; return *this; }
        RetVal& operator= (RetVal&& rv) noexcept { _error = rv._error; _value = std::move(rv._value); return *this; }
        operator const Error&() const { return _error; }

        T value() const { return *_value; }
        Error error() const { return _error; }
    };
}

//todo:Add qualifiers to methods

namespace calculator
{
    using helper::RetVal;
    using res_t = double;  ///! Result type

    enum Error : uint8_t { ///! Error codes
        Success = 0,
        IncorrectInput = 1,
        DivisionByZero = 2,
        Overflow = 3,
        ReadError = 4
    };

    /*!
     * @class Calculator
     * @brief Calculates arithmetic expressions
     * Possible operations: + - * /. Supports parenthesis and floating point numbers. Some examples in Tests.cpp
     * Uses Shunting-yard algorithm to represent expression in Reverse Polish notation.
     */
    class Calculator
    {
        std::stack<res_t> _resStack;   ///! Result stack
        std::stack<char> _opStack;     ///! Operators stack
        static const std::unordered_map<char, uint8_t> _precedence; ///! Precedence of operators
        std::istream* _is = &std::cin; ///! Input stream

        /*!
         * @defgroup stackErrors
         * @retval DivisionByZero
         * @retval Overflow
         * @retval IncorrectInput
         */

        /*!
         * Add operator to operators stack
         * @return Error code
         * @retval Success
         * @copydetails stackErrors
         */
        Error _addOpToStack(char);

        /*!
         * Add operator to result stack, initiates computation
         * @return Error code
         * @retval Success
         * @copydetails stackErrors
         */
        Error _addOpToResult();

        /*!
         * Reads next character from input stream
         * @return Error code (and value)
         * @retval Success and value(next character)
         * @retval ReadError
         */
        RetVal<char> _getChar() const;

        /*!
         * Parses number from input stream
         * @param ch First character of number
         * @return Error code (and value)
         * @retval Success and value(parsed number)
         * @retval ReadError
         */
        RetVal<res_t> _readNumber(char ch) const; //todo: think of noexcept

    public:

        Calculator() = default;
        Calculator(std::istream* is) : _is(is) {}

        Calculator(const Calculator &) = delete;
        Calculator(Calculator &&) = delete;
        Calculator &operator=(const Calculator &) = delete;
        Calculator &operator=(Calculator &&) = delete;

        /*!
         * Executes expression computation
         * @return Error code (and value)
         * @retval Success and value(expression result)
         * @retval ReadError
         * @copydetails stackErrors
         */
        RetVal<res_t> run();
    };
}

#endif //CALCULATOR_CALCULATOR_H