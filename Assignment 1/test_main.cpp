#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "PlayList.h"
#include <stdexcept>

TEST_SUITE_BEGIN("PlayList implementations");

bool operator==(const PlayList& lhs, const PlayList& rhs) {
    const unsigned size = lhs.size();
    if (size != rhs.size()) {
        return false;
    }
    for (unsigned i = 0; i < size; i++) {
        if ((lhs.get(i).length() != rhs.get(i).length())
            /*|| (lhs.get(i).artist() != rhs.get(i).artist())
            || (lhs.get(i).name() != rhs.get(i).name())*/) {
            return false;
        }
    }
    return true;
}

bool operator!=(const PlayList& lhs, const PlayList& rhs) { return !(lhs == rhs); }

TEST_CASE("Empty lists with no insertions") {
    PlayList a, b;
    REQUIRE(a.size() == 0);
    REQUIRE(b.size() == 0);
    REQUIRE(a == b);
    {
        INFO("Get and remove");
        CHECK_THROWS_AS(a.get(-1), const std::out_of_range&);
        CHECK_THROWS_AS(a.get(0), const std::out_of_range&);
        CHECK_THROWS_AS(a.get(1), const std::out_of_range&);
        CHECK_THROWS_AS(b.get(-1), const std::out_of_range&);
        CHECK_THROWS_AS(b.get(0), const std::out_of_range&);
        CHECK_THROWS_AS(b.get(1), const std::out_of_range&);
        CHECK_THROWS_AS(a.remove(-1), const std::out_of_range&);
        CHECK_THROWS_AS(a.remove(0), const std::out_of_range&);
        CHECK_THROWS_AS(a.remove(1), const std::out_of_range&);
        CHECK_THROWS_AS(b.remove(-1), const std::out_of_range&);
        CHECK_THROWS_AS(b.remove(0), const std::out_of_range&);
        CHECK_THROWS_AS(b.remove(1), const std::out_of_range&);
    }
    {
        INFO("Swap");
        REQUIRE_NOTHROW(a.swap(b));
        CHECK_THROWS_AS(a.swap(0, 1), const std::out_of_range&);
        CHECK_THROWS_AS(a.swap(1, 0), const std::out_of_range&);
        CHECK_THROWS_AS(b.swap(0, 0), const std::out_of_range&);
        CHECK_THROWS_AS(b.swap(1, 1), const std::out_of_range&);
        REQUIRE(a.size() == 0);
        REQUIRE(b.size() == 0);
        CHECK(a == b);
        {
            CHECK_THROWS_AS(a.get(-1), const std::out_of_range&);
            CHECK_THROWS_AS(a.get(0), const std::out_of_range&);
            CHECK_THROWS_AS(a.get(1), const std::out_of_range&);
            CHECK_THROWS_AS(b.get(-1), const std::out_of_range&);
            CHECK_THROWS_AS(b.get(0), const std::out_of_range&);
            CHECK_THROWS_AS(b.get(1), const std::out_of_range&);
            CHECK_THROWS_AS(a.remove(-1), const std::out_of_range&);
            CHECK_THROWS_AS(a.remove(0), const std::out_of_range&);
            CHECK_THROWS_AS(a.remove(1), const std::out_of_range&);
            CHECK_THROWS_AS(b.remove(-1), const std::out_of_range&);
            CHECK_THROWS_AS(b.remove(0), const std::out_of_range&);
            CHECK_THROWS_AS(b.remove(1), const std::out_of_range&);
        }
    }
    {
        INFO("Copy assign");
        a = b;
        REQUIRE(a.size() == 0);
        REQUIRE(b.size() == 0);
        CHECK(a == b);
        {
            CHECK_THROWS_AS(a.get(-1), const std::out_of_range&);
            CHECK_THROWS_AS(a.get(0), const std::out_of_range&);
            CHECK_THROWS_AS(a.get(1), const std::out_of_range&);
            CHECK_THROWS_AS(b.get(-1), const std::out_of_range&);
            CHECK_THROWS_AS(b.get(0), const std::out_of_range&);
            CHECK_THROWS_AS(b.get(1), const std::out_of_range&);
            CHECK_THROWS_AS(a.remove(-1), const std::out_of_range&);
            CHECK_THROWS_AS(a.remove(0), const std::out_of_range&);
            CHECK_THROWS_AS(a.remove(1), const std::out_of_range&);
            CHECK_THROWS_AS(b.remove(-1), const std::out_of_range&);
            CHECK_THROWS_AS(b.remove(0), const std::out_of_range&);
            CHECK_THROWS_AS(b.remove(1), const std::out_of_range&);
        }
        REQUIRE_NOTHROW(a.swap(b));
        b = b;
        REQUIRE(a.size() == 0);
        REQUIRE(b.size() == 0);
        CHECK(a == b);
        {
            CHECK_THROWS_AS(a.get(-1), const std::out_of_range&);
            CHECK_THROWS_AS(a.get(0), const std::out_of_range&);
            CHECK_THROWS_AS(a.get(1), const std::out_of_range&);
            CHECK_THROWS_AS(b.get(-1), const std::out_of_range&);
            CHECK_THROWS_AS(b.get(0), const std::out_of_range&);
            CHECK_THROWS_AS(b.get(1), const std::out_of_range&);
            CHECK_THROWS_AS(a.remove(-1), const std::out_of_range&);
            CHECK_THROWS_AS(a.remove(0), const std::out_of_range&);
            CHECK_THROWS_AS(a.remove(1), const std::out_of_range&);
            CHECK_THROWS_AS(b.remove(-1), const std::out_of_range&);
            CHECK_THROWS_AS(b.remove(0), const std::out_of_range&);
            CHECK_THROWS_AS(b.remove(1), const std::out_of_range&);
        }
        REQUIRE_NOTHROW(a.swap(b));
        b = a;
        REQUIRE(a.size() == 0);
        REQUIRE(b.size() == 0);
        CHECK(a == b);
        {
            CHECK_THROWS_AS(a.get(-1), const std::out_of_range&);
            CHECK_THROWS_AS(a.get(0), const std::out_of_range&);
            CHECK_THROWS_AS(a.get(1), const std::out_of_range&);
            CHECK_THROWS_AS(b.get(-1), const std::out_of_range&);
            CHECK_THROWS_AS(b.get(0), const std::out_of_range&);
            CHECK_THROWS_AS(b.get(1), const std::out_of_range&);
            CHECK_THROWS_AS(a.remove(-1), const std::out_of_range&);
            CHECK_THROWS_AS(a.remove(0), const std::out_of_range&);
            CHECK_THROWS_AS(a.remove(1), const std::out_of_range&);
            CHECK_THROWS_AS(b.remove(-1), const std::out_of_range&);
            CHECK_THROWS_AS(b.remove(0), const std::out_of_range&);
            CHECK_THROWS_AS(b.remove(1), const std::out_of_range&);
        }
        REQUIRE_NOTHROW(a.swap(b));
    }
    {
        INFO("Copy construct");
        PlayList pl(a);
        CHECK(a == b);
        REQUIRE(pl.size() == 0);
        CHECK_THROWS_AS(pl.get(-1), const std::out_of_range&);
        CHECK_THROWS_AS(pl.get(0), const std::out_of_range&);
        CHECK_THROWS_AS(pl.get(1), const std::out_of_range&);
        CHECK_THROWS_AS(pl.remove(-1), const std::out_of_range&);
        CHECK_THROWS_AS(pl.remove(0), const std::out_of_range&);
        CHECK_THROWS_AS(pl.remove(1), const std::out_of_range&);
        CHECK_NOTHROW(a.swap(pl));
    }
}

