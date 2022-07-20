// a4test.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "AVLTree.h"
#include <type_traits>

////////// Mostly compile-time checks. ////////////////////////////////////////////////////////////

TEST_SUITE("Some function signatures meet requirements") {
    // Can't check all of them without C++17 and higher.
    TEST_CASE_TEMPLATE(
        "class AVLTreeNode", T, short, char, double, unsigned, long long, std::string) {
        using V = std::vector<T>;
        static_assert(std::is_same<T, decltype(AVLTreeNode<T, T>::key)>::value,
            "key should be a template type");
        static_assert(std::is_same<V, decltype(AVLTreeNode<T, V>::value)>::value,
            "value should be a template type");
        static_assert(std::is_same<AVLTreeNode<T, V>*, decltype(AVLTreeNode<T, V>::left)>::value,
            "left node must be a pointer");
        static_assert(std::is_same<AVLTreeNode<T, V>*, decltype(AVLTreeNode<T, V>::right)>::value,
            "right node must be a pointer");
        static_assert(std::is_same<AVLTreeNode<T, V>*, decltype(AVLTreeNode<T, V>::parent)>::value,
            "parent node must be a pointer");
        static_assert(std::is_integral<decltype(AVLTreeNode<T, T>::height)>::value,
            "height should be an unsigned integer");
        CHECK(std::is_unsigned<decltype(AVLTreeNode<T, T>::height)>::value);
        SUBCASE("Node has a proper constructor") {
            static_assert(std::is_constructible<AVLTreeNode<T, V>, T, V>::value,
                "AVLTreeNode needs a (Key, Value) constructor");
            const AVLTreeNode<T, T> emptyNode(T {}, T {});
            REQUIRE(emptyNode.key == T {});
            REQUIRE(emptyNode.value == T {});
            REQUIRE(emptyNode.left == nullptr);
            REQUIRE(emptyNode.right == nullptr);
            REQUIRE(emptyNode.parent == nullptr);
            REQUIRE(emptyNode.height == 0);
        }
    }
    TEST_CASE_TEMPLATE("class AVLTree", T, bool, time_t, wchar_t, float, std::size_t, std::string) {
        using V = std::vector<T>;
        SUBCASE("Tree has a default constructor") {
            static_assert(std::is_default_constructible<AVLTree<T, V>>::value,
                "AVLTree needs a default constructor");
            const AVLTree<T, T> emptyTree;
            REQUIRE(emptyTree.size() == 0);
            REQUIRE(emptyTree.getRoot() == nullptr);
        }
        SUBCASE("Tree has other special member functions") {
            static_assert(std::is_copy_constructible<AVLTree<T, T>>::value
                    && !std::is_trivially_copy_constructible<AVLTree<T, T>>::value,
                "AVLTree needs a user-defined copy constructor");
            static_assert(std::is_copy_assignable<AVLTree<T, T>>::value
                    && !std::is_trivially_copy_assignable<AVLTree<T, T>>::value,
                "AVLTree needs a user-defined copy assignment operator");
            static_assert(std::is_destructible<AVLTree<T, T>>::value
                    && !std::is_trivially_destructible<AVLTree<T, T>>::value,
                "AVLTree needs a user-defined destructor");
        }
        SUBCASE("Non-mutating AVLTree methods are const") {
            INFO("Checking with an empty tree");
            const AVLTree<T, V> tree;
            const std::vector<T> keys = tree.keys();
            const std::vector<V> values = tree.values();
            CHECK_THROWS(tree.search(T {}));
            CHECK(tree.size() == 0);
            CHECK(std::is_unsigned<decltype(tree.size())>::value);
        }
    }
}

////////// Helper functions. //////////////////////////////////////////////////////////////////////

#include "tools.h" // All tools should now be prepended with: tools::
#include <limits>
#include <string>

