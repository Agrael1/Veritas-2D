#include <catch2/catch_test_macros.hpp>

extern "C" {
#define HMAP_T int
#include <VHashMapT.h>

#define HMAP_T String
#define IsClass
#include <VHashMapT.h>

#include <VHashMapT.c>
}

class CVHMap
{
public:
    CVHMap(uint16_t r)
    {
        ::HashMap_int_ctor(&obj, r);
    }
    ~CVHMap()
    {
        ::HashMap_int_dtor(&obj);
    }

public:
    size_t count() const
    {
        return obj->size;
    }
    size_t buckets() const
    {
        return obj->bucket_cnt;
    }
    void push(std::string_view key, int i, bool* err = nullptr)
    {
        auto* x = ::emplace_HashMap_int(&obj, { key.data(), key.size() });
        if (err)
            *err = x == nullptr;
        if (x)
            *x = i;
    }
    void erase(std::string_view key)
    {
        ::erase_HashMap_int(&obj, { key.data(), key.size() });
    }
    int* find(std::string_view key)
    {
        return ::find_HashMap_int(&obj, { key.data(), key.size() });
    }
    void clear()
    {
        ::clear_HashMap_int(&obj);
    }
    void PrintTable()
    {
#ifndef NDEBUG
        int maxlen = 0;
        int sumcnt = 0;

        printf("------------HASH TABLE--------------\n");
        for (int i = 0; i < buckets(); i++) {
            printf("%i:", i);
            int cnt = 0;
            auto* ptr = obj->bucket[i];
            while (ptr != NULL) {
                printf(" (%s,%d)", c_str(&ptr->key), ptr->value);
                cnt++;
                ptr = ptr->next;
            }
            printf("\n");

            if (cnt > maxlen)
                maxlen = cnt;
            sumcnt += cnt;
        }

        printf("------------------------------------\n");
        printf("Items count %i   The longest list %i\n", sumcnt, maxlen);
        printf("------------------------------------\n");
#endif // !NDEBUG
    }

private:
    HashMap(int) obj;
};

TEST_CASE("hash_map_creation")
{
    {
        CVHMap s(16);
        REQUIRE(s.count() == 0);
        REQUIRE(s.buckets() == 16);
        s.push("Hi", 15);
        REQUIRE(s.count() == 1);
        REQUIRE(*s.find("Hi") == 15);

        s.PrintTable();
    }
}

TEST_CASE("hash_map_insert")
{
    {
        CVHMap s(19);

        s.push("korgon", 18);
        s.push("zlaty bazant", 20);
        s.push("gambrinus", 19);
        s.push("starobrno", 17);
        s.push("plzen", 25);
        s.push("velvet", 28);
        s.push("kelt", 30);
        s.push("kofola", 16);

        REQUIRE(s.count() == 8);
        s.PrintTable();

        REQUIRE_FALSE(s.find("starobrno") == nullptr);

        printf("\n[TEST06] Let's change korgon price to 12\n");

        bool x = false;
        s.push("korgon", 12, &x);
        REQUIRE(x == true);
        s.PrintTable();
        REQUIRE(s.find("korgon") != nullptr);

        REQUIRE(s.find("staroabrno") == nullptr);

        printf("\n[TEST09] Lets delete gambrinus and kofola\n");
        s.erase("gambrinus");
        s.erase("kofola");
        s.PrintTable();
        REQUIRE(s.count() == 6);

        printf("\n[TEST10] Let's delete whole table\n");
        s.clear();
        s.PrintTable();
        REQUIRE(s.count() == 0);
        REQUIRE(s.find("starobrno") == nullptr);

        printf("\n[TEST12] And insert velvet again\n");
        s.push("velvet", 28);
        s.PrintTable();
        REQUIRE(s.count() == 1);
        s.erase("velvet");
        REQUIRE(s.count() == 0);
    }
}
