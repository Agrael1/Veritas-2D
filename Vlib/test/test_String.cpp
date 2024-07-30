#include <catch2/catch_test_macros.hpp>
#include <string>

extern "C" {
#include <VString.h>
}

class CVString
{
public:
    CVString()
    {
        String_ctor(&s, nullptr, 0);
    }
    CVString(const char* str)
    {
        String_ctor(&s, str, 0);
    }
    ~CVString()
    {
        String_dtor(&s);
    }

public:
    bool operator==(const char* str) const
    {
        return c_str() == std::string_view(str);
    }

    const char* c_str() const
    {
        return ::c_str(&s);
    }
    auto empty() const
    {
        return ::string_empty(&s);
    }
    auto length() const
    {
        return ::string_length(&s);
    }
    bool is_long() const
    {
        return s.large;
    }
    void push_back(char c)
    {
        string_push_back(&s, c);
    }
    void prepend(std::string_view c)
    {
        ::string_prepend(&s, c.data());
    }
    void append(std::string_view c)
    {
        ::string_append(&s, c.data());
    }
    void pop_front()
    {
        ::string_pop_front(&s);
    }

private:
    String s;
};

TEST_CASE("string_creation")
{
    {
        CVString s;
        REQUIRE(s.empty());
        REQUIRE(s.length() == 0);
        REQUIRE_FALSE(s.is_long());
    }

    {
        using namespace std::string_view_literals;
        CVString s{ "abcd" };
        REQUIRE_FALSE(s.empty());
        REQUIRE(s.length() == 4);
        REQUIRE(s.c_str() == "abcd"sv);
        REQUIRE_FALSE(s.is_long());
    }

    {
        using namespace std::string_view_literals;
        std::string a = "abcdefghierdfzsdzsczcsdfzds";
        CVString s{ a.c_str() };
        REQUIRE_FALSE(s.empty());
        REQUIRE(s.length() == a.size());
        REQUIRE(s.c_str() == a);
        REQUIRE(s.is_long());
    }
}
TEST_CASE("string_push_back")
{
    {
        CVString s;
        s.push_back('a');
        REQUIRE_FALSE(s.empty());
        REQUIRE(s.length() != 0);
        REQUIRE(s == "a");
    }

    {
        CVString s{ "abcd" };
        s.push_back('e');
        REQUIRE(s.length() ==  5);
        REQUIRE(s == "abcde");
        s.push_back('f');
        REQUIRE(s.length() ==  6);
        REQUIRE(s == "abcdef");
    }

    {
        std::string a = "abcdefghierdfzsdzsczcsdfzds";
        CVString s{ a.c_str() };
        s.push_back('e');
        REQUIRE(s.length() ==  a.size() + 1);
        REQUIRE(s == "abcdefghierdfzsdzsczcsdfzdse");
        s.push_back('f');
        REQUIRE(s.length() ==  a.size() + 2);
        REQUIRE(s == "abcdefghierdfzsdzsczcsdfzdsef");
    }

    {
        std::string a = "abcdefghierdfzsdzsczcs";
        CVString s{ a.c_str() };
        REQUIRE_FALSE(s.is_long());
        s.push_back('e');
        REQUIRE(s.length() ==  a.size() + 1);
        REQUIRE(s == "abcdefghierdfzsdzsczcse");
        REQUIRE_FALSE(s.is_long());
        s.push_back('f');
        REQUIRE(s.length() ==  a.size() + 2);
        REQUIRE(s == "abcdefghierdfzsdzsczcsef");
        REQUIRE(s.is_long());
    }
}
TEST_CASE("string_prepend")
{
    {
        CVString s;
        s.prepend("a");
        REQUIRE_FALSE(s.empty());
        REQUIRE(s.length() !=  0);
        REQUIRE(s == "a");
    }

    {
        CVString s{ "abcd" };
        s.prepend("e");
        REQUIRE(s.length() ==  5);
        REQUIRE(s == "eabcd");
        s.prepend("fuck");
        REQUIRE(s.length() ==  9);
        REQUIRE(s == "fuckeabcd");
    }

    {
        std::string a = "abcdefghierdfzsdzsczcsdfzds";
        CVString s{ a.c_str() };
        s.prepend("e");
        REQUIRE(s.length() ==  a.size() + 1);
        REQUIRE(s == "eabcdefghierdfzsdzsczcsdfzds");
        s.prepend("fuck");
        REQUIRE(s.length() ==  a.size() + 5);
        REQUIRE(s == "fuckeabcdefghierdfzsdzsczcsdfzds");
    }

    {
        std::string a = "abcdefghierdfzsdzsczcs";
        CVString s{ a.c_str() };
        REQUIRE_FALSE(s.is_long());
        s.prepend("e");
        REQUIRE(s.length() ==  a.size() + 1);
        REQUIRE(s == "eabcdefghierdfzsdzsczcs");
        REQUIRE_FALSE(s.is_long());
        s.prepend("fuck");
        REQUIRE(s.length() ==  a.size() + 5);
        REQUIRE(s == "fuckeabcdefghierdfzsdzsczcs");
        REQUIRE(s.is_long());
    }
}