namespace debug {
template <class Tree>
bool isValidAVL(const Tree& tree) {
    if (!tools::isValidAVL(tree)) {
        std::cout << tree << std::endl;
        return false;
    }
    return true;
}
} // namespace debug
#ifdef NDEBUG
#define isValidAVL(tree) tools::isValidAVL(tree)
#else
#define isValidAVL(tree) debug::isValidAVL(tree)
#endif

// Magic number.
constexpr int NUM = 42;

// Random non-unique values of type T.
// Terrible implementation, don't use for anything else.
template <class T>
std::vector<T> randValues(int n) {
    static_assert(
        std::is_convertible<int, T>::value, "Conversion from int is needed for this function");
    std::vector<T> vec(n);
    // Make it nicer to look at.
    const int max = std::min(NUM, static_cast<int>(std::numeric_limits<T>::max()));
    const int min = std::max(0, static_cast<int>(std::numeric_limits<T>::min()));
    std::generate(vec.begin(), vec.end(), [max, min]() -> T { return rand() % max + min; });
    return vec;
}
template <>
std::vector<char> randValues(int n) {
    std::vector<char> vec(n);
    std::generate(vec.begin(), vec.end(), [] { return rand() % ('z' - 'a') + 'a'; });
    return vec;
}
template <>
std::vector<std::string> randValues(int n) {
    std::vector<std::string> vec(n);
    std::generate(vec.begin(), vec.end(), [] { return std::to_string(rand() % NUM); });
    return vec;
}

////////// Run-time checks. ///////////////////////////////////////////////////////////////////////

// https://visualgo.net/en/avl
TEST_SUITE("Nodes are inserted correctly") {
    using tools::makeTree;
    SCENARIO("Inserting sorted keys trigger single rotations") {
        // Empty tree.
        auto tree = makeTree({});
        REQUIRE(isValidAVL(tree));
        GIVEN("Sorted keys") {
            std::vector<int> keys(NUM);
            std::iota(keys.begin(), keys.end(), -5);
            WHEN("They are inserted to an empty tree") {
                THEN("Left rotations are performed") {
                    for (int k : keys) {
                        tree.insert(k, {});
                        REQUIRE(isValidAVL(tree));
                    }
                    CHECK(tree.size() == keys.size());
                }
            }
        }
        GIVEN("Reverse sorted keys") {
            std::vector<int> keys(NUM);
            std::iota(keys.rbegin(), keys.rend(), -5);
            WHEN("They are inserted to an empty tree") {
                THEN("Right rotations are performed") {
                    for (int k : keys) {
                        tree.insert(k, {});
                        REQUIRE(isValidAVL(tree));
                    }
                    CHECK(tree.size() == keys.size());
                }
            }
        }
    }
    SCENARIO("Inserting 2 non-overlapping sets of sorted keys may require double rotations") {
        // Empty tree.
        auto tree = makeTree({});
        REQUIRE(isValidAVL(tree));
        GIVEN("Keys: [1, 2, 3, 4, 5, 10, 9, 8, 7, 6]") {
            const std::vector<int> keys = { 1, 2, 3, 4, 5, 10, 9, 8, 7, 6 };
            WHEN("They are inserted to an empty tree") {
                THEN("Single and double rotations are performed") {
                    for (int k : keys) {
                        CHECK(tree.insert(k, {}));
                        REQUIRE(isValidAVL(tree));
                    }
                    CHECK(tree.size() == keys.size());
                }
            }
        }
        GIVEN("Keys: [10, 9, 8, 7, 6, 1, 2, 3, 4, 5]") {
            const std::vector<int> keys = { 10, 9, 8, 7, 6, 1, 2, 3, 4, 5 };
            WHEN("They are inserted to an empty tree") {
                THEN("Single and double rotations are performed") {
                    for (int k : keys) {
                        CHECK(tree.insert(k, {}));
                        REQUIRE(isValidAVL(tree));
                    }
                    CHECK(tree.size() == keys.size());
                }
            }
        }
        GIVEN("Keys: [5, 4, 3, 2, 1, 10, 9, 8, 7, 6]") {
            const std::vector<int> keys = { 5, 4, 3, 2, 1, 10, 9, 8, 7, 6 };
            WHEN("They are inserted to an empty tree") {
                THEN("Single and double rotations are performed") {
                    for (int k : keys) {
                        CHECK(tree.insert(k, {}));
                        REQUIRE(isValidAVL(tree));
                    }
                    CHECK(tree.size() == keys.size());
                }
            }
        }
        GIVEN("Keys: [6, 7, 8, 9, 10, 1, 2, 3, 4, 5]") {
            const std::vector<int> keys = { 6, 7, 8, 9, 10, 1, 2, 3, 4, 5 };
            WHEN("They are inserted to an empty tree") {
                THEN("Single and double rotations are performed") {
                    for (int k : keys) {
                        CHECK(tree.insert(k, {}));
                        REQUIRE(isValidAVL(tree));
                    }
                    CHECK(tree.size() == keys.size());
                }
            }
        }
    }
}

