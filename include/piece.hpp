#ifndef PIECE_HPP
#define PIECE_HPP

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace pcs {

    struct BB {
        int left, right, up, down; //defined relative to start_square, if at all

        BB(int l, int r, int u, int d) : left(l), right(r), up(u), down(d) {}
    };

    struct Orientation {
        std::vector<std::pair<int, int>> coordinates; //grid data. must always include {0,0}
        BB boundingBox;

        Orientation(const std::vector<std::pair<int, int> >& c, const BB& bb) : coordinates(c), boundingBox(bb) {}
    }; 

    struct Piece {
        std::string name;
        char symbol;
        std::vector<Orientation> orientations;

        Piece(const std::string n, const char s, const std::vector<Orientation>& o) : name(n), symbol(s), 
            orientations(o) {}
    };

    extern const std::unordered_map<std::string, Piece*> piece_names;
    extern const BB genericBB;

}

#endif