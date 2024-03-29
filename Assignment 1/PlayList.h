/**
 * @file PlayList.h
 * @author Hieu Duong
 * @date 2022-06-03
 */
// Write your name and date here
#pragma once
#include "Song.h"

// Definition of the PlayListNode class - *do not change*
struct PlayListNode {
    Song song;          // data representing a song
    PlayListNode* next; // pointer to next node in list

    // PARAM: sng = song data
    // POST: Sets song to sng and next to nullptr
    PlayListNode(Song sng)
        : song(std::move(sng))
        , next(nullptr)
    { }

    // PARAM: sng = song data, nxt = pointer to next node
    // POST: Sets song to sng and next to nxt
    PlayListNode(Song sng, PlayListNode* nxt)
        : song(std::move(sng))
        , next(nxt)
    { }
};

// Complete class definition here
// Class should implement a linked list of nodes
// See assignment description for detailed requirements
class PlayList {
    /**
     * @remarks Lessons:
     * - Specifications should not explicitly define implementations.
     * - Copy constructors should always make deep copies.
     * - .operator==(other) is same as *this == other, evaluating right to left.
     * - Don't mess with force pushing git!!
     * - Const correctness relies on its sub-components also being const correct.
     * - Best to pass by const reference by default, may add overload for std::move.
     *   (https://youtu.be/xnqTKD8uD64?t=4006)
     * - Perfect forwarding is hard to write perfectly.
     */
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
    // PARAM: pos - 0-based insertion position
    //        song - Song to be inserted at pos
    void insert(Song song, unsigned pos);

    // PRE: 0 <= pos <= length of list-1
    // PARAM: pos - 0-based position of element to be removed and returned
    // POST: Song at position pos is removed and returned
    Song remove(unsigned pos);

    // PRE: 0 <= pos1, pos2 <= length of list-1
    // PARAM: pos1, pos2 - 0-based positions of elements to be swapped
    // POST: Songs at positions pos1 and pos2 are swapped
    void swap(unsigned pos1, unsigned pos2);
    
    // Container swap
    void swap(PlayList& other);

    // Accessor
    // PRE: 0 <= pos <= length of list-1
    // PARAM: pos - 0-based position of element to be removed and returned
    // POST: returns the Song at position pos
    Song get(unsigned pos) const;

    // POST: returns the number of songs in the PlayList
    unsigned size() const;

private:
    unsigned size_;
    PlayListNode *head_, *tail_;
};
