#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

// g++ -std=c++11 -Wall -Wfatal-errors -g -fsanitize=undefined,address -o test test_main.cpp Queue.cpp && ./test

#include "Queue.h"
#include <stdexcept>
#include <vector>

static bool operator==(Queue lhs, Queue rhs) {
    // Also check if they share the same memory.
    while (!lhs.isEmpty() && !rhs.isEmpty()) {
        const int n = lhs.peek();
        lhs.dequeue();
        if (n != rhs.peek()) {
            return false;
        }
        rhs.dequeue();
    }
    return lhs.isEmpty() && rhs.isEmpty();
}

static bool operator!=(const Queue& lhs, const Queue& rhs) { return !(lhs == rhs); }

static std::ostream& operator<<(std::ostream& os, Queue q) {
    while (!q.isEmpty()) {
        os << q.peek() << ' ';
        q.dequeue();
    }
    return os;
}

SCENARIO("Queues can grow and shrink.") {
    GIVEN("An empty queue") {
        Queue q;
        CHECK(q.isEmpty());

        THEN("Accessors/mutators throw exceptions.") {
            CHECK_THROWS_AS(q.peek(), const std::logic_error&);
            CHECK_THROWS_AS(q.dequeue(), const std::logic_error&);
        }

        WHEN("An item is inserted") {
            const int n = rand();
            q.enqueue(n);

            THEN("The queue is not empty.") { CHECK(!q.isEmpty()); }
            THEN("Peek returns that item.") { CHECK(q.peek() == n); }

            WHEN("That item is removed") {
                q.dequeue();

                THEN("The queue is empty again.") { CHECK(q.isEmpty()); }
                THEN("Accessors/mutators throw exceptions.") {
                    CHECK_THROWS_AS(q.peek(), const std::logic_error&);
                    CHECK_THROWS_AS(q.dequeue(), const std::logic_error&);
                }
            }
        }

        WHEN("10 items are inserted") {
            for (int i = 0; i < 10; i++) {
                q.enqueue(i);
                THEN("Peek returns the first item inserted during each insertion.") {
                    CHECK(q.peek() == 0);
                }
            }
            THEN("Queue is not empty.") { CHECK(!q.isEmpty()); }
            THEN("10 removals is needed to clear the queue.") {
                int i = 0;
                while (!q.isEmpty()) {
                    THEN("Peek returns the first item during each deletion.") {
                        CHECK(q.peek() == i);
                    }
                    ++i;
                    q.dequeue();
                }
                CHECK(i == 10);
            }
        }
    }
}

static Queue getRandQueue(int minSize, int maxSize) {
    Queue q;
    for (int i = rand() % maxSize + minSize; i >= 0; i--) {
        q.enqueue(rand());
    }
    return q;
}

SCENARIO("Queues can copy.") {
    GIVEN("An empty queue") {
        Queue empty;

        WHEN("An empty queue copies from an empty queue") {
            Queue emptyQ;
            emptyQ = empty;

            THEN("Both of them are empty.") {
                CHECK(empty.isEmpty());
                CHECK(emptyQ.isEmpty());
                CHECK(empty == emptyQ);
            }
        }

        WHEN("A non-empty queue copies from an empty queue") {
            Queue neQ = getRandQueue(1, 20);
            neQ = empty;

            THEN("Both of them are empty.") {
                CHECK(neQ.isEmpty());
                CHECK(empty.isEmpty());
                CHECK(empty == neQ);
            }
        }
    }

    for (int trials = 10; trials >= 0; trials--) {
        GIVEN("A random non-empty queue") {
            Queue randQ = getRandQueue(1, 20);

            WHEN("Its copy is constructed") {
                const Queue copy(randQ);
                THEN("They are the same.") { CHECK(randQ == copy); }

                WHEN("The original queue is modified.") {
                    randQ.dequeue();
                    THEN("They are now different.") { CHECK(copy != randQ); }
                }
            }

            WHEN("Another queue copies from it") {
                Queue q = getRandQueue(0, 20);
                q = randQ;
                THEN("They are the same.") { CHECK(randQ == q); }

                WHEN("The original queue is modified.") {
                    randQ.dequeue();
                    randQ.enqueue(rand());
                    randQ.dequeue();
                    THEN("They are now different.") { CHECK(q != randQ); }
                }
            }

            WHEN("It's assigned to itself") {
                const Queue copy(randQ);
                randQ = randQ;

                THEN("It's still the same.") {
                    CHECK(randQ == copy);
                    CHECK(randQ == randQ);
                }
            }
        }
    }
}

TEST_CASE("Resize stress test" * doctest::timeout(2)) {
    std::vector<int> v(1e5);
    Queue q;
    CHECK(q.isEmpty());

    for (int& n : v) {
        n = rand();
        q.enqueue(n);
        CHECK(q.peek() == v.front());
    }
    CHECK(!q.isEmpty());

    for (int n : v) {
        CHECK(q.peek() == n);
        q.dequeue();
    }
    CHECK(q.isEmpty());
}
