#include <catch2/catch_test_macros.hpp>

extern "C" {
#include <VString.h>
#define RING_T int
#include <VRingT.h>
#define RING_T String
#define IsClass
#include <VRingT.h>
}

class CVRing
{
public:
    CVRing(uint16_t r)
    {
        ::Ring_int_ctor(&ring, r);
    }
    ~CVRing()
    {
        ::Ring_int_dtor(&ring);
    }

public:
    size_t count() const
    {
        return ring.contains;
    }
    void push(int i)
    {
        *::emplace_Ring_int(&ring) = i;
    }
    void pop()
    {
        ::pop_Ring_int(&ring);
    }
    int& top()
    {
        return *::top_Ring_int(&ring);
    }

private:
    Ring(int) ring;
};

class CVRingS
{
public:
    CVRingS(uint16_t r)
    {
        ::Ring_String_ctor(&ring, r);
    }
    ~CVRingS()
    {
        ::Ring_String_dtor(&ring);
    }

public:
    size_t count() const
    {
        return ring.contains;
    }
    void push(std::string_view c)
    {
        ::String_ctor(emplace_Ring_String(&ring), c.data(), c.size());
    }
    void pop()
    {
        ::pop_Ring_String(&ring);
    }
    String* top()
    {
        return ::top_Ring_String(&ring);
    }

private:
    Ring(String) ring;
};

TEST_CASE("ring_creation")
{
    CVRing s(16);
    REQUIRE(s.count() == 0);
    s.push(15);
    REQUIRE(s.count() == 1);
    REQUIRE(s.top() == 15);
}

TEST_CASE("ring_fill")
{
    CVRing s(4);
    REQUIRE(s.count() == 0);
    s.push(15);
    s.push(14);
    s.push(13);
    s.push(12);
    REQUIRE(s.count() == 4);
    REQUIRE(s.top() == 12);
    s.push(11);
    REQUIRE(s.count() == 4);
    REQUIRE(s.top() == 11);
    s.push(11);
    s.push(11);
    s.push(11);
    s.push(11);
    REQUIRE(s.count() == 4);
    REQUIRE(s.top() == 11);
}

TEST_CASE("ring_pop")
{
    CVRing s(4);
    REQUIRE(s.count() == 0);
    s.pop();
    REQUIRE(s.count() == 0);

    s.push(15);
    s.push(14);
    s.push(13);
    s.push(12);
    s.pop();
    REQUIRE(s.count() == 3);
    REQUIRE(s.top() == 13);

    s.push(11);
    REQUIRE(s.count() == 4);
    REQUIRE(s.top() == 11);

    s.push(11);
    s.push(11);
    s.push(11);
    s.push(11);
    s.pop();
    s.pop();
    s.pop();
    s.pop();
    REQUIRE(s.count() == 0);
}

TEST_CASE("vring_s_creation")
{
    CVRingS s(16);
    REQUIRE(s.count() == 0);
    s.push("hello");
    REQUIRE(s.count() == 1);
    REQUIRE(c_str(s.top()) == std::string_view("hello"));
}

TEST_CASE("vring_s_fill")
{
    CVRingS s(4);
    REQUIRE(s.count() == 0);
    s.push("We");
    s.push("can");
    s.push("do");
    s.push("better");
    REQUIRE(s.count() == 4);
    REQUIRE(c_str(s.top()) == std::string_view("better"));
    s.push("Hi");
    REQUIRE(s.count() == 4);
    REQUIRE(c_str(s.top()) == std::string_view("Hi"));
    s.push("Hi");
    s.push("Hi");
    s.push("Hi");
    s.push("Hi");
    REQUIRE(s.count() == 4);
    REQUIRE(c_str(s.top()) == std::string_view("Hi"));
}

TEST_CASE("vring_s_pop")
{
    {
        CVRingS s(4);
        REQUIRE(s.count() == 0);
        s.pop();
        REQUIRE(s.count() == 0);

        s.push("We");
        s.push("can");
        s.push("do");
        s.push("better");
        s.pop();
        REQUIRE(s.count() == 3);
        REQUIRE(c_str(s.top()) == std::string_view("do"));

        s.push("Hi");
        REQUIRE(s.count() == 4);
        REQUIRE(c_str(s.top()) == std::string_view("Hi"));

        s.push("Hi");
        s.push("Hi");
        s.push("Hi");
        s.push("Hi");
        s.pop();
        s.pop();
        s.pop();
        s.pop();
        REQUIRE(s.count() == 0);
    }
}