TEST_SUITE("Nodes are removed correctly") {
    using tools::makeTree;
    SCENARIO("Removed nodes have 0 children") {
        GIVEN("Tree: [1]") {
            auto tree = makeTree({ 1 });
            REQUIRE(isValidAVL(tree));
            WHEN("Its only node is removed") {
                CHECK(tree.remove(1));
                THEN("It's empty") {
                    REQUIRE(isValidAVL(tree));
                    REQUIRE(tree.size() == 0);
                }
            }
        }
        GIVEN("Tree: [4, 2, 6, 1, 3, 5, 7]") {
            auto tree = makeTree({ 4, 2, 6, 1, 3, 5, 7 });
            REQUIRE(isValidAVL(tree));
            WHEN("Keys 1 or 7 are removed") {
                CHECK(tree.remove(1));
                CHECK(tree.remove(7));
                THEN("No rotations are needed") { REQUIRE(isValidAVL(tree)); }
            }
            WHEN("Keys 3 or 5 are removed") {
                CHECK(tree.remove(3));
                CHECK(tree.remove(5));
                THEN("No rotations are needed") { REQUIRE(isValidAVL(tree)); }
            }
        }
        GIVEN("Tree: [6, 2, 10, 1, 4, 8, 11, 3, 5, 7, 9]") {
            auto tree = makeTree({ 6, 2, 10, 1, 4, 8, 11, 3, 5, 7, 9 });
            REQUIRE(isValidAVL(tree));
            WHEN("Keys 1 or 11 are removed") {
                CHECK(tree.remove(1));
                CHECK(tree.remove(11));
                THEN("Single rotations are performed") { REQUIRE(isValidAVL(tree)); }
            }
        }
        GIVEN("Tree: [5, 2, 8, 1, 4, 6, 9, 3, 7]") {
            auto tree = makeTree({ 5, 2, 8, 1, 4, 6, 9, 3, 7 });
            REQUIRE(isValidAVL(tree));
            WHEN("Keys 1 or 9 are removed") {
                CHECK(tree.remove(1));
                CHECK(tree.remove(9));
                THEN("Double rotations are performed") { REQUIRE(isValidAVL(tree)); }
            }
        }
        GIVEN("Tree: [13, 8, 18, 5, 11, 16, 20, 3, 7, 10, 12, 15, 17, 19, 2, 4, 6, 9, 14, 1]") {
            // https://cs.stackexchange.com/a/128270
            auto tree = makeTree(
                { 13, 8, 18, 5, 11, 16, 20, 3, 7, 10, 12, 15, 17, 19, 2, 4, 6, 9, 14, 1 });
            REQUIRE(isValidAVL(tree));
            WHEN("Key 19 is removed") {
                CHECK(tree.remove(19));
                THEN("Multiple rotations are performed") { REQUIRE(isValidAVL(tree)); }
            }
        }
    }
    SCENARIO("Removed nodes have 1 child") {
        GIVEN("Tree: [3, 2, 4, 1, 5]") {
            auto tree = makeTree({ 3, 2, 4, 1, 5 });
            REQUIRE(isValidAVL(tree));
            WHEN("Keys 2 or 4 are removed") {
                tree.remove(2);
                tree.remove(4);
                THEN("No rotations are needed") { REQUIRE(isValidAVL(tree)); }
            }
        }
        GIVEN("Tree: [3, 1, 5, 2, 4]") {
            auto tree = makeTree({ 3, 1, 5, 2, 4 });
            REQUIRE(isValidAVL(tree));
            WHEN("Keys 1 or 5 are removed") {
                tree.remove(1);
                tree.remove(5);
                THEN("No rotations are needed") { REQUIRE(isValidAVL(tree)); }
            }
        }
        GIVEN("Tree: [3, 1, 5, 2, 4, 7, 6]") {
            auto tree = makeTree({ 3, 1, 5, 2, 4, 7, 6 });
            REQUIRE(isValidAVL(tree));
            WHEN("Key 1 is removed") {
                tree.remove(1);
                THEN("Single rotations are performed") { REQUIRE(isValidAVL(tree)); }
            }
        }
        GIVEN("Tree: [5, 3, 10, 2, 4, 7, 12, 1, 6, 9, 11, 8]") {
            auto tree = makeTree({ 5, 3, 10, 2, 4, 7, 12, 1, 6, 9, 11, 8 });
            REQUIRE(isValidAVL(tree));
            WHEN("Key 2 is removed") {
                tree.remove(2);
                THEN("Double rotations are performed") { REQUIRE(isValidAVL(tree)); }
            }
            WHEN("Key 12 is removed") {
                tree.remove(12);
                THEN("Double rotations are performed") { REQUIRE(isValidAVL(tree)); }
            }
        }
    }
    SCENARIO("Removed nodes have 2 children") {
        GIVEN("Tree: [2, 1, 3]") {
            auto tree = makeTree({ 2, 1, 3 });
            REQUIRE(isValidAVL(tree));
            WHEN("Root 2 is removed") {
                tree.remove(2);
                THEN("No rotations are needed") { REQUIRE(isValidAVL(tree)); }
            }
        }
        GIVEN("Tree: [5, 2, 8, 1, 4, 6, 9, 3, 7]") {
            auto tree = makeTree({ 5, 2, 8, 1, 4, 6, 9, 3, 7 });
            REQUIRE(isValidAVL(tree));
            WHEN("Root 5 is removed") {
                tree.remove(5);
                THEN("No rotations are needed") { REQUIRE(isValidAVL(tree)); }
            }
        }
        GIVEN("Tree: [5, 3, 7, 2, 4, 6, 8, 1, 9]") {
            auto tree = makeTree({ 5, 3, 7, 2, 4, 6, 8, 1, 9 });
            REQUIRE(isValidAVL(tree));
            WHEN("Root 5 is removed") {
                tree.remove(5);
                THEN("Single rotations are performed") { REQUIRE(isValidAVL(tree)); }
            }
        }
        GIVEN("Tree: [5, 3, 7, 1, 4, 6, 9, 2, 8]") {
            auto tree = makeTree({ 5, 3, 7, 1, 4, 6, 9, 2, 8 });
            REQUIRE(isValidAVL(tree));
            WHEN("Root 5 is removed") {
                tree.remove(5);
                THEN("Double rotations are performed") { REQUIRE(isValidAVL(tree)); }
            }
        }
        GIVEN("Tree: [5, 3, 10, 1, 4, 8, 12, 2, 6, 9, 11, 7]") {
            auto tree = makeTree({ 5, 3, 10, 1, 4, 8, 12, 2, 6, 9, 11, 7 });
            REQUIRE(isValidAVL(tree));
            WHEN("Root 5 is removed") {
                tree.remove(5);
                THEN("Multiple rotations may be needed (if predecessor is used)") {
                    REQUIRE(isValidAVL(tree));
                }
            }
        }
        GIVEN("Tree: [8, 3, 10, 1, 5, 9, 12, 2, 4, 7, 11, 6]") {
            auto tree = makeTree({ 8, 3, 10, 1, 5, 9, 12, 2, 4, 7, 11, 6 });
            REQUIRE(isValidAVL(tree));
            WHEN("Root 8 is removed") {
                tree.remove(8);
                THEN("Multiple rotations may be needed (if successor is used)") {
                    REQUIRE(isValidAVL(tree));
                }
            }
        }
        GIVEN("Tree: [13, 5, 21, 3, 10, 16, 26, 1, 4, 8, 12, 15, 19, 24, 31, 2, 6, 9,"
              " 11, 14, 18, 20, 23, 25, 29, 32, 7, 17, 22, 28, 30, 33, 27]") {
            auto tree = makeTree({ 13, 5, 21, 3, 10, 16, 26, 1, 4, 8, 12, 15, 19, 24, 31, 2, 6, 9,
                11, 14, 18, 20, 23, 25, 29, 32, 7, 17, 22, 28, 30, 33, 27 });
            REQUIRE(isValidAVL(tree));
            WHEN("Key 5 is removed") {
                tree.remove(5);
                THEN("Multiple rotations may be needed (if predecessor is used)") {
                    REQUIRE(isValidAVL(tree));
                }
            }
        }
    }
}

