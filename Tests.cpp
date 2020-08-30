// Copyright 2020 Evgeny Zaytsev
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "Calculator.h"


TEST_CASE("CalculatorTest")
{
    using calculator::Calculator;
    using calculator::Error;
    using calculator::res_t;
    using helper::RetVal;

    SECTION("Success")
    {
        SECTION("-1 + 5 - 3")
        {
            std::istringstream is("-1 + 5 - 3\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 1);
        }
        SECTION("-10 + (8 * 2.5) - (3 / 1,5)")
        {
            std::istringstream is("-10 + (8 * 2.5) - (3 / 1,5)\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 8);
        }
        SECTION("1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)")
        {
            std::istringstream is("1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 11);
        }
        SECTION("2+2*2")
        {
            std::istringstream is("2+2*2\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 6);
        }
        SECTION("(5+15)/2-10+100")
        {
            std::istringstream is("(5+15)/2-10+100\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 100);
        }
        SECTION("10/2+100")
        {
            std::istringstream is("10/2+100\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 105);
        }
        SECTION("5-4")
        {
            std::istringstream is("5-4\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 1);
        }
        SECTION("5-4*2")
        {
            std::istringstream is("5-4*2\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == -3);
        }
        SECTION("(2-1)/1")
        {
            std::istringstream is("(2-1)/1\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 1);
        }
        SECTION("(-200+199)/1/2")
        {
            std::istringstream is("(2-1)/1\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 1);
        }
        SECTION("(-1-1-1)")
        {
            std::istringstream is("(-1-1-1)\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == -3);
        }
        SECTION("-0/(1+1-1)")
        {
            std::istringstream is("-0/(1+1-1)\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 0);
        }
        SECTION("       2   +       2     * 2     ")
        {
            std::istringstream is("       2   +       2     * 2     \n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 6);
        }
        SECTION("(5+(2+3)*((5+5)/10))+1")
        {
            std::istringstream is("(5+(2+3)*((5+5)/10))+1\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 11);
        }
        SECTION("((5+5*(1*20-(1)+10)))")
        {
            std::istringstream is("((5+5*(1*20-(1)+10)))\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 150);
        }
        SECTION("")
        {
            std::istringstream is("(1.5 + 3,5)* 2\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(res.value() == 10);
        }
        SECTION("12,4+3.2")
        {
            std::istringstream is("12,4+3.2\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(static_cast<uint64_t>(res.value()*100) == 1560);
        }
        SECTION("12.3+15.2")
        {
            std::istringstream is("12.3+15.2\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Success);
            REQUIRE(static_cast<uint64_t>(res.value()*100) == 2750);
        }
        //todo: add inversion at the beginning and inside for -(-1-1-1), -(1-1-1) + 1, 5-(-(4-5)) etc.
    }

    SECTION("Errors")
    {
        SECTION("1.1 + 2.1 + abc")
        {
            std::istringstream is("1.1 + 2.1 + abc\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::IncorrectInput);
            std::ostringstream oss;
            oss << is.rdbuf();
            REQUIRE(oss.str() == "abc\n");
        }
        SECTION("()gfxckhk:PJn'n")
        {
            std::istringstream is("()gfxckhk:PJn'n\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::IncorrectInput);
            std::ostringstream oss;
            oss << is.rdbuf();
            REQUIRE(oss.str() == "gfxckhk:PJn'n\n");
        }
        SECTION("-10/10/2/1/1/-1")
        {
            std::istringstream is("-10/10/2/1/1/-1\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::IncorrectInput);
            std::ostringstream oss;
            oss << is.rdbuf();
            REQUIRE(oss.str() == "-1\n");
        }
        SECTION("121+535(0")
        {
            std::istringstream is("121+535(0\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::IncorrectInput); //todo: maybe if rdbuf == "" then output "Check parenthesis"
            std::ostringstream oss;
            oss << is.rdbuf();
            REQUIRE(oss.str().empty());
        }
        SECTION("(5+(2+3)*((5+5)/10))+1)")
        {
            std::istringstream is("(5+(2+3)*((5+5)/10))+1)\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::IncorrectInput); //todo: parenthesis
            std::ostringstream oss;
            oss << is.rdbuf();
            REQUIRE(oss.str() == "\n");
        }
        SECTION("(()()()())")
        {
            std::istringstream is("(()()()())\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::IncorrectInput);
        }
        SECTION("")
        {
            std::istringstream is("");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::ReadError);
        }
        SECTION("(/")
        {
            std::istringstream is("(/\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::IncorrectInput);
            std::ostringstream oss;
            oss << is.rdbuf();
            REQUIRE(oss.str() == "/\n");
        }
        SECTION("1/0")
        {
            std::istringstream is("1/0\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::DivisionByZero);
        }
        SECTION("max_res_t * 2")
        {
            std::istringstream is(std::to_string(std::numeric_limits<res_t>::max()) + "*2\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::Overflow);
        }
        SECTION("Double comma")
        {
            std::istringstream is("1,3,5\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::IncorrectInput);
        }
        SECTION("Double comma")
        {
            std::istringstream is("1.3.5\n");
            RetVal<res_t> res;
            REQUIRE_NOTHROW(res = Calculator(&is).run());
            REQUIRE(res == Error::IncorrectInput);
        }
    }
    /// todo: add spaces support and round up to 2 digits after comma
    ///todo: 5-(-(4-5))
    ///todo: maybe, too stupid user: +100/(+100+100+1)
//    SECTION("")
//    {
//        std::istringstream is("(2-1)/1\n");
//        double res;
//        REQUIRE_NOTHROW(res = Calculator(&is).run().value());
//        REQUIRE(res == 1);
//    }
//    SECTION("") //todo
//    {
//        std::istringstream is("-(-10)\n");
//        double res;
//        REQUIRE_NOTHROW(res = Calculator(&is).run().value());
//        REQUIRE(res == 10);
//    }
}