TEST_CASE("Full tests") {
    PlayList a, b;
    CHECK(a == b);
    {
        INFO("Insert");
        {
            INFO("Out of range");
            CHECK_THROWS(a.insert({ "out of", "bounds", 1 }, 1));
            CHECK_THROWS(a.insert({ "out of", "bounds", -1 }, -1));
            CHECK_THROWS(a.insert({ "out of", "bounds", -1 }, -1));
            CHECK_THROWS(a.insert({ "out of", "bounds", -1 }, -1));
            {
                INFO("Invalid songs");
                WARN_THROWS(Song("", "invalid", 1));
                WARN_THROWS(Song("invalid", "", 1));
                WARN_THROWS(Song("invalid", "invalid", 0));
            }
        }
        CHECK_NOTHROW(a.insert({ "n1", "a1", 1 }, 0));
        REQUIRE(a.size() == 1);
        CHECK(a != b);
        CHECK_NOTHROW(a.insert({ "n11", "a11", 11 }, 0));
        CHECK_NOTHROW(a.insert({ "n12", "a12", 12 }, 0));
        CHECK_NOTHROW(a.insert({ "n13", "a13", 13 }, 1));
        CHECK_NOTHROW(a.insert({ "n14", "a14", 14 }, 1)); // a = 12 14 13 11 1
        REQUIRE(a.size() == 5);
        CHECK_NOTHROW(b.insert({ "n2", "a2", 2 }, 0));
        CHECK_NOTHROW(b.insert({ "n22", "a22", 22 }, 1)); // b = 2 22
        REQUIRE(b.size() == 2);
        {
            INFO("Get and remove");
            CHECK_THROWS(a.get(-1));
            CHECK_THROWS(a.remove(-1));
            CHECK(a.get(0).length() == 12);
            CHECK(a.get(1).length() == 14);
            CHECK(a.get(2).length() == 13);
            CHECK(a.get(3).length() == 11);
            CHECK(a.remove(4).name() == a.get(4).name());
            CHECK_THROWS(a.get(5));
            CHECK_THROWS(a.remove(5));
            CHECK_THROWS(a.get(4));
            CHECK_THROWS(a.remove(4));
            CHECK(a.remove(2).artist() == a.get(2).artist()); // a = 12 14 11
            CHECK_THROWS(a.get(3));
            CHECK_THROWS(a.remove(3));
            CHECK(a.get(2).length() == 11);
            CHECK_THROWS(b.get(-1));
            CHECK_THROWS(b.remove(-1));
            CHECK(b.get(0).name() == "n2");
            CHECK(b.get(1).artist() == "a22");
            CHECK_THROWS(b.get(2));
            CHECK_THROWS(b.get(3));
        }
        {
            INFO("Operators == and !=");
            CHECK(a != b);
            CHECK(a == a);
            CHECK(b == b);
        }
    }
    {
        INFO("Copy");
        PlayList c(a); // c = a = 12 14 11
        {
            INFO("Copy construct");
            CHECK(a == c);
            CHECK_THROWS(c.get(-1));
            CHECK(c.get(0).name() == a.get(0).name());
            CHECK(c.get(1).artist() == a.get(1).artist());
            CHECK(c.get(2).artist() == a.get(2).artist());
            CHECK(c.get(2).length() == a.get(2).length());
            CHECK(c.get(2).artist() == "a11");
            CHECK_THROWS(c.get(3));
            CHECK(c == a);
        }
        {
            INFO("Copy assign");
            a = c;
            CHECK_NOTHROW(c.remove(2));
            CHECK(c != a);
            CHECK_NOTHROW(c = a);
            CHECK(a == c);
            c.remove(0);
            CHECK(a != c);
            c = a;
            CHECK_NOTHROW(a.remove(2));
            CHECK(a != c);
            CHECK_NOTHROW(a = c);
            CHECK(c == a);
            PlayList d;
            REQUIRE(d.size() == 0);
            d = c;
            CHECK(d == c);
            CHECK(d == a);
            REQUIRE(b != c);
            d = b;
            CHECK(d == b);
            CHECK(d != c);
            CHECK(d != a);
            {
                INFO("Self assign");
                a = a;
                CHECK(a == a);
                CHECK(a == c);
            }
        }
    }
    {
        INFO("Swap");
        a.insert({ "n10", "a10", 10 }, 3);
        PlayList c(a); // c = a = 12 14 11 10
        {
            INFO("Internal swap");
            {
                INFO("First 2");
                a.swap(0, 1); // a = 14 12 11 10
                CHECK(a != c);
                CHECK(a.get(0).length() == 14);
                CHECK(a.get(1).length() == 12);
                CHECK(a.get(2).length() == 11);
                CHECK(a.get(3).length() == 10);
                CHECK(c.get(0).length() == 12);
                CHECK(c.get(1).length() == 14);
                CHECK(c.get(2).length() == 11);
                CHECK(c.get(3).length() == 10);
                c.swap(1, 0);
                CHECK(c == a); // c = a = 14 12 11 10
            }
            {
                INFO("Last 2");
                a.swap(3, 2); // a = 14 12 10 11
                CHECK(a != c);
                CHECK(a.get(0).length() == 14);
                CHECK(a.get(1).length() == 12);
                CHECK(a.get(2).length() == 10);
                CHECK(a.get(3).length() == 11);
                CHECK(c.get(0).length() == 14);
                CHECK(c.get(1).length() == 12);
                CHECK(c.get(2).length() == 11);
                CHECK(c.get(3).length() == 10);
                c.swap(2, 3);
                CHECK(c == a); // c = a = 14 12 10 11
            }
            {
                INFO("2 ends");
                a.swap(0, 3); // a = 11 12 10 14
                CHECK(a != c);
                CHECK(a.get(0).length() == 11);
                CHECK(a.get(1).length() == 12);
                CHECK(a.get(2).length() == 10);
                CHECK(a.get(3).length() == 14);
                CHECK(c.get(0).length() == 14);
                CHECK(c.get(1).length() == 12);
                CHECK(c.get(2).length() == 10);
                CHECK(c.get(3).length() == 11);
                c.swap(3, 0);
                CHECK(c == a); // c = a = 11 12 10 14
            }
            {
                INFO("Middle 2");
                a.swap(1, 2); // a = 11 10 12 14
                CHECK(a != c);
                CHECK(a.get(0).length() == 11);
                CHECK(a.get(1).length() == 10);
                CHECK(a.get(2).length() == 12);
                CHECK(a.get(3).length() == 14);
                CHECK(c.get(0).length() == 11);
                CHECK(c.get(1).length() == 12);
                CHECK(c.get(2).length() == 10);
                CHECK(c.get(3).length() == 14);
                c.swap(2, 1);
                CHECK(c == a); // c = a = 11 10 12 14
            }
            {
                INFO("1 last, 1 middle");
                a.swap(3, 1); // a = 11 14 12 10
                CHECK(a != c);
                CHECK(a.get(0).length() == 11);
                CHECK(a.get(1).length() == 14);
                CHECK(a.get(2).length() == 12);
                CHECK(a.get(3).length() == 10);
                CHECK(c.get(0).length() == 11);
                CHECK(c.get(1).length() == 10);
                CHECK(c.get(2).length() == 12);
                CHECK(c.get(3).length() == 14);
                c.swap(1, 3);
                CHECK(c == a); // c = a = 11 14 12 10
            }
            {
                INFO("1 first, 1 middle");
                a.swap(0, 2); // a = 12 14 11 10
                CHECK(a != c);
                CHECK(a.get(0).length() == 12);
                CHECK(a.get(1).length() == 14);
                CHECK(a.get(2).length() == 11);
                CHECK(a.get(3).length() == 10);
                CHECK(c.get(0).length() == 11);
                CHECK(c.get(1).length() == 14);
                CHECK(c.get(2).length() == 12);
                CHECK(c.get(3).length() == 10);
                c.swap(2, 0);
                CHECK(c == a); // c = a = 12 14 11 10
            }                                    
            {
                INFO("no-op swaps");
                CHECK_THROWS(a.swap(3, 4));
                CHECK_THROWS(a.swap(4, 2));
                CHECK_THROWS(a.swap(4, 4));
                CHECK_NOTHROW(a.swap(0, 0));
                CHECK(c == a);
                CHECK_NOTHROW(a.swap(1, 1));
                CHECK(a == c);
                CHECK_NOTHROW(a.swap(3, 3));
                CHECK(a == c);
            }
        }
        {
            INFO("Container swap");
            REQUIRE_NOTHROW(c.swap(a));
            CHECK(a == c);
            a.remove(0); // a = 14 11 10
            REQUIRE(a.size() == 3);
            REQUIRE(c.size() == 4); // c = 12 14 11 10
            REQUIRE_NOTHROW(c.swap(a)); // a[4], c[3]
            CHECK(c != a);
            CHECK(a.size() == 4);
            CHECK(c.size() == 3);
            CHECK(a.get(0).length() == 12);
            CHECK(c.get(0).length() == 14);
            REQUIRE_NOTHROW(a.swap(c)); // a[3], c[4]
            CHECK(a != c);
            CHECK(a.size() == 3);
            CHECK(c.size() == 4);
            CHECK(a.get(1).length() == 11);
            CHECK(c.get(1).length() == 14);
            REQUIRE_NOTHROW(a.swap(a)); // Itself.
            CHECK(a == a);
            CHECK(c == c);
        }
    }
}