SCENARIO("Insert and remove return bool correctly") {
    using tools::makeTree;
    GIVEN("Unique keys and an empty tree") {
        auto tree = makeTree({});         // Empty tree.
        std::vector<int> uniqueKeys(NUM); // Multiples of 5.
        for (int i = 0; i < uniqueKeys.size(); i++) {
            uniqueKeys[i] = i * 5 - 10;
        }
        std::random_shuffle(uniqueKeys.begin(), uniqueKeys.end());
        WHEN("They are inserted to the tree") {
            THEN("All inserts return true") {
                for (int n : uniqueKeys) {
                    REQUIRE(tree.insert(n, {}));
                }
                REQUIRE(isValidAVL(tree));
                REQUIRE(tree.size() == uniqueKeys.size());
                AND_WHEN("Those keys are inserted again") {
                    std::random_shuffle(uniqueKeys.begin(), uniqueKeys.end());
                    THEN("All inserts return false") {
                        for (int n : uniqueKeys) {
                            REQUIRE(!tree.insert(n, {}));
                        }
                        REQUIRE(isValidAVL(tree));
                        REQUIRE(tree.size() == uniqueKeys.size());
                    }
                }
                AND_WHEN("Those keys are removed from the tree") {
                    std::random_shuffle(uniqueKeys.begin(), uniqueKeys.end());
                    THEN("Remove returns true") {
                        for (int n : uniqueKeys) {
                            REQUIRE(tree.remove(n));
                        }
                        REQUIRE(isValidAVL(tree));
                        REQUIRE(tree.size() == 0);
                    }
                }
            }
            tree = makeTree(uniqueKeys);
            REQUIRE(isValidAVL(tree));
            REQUIRE(tree.size() == uniqueKeys.size());
            THEN("Removing non-existent keys returns false") {
                REQUIRE(!tree.remove(INT_MIN));
                REQUIRE(!tree.remove(INT_MAX));
                for (int n : uniqueKeys) {
                    REQUIRE(!tree.remove(n + rand() % 4 + 1));
                }
                REQUIRE(isValidAVL(tree));
                REQUIRE(tree.size() == uniqueKeys.size());
            }
        }
        WHEN("They are removed from the tree") {
            THEN("Remove returns false") {
                for (int n : uniqueKeys) {
                    REQUIRE(!tree.remove(n));
                }
                REQUIRE(tree.size() == 0);
            }
        }
    }
    GIVEN("All identical keys") {
        std::vector<std::string> sameStrings(NUM);
        std::fill(sameStrings.begin(), sameStrings.end(), "same");
        WHEN("They are inserted to an empty tree") {
            // Empty tree.
            auto tree = makeTree<std::string>({});
            REQUIRE(isValidAVL(tree));
            THEN("The first insert returns true") {
                REQUIRE(tree.insert(sameStrings.front(), {}));
                REQUIRE(isValidAVL(tree));
                THEN("Subsequent inserts return false") {
                    for (const auto& s : sameStrings) {
                        REQUIRE(!tree.insert(s, {}));
                    }
                    REQUIRE(tree.size() == 1);
                    REQUIRE(isValidAVL(tree));
                }
                THEN("Only 1 remove returns true") {
                    REQUIRE(tree.remove(sameStrings.front()));
                    for (const auto& s : sameStrings) {
                        REQUIRE(!tree.remove(s));
                    }
                    REQUIRE(tree.size() == 0);
                    REQUIRE(isValidAVL(tree));
                }
            }
        }
    }
}

