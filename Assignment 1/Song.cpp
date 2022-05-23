// Song method implementations for CMPT 225 assignment
// Author: John Edgar
// Date: May 2022

#include "Song.h"
#include <stdexcept>

using std::string;
using std::out_of_range;
using std::runtime_error;

// Constructor
Song::Song(const string& name, const string& artist, int length)
    : name_(name)
    , artist_(artist)
    , length_(length)
{
    if (length < 1) {
        throw out_of_range("negative run time");
    }
    if (name.empty()) {
        throw runtime_error("a song must have a name");
    }
    if (artist.empty()) {
        throw runtime_error("a song must have an artist");
    }
}

// Accessors
// POST: returns name of song
const string& Song::getName() const { return name_; }

// POST: returns recording artist of song
const string& Song::getArtist() const { return artist_; }

// POST: returns length in seconds of song
int Song::getLength() const { return length_; }
