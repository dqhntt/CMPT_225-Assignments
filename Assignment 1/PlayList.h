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

    // Constructors

    // PARAM: sng = song data
    // POST: Sets song to sng and next to nullptr
    PlayListNode(Song sng)
        : PlayListNode(std::move(sng), nullptr)
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
    PlayList& operator=(const PlayList& other);

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

    void swap(PlayList& other);
};