SCENARIO("Searching for keys returns associated values") {
    GIVEN("An empty tree") {
        using tools::makeTree;
        const auto tree = makeTree<unsigned>({});
        REQUIRE(isValidAVL(tree));
        WHEN("Keys are searched") {
            THEN("Search always throws a runtime error") {
                REQUIRE_THROWS_AS(tree.search(INT_MIN), const std::runtime_error&);
                REQUIRE_THROWS_AS(tree.search(INT_MAX), const std::runtime_error&);
                for (int trials = 20; trials > 0; trials--) {
                    REQUIRE_THROWS_AS(tree.search(rand() - 1000), const std::runtime_error&);
                }
            }
        }
    }
    GIVEN("A non-empty tree") {
        using tools::HashMap;
        // Multiples of 8.
        std::vector<int> uniqueKeys(NUM);
        for (int i = 0; i < uniqueKeys.size(); i++) {
            uniqueKeys[i] = i * 8 - 16;
        }
        std::random_shuffle(uniqueKeys.begin(), uniqueKeys.end());
        // Checking against a hash map.
        HashMap<int, int> map;
        AVLTree<int, int> tree;
        map.reserve(uniqueKeys.size());
        for (int k : uniqueKeys) {
            const int n = rand() % NUM;
            tree.insert(k, n);
            map[k] = n;
        }
        REQUIRE(isValidAVL(tree));
        REQUIRE(tree.size() == uniqueKeys.size());
        WHEN("Searching for keys already in the tree") {
            std::random_shuffle(uniqueKeys.begin(), uniqueKeys.end());
            THEN("Search returns the values") {
                for (int k : uniqueKeys) {
                    REQUIRE(tree.search(k) == map.at(k));
                }
                REQUIRE(tree.size() == uniqueKeys.size());
                REQUIRE(isValidAVL(tree));
            }
        }
        WHEN("Searching for keys not in the tree") {
            THEN("Search throws a runtime error") {
                for (int k : uniqueKeys) {
                    REQUIRE_THROWS_AS(tree.search(k + rand() % 7 + 1), const std::runtime_error&);
                }
                REQUIRE_THROWS_AS(tree.search(INT_MIN), const std::runtime_error&);
                REQUIRE_THROWS_AS(tree.search(INT_MAX), const std::runtime_error&);
                REQUIRE(tree.size() == uniqueKeys.size());
                REQUIRE(isValidAVL(tree));
            }
        }
    }
}

