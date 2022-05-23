// Song class definition for CMPT 225 assignment
// Author: John Edgar
// Date: May 2022

#pragma once
#include <string>

class Song {
public:
    // Constructor
    Song(const std::string& name, const std::string& artist, int length);

    // Accessors
    const std::string& getName() const;
    const std::string& getArtist() const;
    int getLength() const;

private:
    std::string name_;
    std::string artist_;
    int length_;
};
