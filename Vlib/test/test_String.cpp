#include <gtest/gtest.h>

extern "C"
{
#include <VString.h>
}

class CVString
{
public:
	CVString()
	{
		String_ctor(&s, nullptr,0);
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
	auto c_str()const
	{
		return ::c_str(&s);
	}
	auto empty()const
	{
		return ::string_empty(&s);
	}
	auto length()const
	{
		return ::string_length(&s);
	}
	bool is_long()const
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

TEST(String, creation) {
	{
		CVString s;
		EXPECT_TRUE(s.empty());
		EXPECT_EQ(s.length(), 0);
		EXPECT_FALSE(s.is_long());
	}

	{
		CVString s{ "abcd" };
		EXPECT_FALSE(s.empty());
		EXPECT_EQ(s.length(), 4);
		EXPECT_STREQ(s.c_str(), "abcd");
		EXPECT_FALSE(s.is_long());
	}

	{
		std::string a = "abcdefghierdfzsdzsczcsdfzds";
		CVString s{ a.c_str() };
		EXPECT_FALSE(s.empty());
		EXPECT_EQ(s.length(), a.size());
		EXPECT_STREQ(s.c_str(), a.c_str());
		EXPECT_TRUE(s.is_long());
	}
}
TEST(String, push_back) {
	{
		CVString s;
		s.push_back('a');
		EXPECT_FALSE(s.empty());
		EXPECT_NE(s.length(), 0);
		EXPECT_STREQ(s.c_str(), "a");
	}

	{
		CVString s{ "abcd" };
		s.push_back('e');
		EXPECT_EQ(s.length(), 5);
		EXPECT_STREQ(s.c_str(), "abcde");
		s.push_back('f');
		EXPECT_EQ(s.length(), 6);
		EXPECT_STREQ(s.c_str(), "abcdef");
	}

	{
		std::string a = "abcdefghierdfzsdzsczcsdfzds";
		CVString s{ a.c_str() };
		s.push_back('e');
		EXPECT_EQ(s.length(), a.size() + 1);
		EXPECT_STREQ(s.c_str(), "abcdefghierdfzsdzsczcsdfzdse");
		s.push_back('f');
		EXPECT_EQ(s.length(), a.size() + 2);
		EXPECT_STREQ(s.c_str(), "abcdefghierdfzsdzsczcsdfzdsef");
	}

	{
		std::string a = "abcdefghierdfzsdzsczcs";
		CVString s{ a.c_str() };
		EXPECT_FALSE(s.is_long());
		s.push_back('e');
		EXPECT_EQ(s.length(), a.size() + 1);
		EXPECT_STREQ(s.c_str(), "abcdefghierdfzsdzsczcse");
		EXPECT_FALSE(s.is_long());
		s.push_back('f');
		EXPECT_EQ(s.length(), a.size() + 2);
		EXPECT_STREQ(s.c_str(), "abcdefghierdfzsdzsczcsef");
		EXPECT_TRUE(s.is_long());
	}
}
TEST(String, prepend)
{
	{
		CVString s;
		s.prepend("a");
		EXPECT_FALSE(s.empty());
		EXPECT_NE(s.length(), 0);
		EXPECT_STREQ(s.c_str(), "a");
	}

	{
		CVString s{ "abcd" };
		s.prepend("e");
		EXPECT_EQ(s.length(), 5);
		EXPECT_STREQ(s.c_str(), "eabcd");
		s.prepend("fuck");
		EXPECT_EQ(s.length(), 9);
		EXPECT_STREQ(s.c_str(), "fuckeabcd");
	}

	{
		std::string a = "abcdefghierdfzsdzsczcsdfzds";
		CVString s{ a.c_str() };
		s.prepend("e");
		EXPECT_EQ(s.length(), a.size() + 1);
		EXPECT_STREQ(s.c_str(), "eabcdefghierdfzsdzsczcsdfzds");
		s.prepend("fuck");
		EXPECT_EQ(s.length(), a.size() + 5);
		EXPECT_STREQ(s.c_str(), "fuckeabcdefghierdfzsdzsczcsdfzds");
	}

	{
		std::string a = "abcdefghierdfzsdzsczcs";
		CVString s{ a.c_str() };
		EXPECT_FALSE(s.is_long());
		s.prepend("e");
		EXPECT_EQ(s.length(), a.size() + 1);
		EXPECT_STREQ(s.c_str(), "eabcdefghierdfzsdzsczcs");
		EXPECT_FALSE(s.is_long());
		s.prepend("fuck");
		EXPECT_EQ(s.length(), a.size() + 5);
		EXPECT_STREQ(s.c_str(), "fuckeabcdefghierdfzsdzsczcs");
		EXPECT_TRUE(s.is_long());
	}
}
TEST(String, append)
{
	{
		CVString s;
		s.append("a");
		EXPECT_FALSE(s.empty());
		EXPECT_NE(s.length(), 0);
		EXPECT_STREQ(s.c_str(), "a");
	}

	{
		CVString s{ "abcd" };
		s.append("e");
		EXPECT_EQ(s.length(), 5);
		EXPECT_STREQ(s.c_str(), "abcde");
		s.append("fuck");
		EXPECT_EQ(s.length(), 9);
		EXPECT_STREQ(s.c_str(), "abcdefuck");
	}

	{
		std::string a = "abcdefghierdfzsdzsczcsdfzds";
		CVString s{ a.c_str() };
		s.append("e");
		EXPECT_EQ(s.length(), a.size() + 1);
		EXPECT_STREQ(s.c_str(), "abcdefghierdfzsdzsczcsdfzdse");
		s.append("fuck");
		EXPECT_EQ(s.length(), a.size() + 5);
		EXPECT_STREQ(s.c_str(), "abcdefghierdfzsdzsczcsdfzdsefuck");
	}

	{
		std::string a = "abcdefghierdfzsdzsczcs";
		CVString s{ a.c_str() };
		EXPECT_FALSE(s.is_long());
		s.append("e");
		EXPECT_EQ(s.length(), a.size() + 1);
		EXPECT_STREQ(s.c_str(), "abcdefghierdfzsdzsczcse");
		EXPECT_FALSE(s.is_long());
		s.append("fuck");
		EXPECT_EQ(s.length(), a.size() + 5);
		EXPECT_STREQ(s.c_str(), "abcdefghierdfzsdzsczcsefuck");
		EXPECT_TRUE(s.is_long());
	}
}
TEST(String, pop_front)
{
	{
		CVString s{"a"};
		s.pop_front();
		EXPECT_TRUE(s.empty());
		EXPECT_STREQ(s.c_str(), "");
#ifndef NDEBUG
		EXPECT_DEATH(s.pop_front(), "");
#endif // !NDEBUG
	}

	{
		CVString s{ "abcd" };
		s.pop_front();
		EXPECT_EQ(s.length(), 3);
		EXPECT_STREQ(s.c_str(), "bcd");
	}

	{
		std::string a = "abcdefghierdfzsdzsczcsdfzds";
		CVString s{ a.c_str() };
		s.pop_front();
		EXPECT_TRUE(s.is_long());
		EXPECT_EQ(s.length(), a.size() - 1);
		EXPECT_STREQ(s.c_str(), "bcdefghierdfzsdzsczcsdfzds");
	}

	{
		std::string a = "abcdefghierdfzsdzsczcsee";
		CVString s{ a.c_str() };
		EXPECT_TRUE(s.is_long());
		s.pop_front();
		EXPECT_EQ(s.length(), a.size() - 1);
		EXPECT_STREQ(s.c_str(), "bcdefghierdfzsdzsczcsee");
		EXPECT_TRUE(s.is_long());
		s.pop_front();
		EXPECT_EQ(s.length(), a.size() - 2);
		EXPECT_STREQ(s.c_str(), "cdefghierdfzsdzsczcsee");
		EXPECT_TRUE(s.is_long());
	}
}