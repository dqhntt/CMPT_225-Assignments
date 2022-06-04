// Song class definition for CMPT 225 assignment
// Author: John Edgar
// Date: May 2022

#pragma once
#include <string>

class Song {
public:
    Song() = default;
    Song(std::string name, std::string artist, int length);

    const std::string& name() const;
    const std::string& artist() const;
    int length() const;

private:
    std::string name_;
    std::string artist_;
    int length_ { -1 };
};
