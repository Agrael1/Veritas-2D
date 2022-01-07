#include <gtest/gtest.h>

extern "C"
{
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
	size_t count()const
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
	Ring(int)ring;
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
	size_t count()const
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
	Ring(String)ring;
};


TEST(VRing, creation) {
	{
		CVRing s(16);
		EXPECT_TRUE(s.count() == 0);
		s.push(15);
		EXPECT_TRUE(s.count() == 1);
		EXPECT_EQ(s.top(), 15);
	}
}

TEST(VRing, fill) {
	{
		CVRing s(4);
		EXPECT_TRUE(s.count() == 0);
		s.push(15);
		s.push(14);
		s.push(13);
		s.push(12);
		EXPECT_TRUE(s.count() == 4);
		EXPECT_EQ(s.top(), 12);
		s.push(11);
		EXPECT_TRUE(s.count() == 4);
		EXPECT_EQ(s.top(), 11);
		s.push(11);
		s.push(11);
		s.push(11);
		s.push(11);
		EXPECT_TRUE(s.count() == 4);
		EXPECT_EQ(s.top(), 11);
	}
}

TEST(VRing, pop) {
	{
		CVRing s(4);
		EXPECT_TRUE(s.count() == 0);
		s.pop();
		EXPECT_TRUE(s.count() == 0);


		s.push(15);
		s.push(14);
		s.push(13);
		s.push(12);
		s.pop();
		EXPECT_TRUE(s.count() == 3);
		EXPECT_EQ(s.top(), 13);


		s.push(11);
		EXPECT_TRUE(s.count() == 4);
		EXPECT_EQ(s.top(), 11);


		s.push(11);
		s.push(11);
		s.push(11);
		s.push(11);
		s.pop();
		s.pop();
		s.pop();
		s.pop();
		EXPECT_TRUE(s.count() == 0);
	}
}


TEST(VRingS, creation) {
	{
		CVRingS s(16);
		EXPECT_TRUE(s.count() == 0);
		s.push("hello");
		EXPECT_TRUE(s.count() == 1);
		EXPECT_STREQ(c_str(s.top()), "hello");
	}
}

TEST(VRingS, fill) {
	{
		CVRingS s(4);
		EXPECT_TRUE(s.count() == 0);
		s.push("We");
		s.push("can");
		s.push("do");
		s.push("better");
		EXPECT_TRUE(s.count() == 4);
		EXPECT_STREQ(c_str(s.top()), "better");
		s.push("Hi");
		EXPECT_TRUE(s.count() == 4);
		EXPECT_STREQ(c_str(s.top()), "Hi");
		s.push("Hi");
		s.push("Hi");
		s.push("Hi");
		s.push("Hi");
		EXPECT_TRUE(s.count() == 4);
		EXPECT_STREQ(c_str(s.top()), "Hi");
	}
}

TEST(VRingS, pop) {
	{
		CVRingS s(4);
		EXPECT_TRUE(s.count() == 0);
		s.pop();
		EXPECT_TRUE(s.count() == 0);


		s.push("We");
		s.push("can");
		s.push("do");
		s.push("better");
		s.pop();
		EXPECT_TRUE(s.count() == 3);
		EXPECT_STREQ(c_str(s.top()), "do");


		s.push("Hi");
		EXPECT_TRUE(s.count() == 4);
		EXPECT_STREQ(c_str(s.top()), "Hi");


		s.push("Hi");
		s.push("Hi");
		s.push("Hi");
		s.push("Hi");
		s.pop();
		s.pop();
		s.pop();
		s.pop();
		EXPECT_TRUE(s.count() == 0);
	}
}