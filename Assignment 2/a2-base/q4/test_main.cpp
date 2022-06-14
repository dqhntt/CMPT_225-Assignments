#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "Queue.h"
#include <cstddef>
#include <stdexcept>
#include <vector>

bool operator==(const Queue& lhs, const Queue& rhs) {
    Queue lcopy(lhs), rcopy = rhs;
    while (!lcopy.isEmpty()) {
        if (lcopy.peek() != rcopy.peek()) {
            return false;
        }
        lcopy.dequeue();
        rcopy.dequeue();
    }
    return lcopy.isEmpty() && rcopy.isEmpty();
}

bool operator!=(const Queue& lhs, const Queue& rhs) { return !(lhs == rhs); }

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
            THEN("10 removals are needed to clear the queue.") {
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

    GIVEN("A random queue") {
        for (int trials = 10; trials >= 0; trials--) {
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
                    THEN("They are now different.") { CHECK(q != randQ); }
                }
            }
        }
    }
}

TEST_CASE("Resize stress test" * doctest::timeout(2)) {
    std::vector<int> v(1e6);
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
