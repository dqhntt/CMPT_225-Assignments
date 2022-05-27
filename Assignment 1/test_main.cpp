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
        if ((lhs.get(i).getLength() != rhs.get(i).getLength())
            /*|| (lhs.get(i).getArtist() != rhs.get(i).getArtist())
            || (lhs.get(i).getName() != rhs.get(i).getName())*/) {
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
    SUBCASE("Get and remove") {
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
    SUBCASE("Swap") {
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
    SUBCASE("Copy assign") {
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
    SUBCASE("Copy construct") {
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
    SUBCASE("Insert") {
        a.insert(Song("name1", "artist1", 1), 0);
        REQUIRE(a.size() == 1);
        CHECK(a != b);
        a.insert(Song("name11", "artist11", 11), 0);
        a.insert(Song("name12", "artist12", 12), 0);
        a.insert(Song("name13", "artist13", 13), 1);
        a.insert(Song("name14", "artist14", 14), 1); // a = 12 14 13 11 1
        REQUIRE(a.size() == 5);
        b.insert(Song("name2", "artist2", 2), 0);
        b.insert(Song("name22", "artist22", 22), 1); // b = 2 22
        REQUIRE(b.size() == 2);
        SUBCASE("Get and remove") {
            CHECK_THROWS(a.get(-1));
            CHECK_THROWS(a.remove(-1));
            CHECK(a.get(0).getLength() == 12);
            CHECK(a.get(1).getLength() == 14);
            CHECK(a.get(2).getLength() == 13);
            CHECK(a.get(3).getLength() == 11);
            CHECK(a.remove(4).getName() == a.get(4).getName());
            CHECK_THROWS(a.get(5));
            CHECK_THROWS(a.remove(5));
            CHECK_THROWS(a.get(4));
            CHECK_THROWS(a.remove(4));
            CHECK(a.remove(2).getArtist() == a.get(2).getArtist()); // a = 12 14 11
            CHECK_THROWS(a.get(3));
            CHECK_THROWS(a.remove(3));
            CHECK(a.get(2).getLength() == 11);
            CHECK_THROWS(b.get(-1));
            CHECK_THROWS(b.remove(-1));
            CHECK(b.get(0).getName() == "name2");
            CHECK(b.get(1).getArtist() == "artist22");
            CHECK_THROWS(b.get(2));
            CHECK_THROWS(b.get(3));
        }
        SUBCASE("Operators == and !=") {
            CHECK(a != b);
            CHECK(a == a);
            CHECK(b == b);
        }
    }
    SUBCASE("Copy") {
        PlayList c(a); // c = a = 12 14 11
        SUBCASE("Copy construct") {
            CHECK(a == c);
            CHECK_THROWS(c.get(-1));
            CHECK(c.get(0).getName() == a.get(0).getName());
            CHECK(c.get(1).getArtist() == a.get(1).getArtist());
            CHECK(c.get(2).getArtist() == a.get(2).getArtist());
            CHECK(c.get(2).getLength() == a.get(2).getLength());
            CHECK(c.get(2).getName() == "artist11");
            CHECK_THROWS(c.get(3));
            CHECK(c == a);
        }
        SUBCASE("Copy assign") {
            c = a;
            CHECK(c == a);
            a = c;
            CHECK(a == c);
            SUBCASE("Self assign") {
                a = a;
                CHECK(a == a);
                CHECK(a == c);
            }
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
        }
    }
    SUBCASE("Swap") {
        PlayList c(a); // c = a = 12 14 11
        SUBCASE("Internal swap") {
            SUBCASE("First 2") {
                a.swap(0, 1); // a = 14 12 11
                CHECK(a != c);
                CHECK(a.get(0).getLength() == 14);
                CHECK(a.get(1).getLength() == 12);
                CHECK(a.get(2).getLength() == 11);
                CHECK(c.get(0).getLength() == 12);
                CHECK(c.get(1).getLength() == 14);
                CHECK(c.get(2).getLength() == 11);
                c.swap(1, 0);
                CHECK(c == a); // c = a = 14 12 11
            }
            SUBCASE("Last 2") {
                a.swap(2, 1); // a = 14 11 12
                CHECK(a != c);
                CHECK(a.get(0).getLength() == 14);
                CHECK(a.get(1).getLength() == 11);
                CHECK(a.get(2).getLength() == 12);
                CHECK(c.get(0).getLength() == 14);
                CHECK(c.get(1).getLength() == 12);
                CHECK(c.get(2).getLength() == 11);
                c.swap(1, 2);
                CHECK(c == a); // c = a = 14 11 12
            }
            SUBCASE("2 ends") {
                a.swap(0, 2); // a = 12 11 14
                CHECK(a != c);
                CHECK(a.get(0).getLength() == 12);
                CHECK(a.get(1).getLength() == 11);
                CHECK(a.get(2).getLength() == 14);
                CHECK(c.get(0).getLength() == 14);
                CHECK(c.get(1).getLength() == 11);
                CHECK(c.get(2).getLength() == 12);
                c.swap(2, 0);
                CHECK(c == a); // c = a = 12 11 14
            }
        }
        SUBCASE("Container swap") {
            REQUIRE_NOTHROW(c.swap(a));
            CHECK(a == c);
            REQUIRE_NOTHROW(c.swap(a));
            CHECK(c == a);
            REQUIRE_NOTHROW(a.swap(c));
            CHECK(a == c);
            REQUIRE_NOTHROW(a.swap(a));
            CHECK(a == a);
            CHECK(c == a);
        }
    }
}

TEST_CASE("Stress tests") {
    PlayList rev;
    SUBCASE("Insert at front") { // INT_MAX ... 1
        for (int i = 1; i < INT_MAX; i++) {
            REQUIRE_NOTHROW(rev.insert(Song("s", "t", i), 0));
            REQUIRE(rev.get(0).getLength() == i);
            REQUIRE(rev.size() == i);
        }
    }
    const unsigned size = rev.size();
    PlayList ord = rev;
    SUBCASE("Copy") {
        REQUIRE(ord == PlayList(rev));
        rev = ord;
        REQUIRE(ord == rev);
    }
    SUBCASE("Swap") {
        SUBCASE("Reverse order") { // 1 ... size
            const unsigned half = size / 2;
            for (unsigned i = 0; i < half; i++) {
                WARN_NOTHROW(ord.swap(i, size - i - 1));
            }
            for (unsigned i = 0; i < size; i++) {
                REQUIRE(ord.get(i).getLength() == i + 1);
            }
        }
        REQUIRE(size == ord.size());
        CHECK(size == rev.size());
        CHECK(ord != rev);
    }
    SUBCASE("Remove and insert") {
        SUBCASE("push_back( pop_front ) == copy container") { // 1 ... size
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
        SUBCASE("push_front( pop_back ) == copy container") { // 1 ... size
            INFO("Insert at front, remove at back");
            PlayList ord1, ord2(ord);
            REQUIRE(ord1 != ord2);
            for (int i = size - 1; i >= 0; i--) {
                WARN_NOTHROW(ord1.insert(ord2.remove(i), 0));
            }
            CHECK(ord2 == PlayList());
            CHECK(ord1 == ord);
        }
        SUBCASE("push_back( pop_back ) == reverse container") { // size ... 1
            INFO("Insert at back, remove at back");
            PlayList rev2, ord2(ord);
            REQUIRE(rev != rev2);
            for (int i = size - 1, j = 0; i >= 0; i--, j++) {
                WARN_NOTHROW(rev2.insert(ord2.remove(i), j));
            }
            CHECK(ord2 == PlayList());
            CHECK(rev2 == rev);
        }
        SUBCASE("push_front( pop_front ) == reverse container") { // size ... 1
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
