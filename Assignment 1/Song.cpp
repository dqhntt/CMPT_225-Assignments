// Song method implementations for CMPT 225 assignment
// Author: John Edgar
// Date: May 2022

#include "Song.h"
#include <stdexcept>

using std::out_of_range;
using std::runtime_error;
using std::string;

// Constructor
Song::Song(std::string name, std::string artist, int length)
    : name_(std::move(name))
    , artist_(std::move(artist))
    , length_(length)
{
    if (length < 1) {
        throw std::invalid_argument("negative run time");
    }
    if (name_.empty()) {
        throw std::invalid_argument("a song must have a name");
    }
    if (artist_.empty()) {
        throw std::invalid_argument("a song must have an artist");
    }
}

// Accessors
// POST: returns name of song
const string& Song::name() const { return name_; }

// POST: returns recording artist of song
const string& Song::artist() const { return artist_; }

// POST: returns length in seconds of song
int Song::length() const { return length_; }