SCENARIO_TEMPLATE("Copy constructor and operator make copies", T, char, short, unsigned,
    long double, std::string) {
    using tools::makeTree;
    GIVEN("An empty tree") {
        auto tree = makeTree<T>({});
        REQUIRE(tree.size() == 0);
        REQUIRE(isValidAVL(tree));
        WHEN("It's copied") {
            const auto copy = tree;
            REQUIRE(isValidAVL(copy));
            REQUIRE(isValidAVL(tree));
            THEN("The copy is also empty") {
                REQUIRE(copy.size() == 0);
                REQUIRE(tree == copy);
            }
            AND_WHEN("The copied-from tree is modified") {
                tree = makeTree(randValues<T>(10));
                REQUIRE(isValidAVL(tree));
                REQUIRE(isValidAVL(copy));
                THEN("They are now different") { REQUIRE(tree != copy); }
            }
        }
        WHEN("It's self assigned") {
            tree = tree;
            REQUIRE(isValidAVL(tree));
            THEN("It's still empty") {
                REQUIRE(tree.size() == 0);
                REQUIRE(tree == tree);
            }
            THEN("Insert and remove still work properly") {
                tools::appendTree(randValues<T>(NUM), tree);
                REQUIRE(isValidAVL(tree));
                tools::removeTree(randValues<T>(5), tree);
                REQUIRE(isValidAVL(tree));
            }
        }
        WHEN("It's assigned with an empty tree") {
            const decltype(tree) emptyTree;
            tree = emptyTree;
            REQUIRE(isValidAVL(tree));
            REQUIRE(isValidAVL(emptyTree));
            THEN("It's still empty") {
                REQUIRE(tree.size() == 0);
                REQUIRE(tree == emptyTree);
            }
        }
        WHEN("It's assigned with a non-empty tree") {
            auto randTree = makeTree(randValues<T>(NUM));
            REQUIRE(isValidAVL(randTree));
            tree = randTree;
            REQUIRE(isValidAVL(tree));
            THEN("They are the same") { REQUIRE(tree == randTree); }
            AND_WHEN("The copied-from tree is modified") {
                randTree = {};
                REQUIRE(isValidAVL(randTree));
                REQUIRE(isValidAVL(tree));
                THEN("They are now different") { REQUIRE(tree != randTree); }
            }
        }
    }
    GIVEN("A non-empty tree") {
        auto tree = makeTree(randValues<T>(NUM));
        REQUIRE(isValidAVL(tree));
        WHEN("It's copied") {
            const auto copy = tree;
            REQUIRE(isValidAVL(copy));
            REQUIRE(isValidAVL(tree));
            THEN("The copy is identical") { REQUIRE(tree == copy); }
            AND_WHEN("The copied-from tree is modified") {
                tree = {};
                REQUIRE(isValidAVL(tree));
                REQUIRE(isValidAVL(copy));
                THEN("They are now different") { REQUIRE(tree != copy); }
            }
        }
        WHEN("It's self assigned") {
            tree = tree;
            THEN("It's still the same") {
                REQUIRE(isValidAVL(tree));
                REQUIRE(tree == tree);
            }
            THEN("Insert and remove still work properly") {
                tools::appendTree(randValues<T>(NUM), tree);
                REQUIRE(isValidAVL(tree));
                tools::removeTree(randValues<T>(5), tree);
                REQUIRE(isValidAVL(tree));
            }
        }
        WHEN("It's assigned with an empty tree") {
            const decltype(tree) emptyTree;
            tree = emptyTree;
            REQUIRE(isValidAVL(tree));
            REQUIRE(isValidAVL(emptyTree));
            THEN("It's empty") {
                REQUIRE(tree.size() == 0);
                REQUIRE(tree == emptyTree);
            }
        }
        WHEN("It's assigned with a non-empty tree") {
            auto randTree = makeTree(randValues<T>(NUM));
            REQUIRE(isValidAVL(randTree));
            tree = randTree;
            REQUIRE(isValidAVL(tree));
            THEN("They are the same") { REQUIRE(tree == randTree); }
            AND_WHEN("The copied-from tree is modified") {
                randTree = {};
                REQUIRE(isValidAVL(randTree));
                REQUIRE(isValidAVL(tree));
                THEN("They are now different") { REQUIRE(tree != randTree); }
            }
        }
    }
}

