/**
 * @file PlayList.h
 * @author Hieu Duong
 * @date 2022-06-03
 */
// Write your name and date here
#include "PlayList.h"

// PlayList method implementations go here

PlayList::PlayList() { }

PlayList::PlayList(const PlayList& pl) { }

PlayList::~PlayList() { }

/**
 * @note Copy-and-swap idiom.
 */
PlayList& PlayList::operator=(PlayList other) {
    using std::swap;
    swap(*this, other);
    return *this;
}

void PlayList::insert(const Song& sng, unsigned pos) { }

Song PlayList::remove(unsigned pos) { }

void PlayList::swap(unsigned pos1, unsigned pos2) { }

const Song& PlayList::get(unsigned pos) const { }

unsigned PlayList::size() const { }

    }
    TEST_CASE("bar") { CHECK("bar" != "foo"); }
}

// TODO: Test the swap function.