TEST_CASE("string_append")
{
    {
        CVString s;
        s.append("a");
        REQUIRE_FALSE(s.empty());
        REQUIRE(s.length() !=  0);
        REQUIRE(s == "a");
    }

    {
        CVString s{ "abcd" };
        s.append("e");
        REQUIRE(s.length() ==  5);
        REQUIRE(s == "abcde");
        s.append("funn");
        REQUIRE(s.length() ==  9);
        REQUIRE(s == "abcdefunn");
    }

    {
        std::string a = "abcdefghierdfzsdzsczcsdfzds";
        CVString s{ a.c_str() };
        s.append("e");
        REQUIRE(s.length() ==  a.size() + 1);
        REQUIRE(s == "abcdefghierdfzsdzsczcsdfzdse");
        s.append("funn");
        REQUIRE(s.length() ==  a.size() + 5);
        REQUIRE(s == "abcdefghierdfzsdzsczcsdfzdsefunn");
    }

    {
        std::string a = "abcdefghierdfzsdzsczcs";
        CVString s{ a.c_str() };
        REQUIRE_FALSE(s.is_long());
        s.append("e");
        REQUIRE(s.length() ==  a.size() + 1);
        REQUIRE(s == "abcdefghierdfzsdzsczcse");
        REQUIRE_FALSE(s.is_long());
        s.append("funn");
        REQUIRE(s.length() ==  a.size() + 5);
        REQUIRE(s == "abcdefghierdfzsdzsczcsefunn");
        REQUIRE(s.is_long());
    }
}

TEST_CASE("string_pop_front")
{
    {
        CVString s{ "a" };
        s.pop_front();
        REQUIRE(s.empty());
        REQUIRE(s == "");
    }

    {
        CVString s{ "abcd" };
        s.pop_front();
        REQUIRE(s.length() == 3);
        REQUIRE(s == "bcd");
    }

    {
        std::string a = "abcdefghierdfzsdzsczcsdfzds";
        CVString s{ a.c_str() };
        s.pop_front();
        REQUIRE(s.is_long());
        REQUIRE(s.length() == a.size() - 1);
        REQUIRE(s == "bcdefghierdfzsdzsczcsdfzds");
    }

    {
        std::string a = "abcdefghierdfzsdzsczcsee";
        CVString s{ a.c_str() };
        REQUIRE(s.is_long());
        s.pop_front();
        REQUIRE(s.length() == a.size() - 1);
        REQUIRE(s == "bcdefghierdfzsdzsczcsee");
        REQUIRE(s.is_long());
        s.pop_front();
        REQUIRE(s.length() == a.size() - 2);
        REQUIRE(s == "cdefghierdfzsdzsczcsee");
        REQUIRE(s.is_long());
    }
}