TEST_CASE("Stress tests" * doctest::timeout(2)) {
    PlayList rev;
    const int BIG_NUM = 1000;
    {
        INFO("Insert at front"); // BIG_NUM ... 1
        for (int i = 1; i < BIG_NUM; i++) {
            REQUIRE_NOTHROW(rev.insert({ "s", "t", i }, 0));
            REQUIRE(rev.get(0).length() == i);
            REQUIRE(rev.size() == i);
        }
    }
    const unsigned size = rev.size();
    PlayList ord = rev;
    {
        INFO("Copy");
        const PlayList revCopy = rev;
        REQUIRE(ord == revCopy);
        rev = ord;
        REQUIRE(ord == rev);
    }
    {
        INFO("Swap");
        {
            INFO("Reverse order"); // 1 ... size
            const unsigned half = size / 2;
            for (unsigned i = 0; i < half; i++) {
                WARN_NOTHROW(ord.swap(i, size - i - 1));
            }
            for (unsigned i = 0; i < size; i++) {
                REQUIRE(ord.get(i).length() == i + 1);
            }
        }
        REQUIRE(size == ord.size());
        CHECK(size == rev.size());
        CHECK(ord != rev);
    }
    {
        INFO("Remove and insert");
        {
            INFO("push_back( pop_front ) == copy container"); // 1 ... size
            INFO("Insert at back, remove at front");
            PlayList ord1, ord2 = ord;
            REQUIRE(ord2 == ord);
            REQUIRE(ord1 != ord2);
            for (unsigned i = 0; i < size; i++) {
                WARN_NOTHROW(ord1.insert(ord2.remove(0), i));
            }
            CHECK(ord2.size() == 0);
            CHECK(ord2 == PlayList());
            CHECK(ord1 == ord);
        }
        {
            INFO("push_front( pop_back ) == copy container"); // 1 ... size
            INFO("Insert at front, remove at back");
            PlayList ord1, ord2(ord);
            REQUIRE(ord1 != ord2);
            for (int i = size - 1; i >= 0; i--) {
                WARN_NOTHROW(ord1.insert(ord2.remove(i), 0));
            }
            CHECK(ord2 == PlayList());
            CHECK(ord1 == ord);
        }
        {
            INFO("push_back( pop_back ) == reverse container"); // size ... 1
            INFO("Insert at back, remove at back");
            PlayList rev2, ord2(ord);
            REQUIRE(rev != rev2);
            for (int i = size - 1, j = 0; i >= 0; i--, j++) {
                WARN_NOTHROW(rev2.insert(ord2.remove(i), j));
            }
            CHECK(ord2 == PlayList());
            CHECK(rev2 == rev);
        }
        {
            INFO("push_front( pop_front ) == reverse container"); // size ... 1
            INFO("Insert at front, remove at front");
            PlayList rev2, ord2(ord);
            REQUIRE(rev != rev2);
            for (unsigned i = 0; i < size; i++) {
                WARN_NOTHROW(rev2.insert(ord2.remove(0), 0));
            }
            CHECK(ord2 == PlayList());
            CHECK(rev2 == rev);
        }
    }
}

TEST_SUITE_END();