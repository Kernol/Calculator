#include "Calculator.h"

const std::unordered_map<char, uint16_t> calculator::Calculator::_precedence = /*todo: edit to uint8_t*/
        {
                {'(', 1}, {')', 1},
                {'-', 2}, {'+', 2},
                {'*', 3}, {'/', 3}
        };

double calculator::Calculator::_readNumber() const //todo: get negative numbers
{
    char ch;
    double a = 0;
    std::cin.get(ch);
    if(isdigit(ch))
    {
        bool commaFound = false;
        std::stringstream ss;
        ss.put(ch);
        while(std::cin.get(ch))
        {
            if(std::cin.fail())
                return 1; ///error
            if(isdigit(ch) || ch =='.')
            {
                ss.put(ch);
            }
            else if(ch == ',' && !commaFound) ///todo: test double comma
            {
                ss.put('.');
                commaFound = true;
            }
            else if(ch == '\n')
                break;
            else ///return
                std::cout << "Wrong input";
        }
        std::cout << ss.rdbuf();
        ss >> a;
        std::cout.precision(10);
        std::cout << std::fixed << std::endl << "double: " << a;
    }
    return a;
}

bool calculator::Calculator::_addOpToStack(char ch) //todo: bool return?
{
    while ( (_precedence.at(_opStack.top()) >= _precedence.at(ch)) /*&& (!_opStack.empty())*/ ) //todo: edit
        _addOpToResult();
    _opStack.push(ch);
    return true;
}

bool calculator::Calculator::_addOpToResult() // todo:bool? need parameter everywhere?
{
    if(_resStack.size() < 2 || _opStack.empty())
        return false;
    auto num1 = _resStack.top();
    _resStack.pop();
    auto num2 = _resStack.top();
    _resStack.pop();
    double res;
    switch (_opStack.top()) //todo: check delete and multiply (overflow, divide by 0 etc.)
    {
        case '+':
            res = num1 + num2;
            break;
        case '-':
            res = num1 - num2;
            break;
        case '*':
            res = num1 * num2;
            break;
        case '/':
            res = num1 / num2;
            break;
        default:
            return false; //todo: improve
    }
    _opStack.pop();
    _resStack.push(res);
    return true;
}

double calculator::Calculator::run()
{
    //double result = 0;
    char ch;
    uint8_t minusDetected = 0;
    char prevChar = 'N'; // to detect if one operator is situated after another
    while(std::cin.get(ch) && prevChar != 'e')
    {
        if(std::cin.fail()) //todo: add conditions
            return 0; //todo:edit
        if(prevChar == 'o' && _precedence.find(ch) != _precedence.cend() && ch != '-') // if operator again
            return 0; //todo:edit
        switch(ch)
        {
            case '+':
                _addOpToStack('+');
                prevChar = 'o';
                break;
            case '-': //todo: complete
                ++minusDetected;
                if(!minusDetected)
                {
                    _addOpToStack('-');
                    break;
                }
                else if(minusDetected > 1)
                {
                    break; ///todo: return incorrect error
                }
                else
                {

                }
                prevChar = 'o';
                break;
            case '*':
                _addOpToStack('*');
                prevChar = 'o';
                break;
            case '/':
                _addOpToStack('/');
                prevChar = 'o';
                break;
            case '(':
                _opStack.push('(');
                prevChar = 'b';
                break;
            case ')':
                while(_opStack.top() != '(')
                {
                    if (!_addOpToResult())
                        return 0; //todo: edit
                }
                _opStack.pop();
                prevChar = 'b';
                break;
            case '\n':
                prevChar = 'e';
                break;
            default: //todo: complete
                if(isdigit(ch))
                {
                    _resStack.push(_readNumber());
                    prevChar = 'd';
                }
                else
                    return 0; //todo: edit
                break;
        }
    }
    while(!_opStack.empty())
    {
        if (!_addOpToResult())
            return 0; //todo: edit
    }
    if(_resStack.size() > 1)
        return 0; //todo: return error
    return _resStack.top();
}