SCENARIO_TEMPLATE("Values and keys vectors are sorted by keys", T, char, int, long, double,
    unsigned long long, std::string) {
    GIVEN("An empty tree") {
        using tools::makeTree;
        const auto tree = makeTree<T>({});
        WHEN("Keys are returned") {
            THEN("They are empty") { REQUIRE(tree.keys().empty()); }
        }
        WHEN("Values are returned") {
            THEN("They are empty") { REQUIRE(tree.values().empty()); }
        }
    }
    GIVEN("Randomly inserted keys and values") {
        // Checking against a red-black tree.
        std::map<T, int> map;
        AVLTree<T, int> tree;
        for (const auto& key : randValues<T>(NUM)) {
            const int val = rand() % NUM;
            tree.insert(key, val);
            map.emplace(key, val);
        }
        REQUIRE(isValidAVL(tree));
        REQUIRE(tree.size() == map.size());
        WHEN("Keys are returned") {
            const auto retKeys = tree.keys();
            THEN("They are in sorted order") {
                REQUIRE(std::is_sorted(retKeys.begin(), retKeys.end()));
            }
        }
        WHEN("Values are returned") {
            const auto retVals = tree.values();
            THEN("They are in sorted order by keys") {
                std::vector<int> expVals;
                expVals.reserve(map.size());
                for (const auto& pair : map) {
                    expVals.push_back(pair.second);
                }
                REQUIRE((retVals == expVals));
            }
        }
    }
    GIVEN("Uniquely inserted keys") {
        using tools::randUniqueInts;
        std::map<int, T> map;
        AVLTree<int, T> tree;
        for (int key : randUniqueInts(NUM)) {
            const T val = randValues<T>(1).front();
            tree.insert(key, val);
            map.emplace(key, val);
        }
        REQUIRE(isValidAVL(tree));
        REQUIRE(tree.size() == map.size());
        WHEN("Keys are returned") {
            const auto retKeys = tree.keys();
            THEN("They are in sorted order") {
                REQUIRE(std::is_sorted(retKeys.begin(), retKeys.end()));
            }
        }
        WHEN("Values are returned") {
            const auto retVals = tree.values();
            THEN("They are in sorted order by keys") {
                std::vector<T> expVals;
                expVals.reserve(map.size());
                for (const auto& pair : map) {
                    expVals.push_back(pair.second);
                }
                REQUIRE((retVals == expVals));
            }
        }
    }
}

