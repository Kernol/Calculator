// Copyright 2020 Evgeny Zaytsev
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

#include "Calculator.h"

using helper::RetVal;
using calculator::Error;
using calculator::res_t;

const std::unordered_map<char, uint8_t> calculator::Calculator::_precedence = {
                {'(', 1}, {')', 1},
                {'-', 2}, {'+', 2},
                {'*', 3}, {'/', 3}
        };

RetVal<res_t> calculator::Calculator::_readNumber(char ch) const
{
    std::stringstream ss;
    if(ch == '-') {
        ss.put('-');
        if(auto getRes = _getChar(); getRes == Success)
            ch = getRes.value();
        else
            return getRes.error();
    }
    if(!isdigit(ch)) {
        _is->putback(ch);
        return IncorrectInput;
    }
    bool commaFound = false;
    ss.put(ch);
    RetVal getRes;
    while((getRes = _getChar()) == Success) {
        ch = getRes.value();
        if(isdigit(ch))
            ss.put(ch);
        else if((ch =='.' || ch == ',') && !commaFound) {
            ss.put('.');
            commaFound = true;
        } else {
            _is->putback(ch);
            break;
        }
    }
    if(getRes != Success)
        return getRes.error();
    res_t result;
    ss >> result;
    return {Success, result};
}

Error calculator::Calculator::_addOpToStack(char ch)
{
    if(!_opStack.empty()) {
        while(_precedence.at(_opStack.top()) >= _precedence.at(ch)) {
            if(Error error = _addOpToResult(); error == Success) {
                if (_opStack.empty())
                    break;
            } else {
                return error;
            }
        }
    }
    _opStack.push(ch);
    return Success;
}

Error calculator::Calculator::_addOpToResult()
{
    if(_resStack.size() < 2 || _opStack.empty())
        return IncorrectInput;
    auto num1 = _resStack.top();
    _resStack.pop();
    auto num2 = _resStack.top();
    _resStack.pop();
    res_t res;
    switch (_opStack.top())
    {
        case '+':
            res = num1 + num2;
            break;
        case '-':
            res = num2 - num1;
            break;
        case '*':
            if(num1 != 0)
                if(std::numeric_limits<decltype(num1)>::max() / (num1 > 0 ? num1 : -num1) < (num2 > 0 ? num2 : -num2))
                    return Overflow;
            res = num1 * num2;
            break;
        case '/':
            if(num1 == 0)
                return DivisionByZero;
            res = num2 / num1;
            break;
        default:
            return IncorrectInput;
    }
    _opStack.pop();
    _resStack.push(res);
    return Success;
}

RetVal<char> calculator::Calculator::_getChar() const
{
    char ch;
    do {
        _is->get(ch);
        if(_is->fail())
            return ReadError;
    } while (std::isspace(static_cast<unsigned char>(ch)) && ch != '\n');
    return {Success, ch};
}

RetVal<res_t> calculator::Calculator::run()
{
    char ch;
    char prevChar = 'N';
    while(prevChar != 'e') {
        if(auto getRes = _getChar(); getRes == Success)
            ch = getRes.value();
        else
            return getRes.error();
        if(prevChar == 'o' && _precedence.find(ch) != _precedence.cend() && ch != '(' && ch != ')') { // if operator again
            _is->putback(ch);
            return IncorrectInput;
        }
        switch(ch)
        {
            case '-':
                if((_opStack.empty() && _resStack.empty()) || (prevChar == '(')) {
                    if (auto res = _readNumber('-'); res != Success)
                        return res.error();
                    else
                        _resStack.push(res.value());
                    prevChar = 'd';
                    break;
                }
                [[fallthrough]];
            case '+': [[fallthrough]];
            case '*': [[fallthrough]];
            case '/':
                if(prevChar == '(') {
                    _is->putback(ch);
                    return IncorrectInput;
                }
                if(Error error = _addOpToStack(ch); error != Success)
                    return error;
                prevChar = 'o';
                break;
            case '(':
                _opStack.push('(');
                prevChar = '(';
                break;
            case ')':
                if(_opStack.empty())
                    return IncorrectInput;
                while (_opStack.top() != '(') {
                    if (_addOpToResult() != Success)
                        return IncorrectInput;
                    if (_opStack.empty())
                        return IncorrectInput;
                }
                _opStack.pop();
                prevChar = ')';
                break;
            case '\n':
                prevChar = 'e';
                break;
            default:
                if(isdigit(ch)) {
                    if(auto res = _readNumber(ch); res != Success)
                        return res.error();
                    else
                        _resStack.push(res.value());
                    prevChar = 'd';
                } else {
                    _is->putback(ch);
                    return IncorrectInput;
                }
                break;
        }
    }
    while(!_opStack.empty()) {
        if (Error error =_addOpToResult(); error != Success)
            return error;
    }
    if(_resStack.size() > 1 || _resStack.empty())
        return IncorrectInput;
    return {Success, _resStack.top()};
}