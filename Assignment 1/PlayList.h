/**
 * @file PlayList.h
 * @author Hieu Duong
 * @date 2022-06-03
 */
// Write your name and date here
#pragma once
#include "Song.h"
#include <stdexcept>

// Definition of the PlayListNode class - *do not change*
struct PlayListNode {
    Song song;          // data representing a song
    PlayListNode* next; // pointer to next node in list

    // Constructors

    // PARAM: sng = song data
    // POST: Sets song to sng and next to nullptr
    PlayListNode(const Song& sng)
        : PlayListNode(sng, nullptr)
	{ }

    // PARAM: sng = song data, nxt = pointer to next node
    // POST: Sets song to sng and next to nxt
    PlayListNode(const Song& sng, PlayListNode* nxt)
        : song(sng)
        , next(nxt)
	{ }
};

// Complete class definition here
// Class should implement a linked list of nodes
// See assignment description for detailed requirements
class PlayList {
public:
    // Constructors and destructor
    // POST: head of list is set to nullptr
    PlayList();

    // PARAM: other - PlayList to be copied
    // POST: new PlayList is created that is a deep copy of other
    PlayList(const PlayList& other);

    // POST: dynamic memory associated with object is deallocated
    ~PlayList();

    // PARAM: other - PlayList to be copied
    // POST: dynamic memory of calling object deallocated (except
    //       under self-assignment), calling object is set to a
    //       a deep copy of other
    PlayList& operator=(PlayList other);

    // Mutators
    // PRE: 0 <= i <= length of list
    // PARAM: position - 0-based insertion position
    //        song - Song to be inserted at position
    void insert(const Song& song, unsigned position);

    // PRE: 0 <= position <= length of list-1
    // PARAM: position - 0-based position of element to be removed and returned
    // POST: Song at position position is removed and returned
    Song remove(unsigned position);

    // PRE: 0 <= position1, position2 <= length of list-1
    // PARAM: position1, position2 - 0-based positions of elements to be swapped
    // POST: Songs at positions position1 and position2 are swapped
    void swap(unsigned position1, unsigned position2);

    // Accessor
    // PRE: 0 <= position <= length of list-1
    // PARAM: position - 0-based position of element to be removed and returned
    // POST: returns the Song at position position
    const Song& get(unsigned position) const;

    // POST: returns the number of songs in the PlayList
    unsigned size() const;

    /**
     * @throw None.
     */
    friend void swap(PlayList& lhs, PlayList& rhs) {
		std::swap(lhs.head_, rhs.head_);
		std::swap(lhs.size_, rhs.size_);
	}

private:
    // TO DO
    unsigned size_;
    PlayListNode* head_;
};
