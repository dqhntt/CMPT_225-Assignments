/**
 * @file a1main.cpp
 * @author Hieu Duong
 * @brief Song PlayList using singly linked list.
 * @date 2022-06-03
 *
 * @remarks Worst-case running time in Big-O notation of:
 *
 * PlayList Methods:
 * 1. Default constructor - O(1)
 * Constant.
 * Only a few variables are initialized.
 *
 * 2. Destructor - O(n)
 * Linear in list's size.
 * Traverse through the entire list (n) and destroy each (1) node. n * 1 ~ n.
 *
 * 3. Copy constructor - O(n)
 * Linear in other list's size.
 * Copy all (n) elements from the other list.
 *
 * 4. Overloaded assignment operator - O(n)
 * Linear in sum (n) of this list's size (n1) and other list's size (n2).
 * Copy all (n2), swap few (1), destroy all (n1) elements.
 *
 * 5. Insert - O(n)
 * Linear in the position of the wanted element.
 * Worst when inserting near end, has to traverse (n - 1) elements to get there.
 *
 * 6. Remove - O(n)
 * Linear in the position of the wanted element.
 * Worst when removing end, has to traverse (n - 1) elements to get in front of it.
 *
 * 7. Get - O(n)
 * Linear in the position of the wanted element.
 * Worst when getting near end, has to traverse (n - 1) elements to get there.
 *
 * 8. Swap - O(n)
 * Linear in max position of the 2 wanted elements.
 * Worst when swapping end, has to traverse (n - 1) elements to get in front of it.
 *
 * 9. Size - O(1)
 * Size is cached in a variable.
 *
 * Main Function Commands:
 * 1. Enter a song - O(n)
 * Linear in the position of the wanted element.
 * Same as 5. Insert.
 *
 * 2. Remove a song - O(n)
 * Linear in the position of the wanted element.
 * Same as 6. Remove.
 *
 * 3. Swap two songs - O(n)
 * Linear in max position of the 2 wanted elements.
 * Same as 8. Swap.
 *
 * 4. Print all the songs - O(n^2)
 * Quadratic in the total number of list's elements.
 * Loop through all elements (n) with get in each iteration. sum(1..n) ~ n^2.
 */
#include "PlayList.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {
std::string input(const std::string& prompt) {
    std::cout << prompt << ' ';
    std::string response;
    std::getline(std::cin, response);
    return response;
}

bool isInt(const std::string& str) {
    std::istringstream iss(str);
    int testNum = 0;
    char testChar = ' ';
    return (iss >> testNum) && !(iss >> testChar);
}

template <class UnaryPredicate = bool (*)(const std::string&)>
std::string getValidInput(
    const std::string& promptMsg, const std::string& errorMsg, UnaryPredicate pred) {
    std::string response = input(promptMsg);
    while (!pred(response)) {
        std::cout << errorMsg << "\n";
        response = input(promptMsg);
    }
    return response;
}

int getIntInRange(int low, int high, const std::string& promptMsg, const std::string& errorMsg) {
    return std::stoi(
        getValidInput(promptMsg, errorMsg, [low, high](const std::string& str) -> bool {
            int num = -1;
            return isInt(str) && (num = std::stoi(str)) >= low && num <= high;
        }));
}

enum class SongField { name, artist, length };
bool isValidSong(const std::string& input, SongField field) {
    switch (field) {
    case SongField::name:
    case SongField::artist:
        return !input.empty();
    case SongField::length:
        return isInt(input) && std::stoi(input) > 0;
    default:
        throw std::invalid_argument("Unimplemented song field in validator function.");
        return false;
    }
}

namespace menu {
    enum class Option { insert, remove, swap, print, quit };
    Option getOption() {
        return static_cast<Option>(
            getIntInRange(1, 5, "Enter 1 (insert), 2 (remove), 3 (swap), 4 (print) or 5 (quit):",
                "Invalid menu option.")
            - 1);
    }

    void displayInstructions() {
        std::cout << "Menu:\n"
                     "1 - Enter a song in the play list at a given position\n"
                     "2 - Remove a song from the play list at a given position\n"
                     "3 - Swap two songs in the play list\n"
                     "4 - Print all the songs in the play list\n"
                     "5 - Quit\n\n";
    }

    void insertSongs(PlayList& pl) {
        auto validateSong = [](SongField field) {
            return [field](const std::string& str) { return isValidSong(str, field); };
        };
        const std::string songName
            = getValidInput("Song name:", "Invalid name.", validateSong(SongField::name));
        Song song = {
            songName, getValidInput("Artist:", "Invalid artist.", validateSong(SongField::artist)),
            std::stoi(getValidInput("Length:", "Invalid length.", validateSong(SongField::length)))
        };
        const unsigned size = pl.size();
        const unsigned pos = getIntInRange(1, size + 1,
            "Position (1" + (size > 0 ? (" to " + std::to_string(size + 1)) : "") + "):",
            "Invalid position.");
        pl.insert(std::move(song), pos - 1);
        std::cout << "You entered " << songName << " at position " << pos
                  << " in the play list.\n\n";
    }

    void removeSongs(PlayList& pl) {
        const unsigned size = pl.size();
        if (size == 0) {
            std::cout << "There is no song in the play list to remove.\n\n";
            return;
        }
        const std::string removedSongName
            = pl.remove(
                    getIntInRange(1, size,
                        "Position (1" + (size > 1 ? (" to " + std::to_string(size)) : "") + "):",
                        "Invalid position.")
                    - 1)
                  .getName();
        std::cout << "You removed " << removedSongName << " from the play list.\n\n";
    }

    void swapSongs(PlayList& pl) {
        const unsigned size = pl.size();
        if (size <= 1) {
            std::cout << "Play list has " << size << " song. Nothing to swap.\n\n";
            return;
        }
        const unsigned pos1 = getIntInRange(1, size,
            "Swap song at position (1 to " + std::to_string(size) + "):", "Invalid position.");
        const unsigned pos2 = getIntInRange(1, size,
            "with the song at position (1 to " + std::to_string(size) + "):", "Invalid position.");
        if (pos1 == pos2) {
            std::cout << "Same position (" << pos1 << "). Nothing to swap.\n\n";
            return;
        }
        pl.swap(pos1 - 1, pos2 - 1);
        std::cout << "You swapped the songs at positions " << pos1 << " and " << pos2 << ".\n\n";
    }

    void printSongs(const PlayList& pl) {
        const unsigned size = pl.size();
        for (unsigned i = 0; i < size; i++) {
            Song song = pl.get(i);
            std::cout << " " << i + 1 << " " << song.getName() << " (" << song.getArtist() << ") "
                      << song.getLength() << "s\n";
        }
        std::cout << "There are " << size << " songs in the play list.\n\n";
    }
} // namespace menu
} // namespace

int main() {
    menu::displayInstructions();
    PlayList pl;
    for (;;) {
        switch (menu::getOption()) {
        case menu::Option::insert:
            menu::insertSongs(pl);
            break;
        case menu::Option::remove:
            menu::removeSongs(pl);
            break;
        case menu::Option::swap:
            menu::swapSongs(pl);
            break;
        case menu::Option::print:
            menu::printSongs(pl);
            break;
        case menu::Option::quit:
            std::cout << "You have chosen to quit the program.\n";
            return EXIT_SUCCESS;
        default:
            throw std::invalid_argument("Unimplemented menu option.");
        }
    }
}