// An attempt at monkey testing.
TEST_CASE("Randomized insert and remove stress test" * doctest::timeout(300)) {
    auto uniqueNums = tools::randUniqueInts(12321);
    std::vector<int> maxSizes;
    for (int maxSize = uniqueNums.size(); maxSize > 0; maxSize /= 2) {
        maxSizes.push_back(maxSize);
    }
    std::reverse(maxSizes.begin(), maxSizes.end());
    AVLTree<int, bool> tree;
    for (int maxSize : maxSizes) {
        // Clear.
        tree = {};
        // Insert maxSize unique keys.
        for (int i = 0; i < maxSize; i++) {
            REQUIRE(tree.insert(uniqueNums[i], {}));
            if (rand() % 100 == 0) {
                // Randomly inspecting ~ 1%.
                REQUIRE(isValidAVL(tree));
            }
        }
        REQUIRE(isValidAVL(tree));
        REQUIRE(tree.size() == maxSize);
        // Remove all inserted keys.
        std::random_shuffle(uniqueNums.begin(), uniqueNums.begin() + maxSize);
        for (int i = 0; i < maxSize; i++) {
            REQUIRE(tree.remove(uniqueNums[i]));
            if (rand() % 10 == 0) {
                // Randomly inspecting ~ 10%.
                REQUIRE(isValidAVL(tree));
            }
        }
        REQUIRE(isValidAVL(tree));
        REQUIRE(tree.size() == 0);
        std::random_shuffle(uniqueNums.begin(), uniqueNums.end());
    }
}
