#include <iostream>
#include <unordered_set>
#include <utility>

#include "../include/messages.hpp"
#include "../include/piece.hpp"
#include "../include/stage0.hpp"

using Location = std::pair<int, int>;

bool piecesValidation() {
    std::unordered_set<char> symbols;
    for (const std::pair<std::string, pcs::Piece*>& p : pcs::piece_names) {
        //Check that the piece has exactly one orientations
        if (p.second->orientations.size() != 1) {
            std::cerr << msgs::incorrectNumberPieceOrientationsErrorMessage << std::endl;
            return false;
        }
        //Check that the piece name matches the name of the piece it maps to
        if (p.first != p.second->name) {
            std::cerr << msgs::incorrectPieceNameErrorMessage << std::endl;
            return false;
        }
        //Check that the piece symbol is not ' ' (the empty char)
        if (p.second->symbol == ' ') {
            std::cerr << msgs::illegalEmptyPieceSymbolErrorMessage << std::endl;
            return false;
        }
        symbols.insert(p.second->symbol);
        //Check that the piece contains {0,0}
        bool hasZeroZero = false;
        for (const auto& coordinate : p.second->orientations[0].coordinates) {
            if (coordinate.first == 0 && coordinate.second == 0) {
                hasZeroZero = true;
                break;
            }
        }
        if (hasZeroZero == false) {
            std::cerr << msgs::pieceMissingZeroZeroErrorMessage << std::endl;
            return false;
        }
    }
    //Check that this piece's symbol is unique
    if (symbols.size() != pcs::piece_names.size()) {
        std::cerr << msgs::repeatedPieceSymbolErrorMessage << std::endl;
        return false;
    }
    return true;
}

inline void rotate(std::vector<std::pair<int, int>>& coordinates) {
    for (auto& [row, col] : coordinates) {
        col -= row;
        row += col;
        col -= row;
    }
}

void generateOrientationCoords(pcs::Piece* piece) {
    std::vector<std::pair<int, int>> coordinates = piece->orientations[0].coordinates;
    //rotate once
    rotate(coordinates);
    //create Orientation and add to piece.orientations
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    rotate(coordinates);
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    rotate(coordinates);
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    //flip the piece over the x-axis
    for (auto& [row, col] : coordinates) row = -row;
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    rotate(coordinates);
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    rotate(coordinates);
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    rotate(coordinates);
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
}

inline void sortOrientations(pcs::Piece* piece) {
    for (auto& orientation : piece->orientations) {
        auto& coordinates = orientation.coordinates;
        std::sort(coordinates.begin(), coordinates.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            if (a.first != b.first) return a.first < b.first;
            else return a.second < b.second;
        });
    }
}

struct VectorPairHash {
    std::size_t operator()(const std::vector<Location>& vec) const {
        std::size_t seed = vec.size();
        for (const auto& p : vec) {
            seed ^= std::hash<int>()(p.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<int>()(p.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

inline void trimOrientations(pcs::Piece* piece) {
    //look for duplicates, store indices in duplicate_indices for deletion
    std::unordered_set<std::vector<Location>, VectorPairHash> orientations_set;
    std::vector<int> duplicate_indices;
    for (int i = 0; i < piece->orientations.size(); ++i) {
        auto& coords = piece->orientations[i].coordinates;
        if (!orientations_set.insert(coords).second) {
            duplicate_indices.push_back(i);
        }
    }
    //erase orientations at indices corresponding to duplicates
    for (int i = duplicate_indices.size() - 1; i >= 0; --i) {
        piece->orientations.erase(piece->orientations.begin() + duplicate_indices[i]);
    }
}

void createBoundingBoxes(pcs::Piece* piece) {
    for (pcs::Orientation& orientation : piece->orientations) {
        int l = 0, u = 0, r = 0, d = 0;
        for (const auto& coordinate : orientation.coordinates) {
            int row = coordinate.first;
            int col = coordinate.second;
            l = std::min(l, col);
            r = std::max(r, col);
            u = std::min(u, row);
            d = std::max(d, row);
        }
        orientation.boundingBox = pcs::BB(l, r, u, d);
    }
}

//generates orientations, sorts orientation coordinates internally, and removes duplicate orientations. 
void generatePieceOrientations() {
    for (auto& [name, piece] : pcs::piece_names) {
        generateOrientationCoords(piece);
        sortOrientations(piece);
        trimOrientations(piece);
        createBoundingBoxes(piece);
    }
}

void stage0() {
    std::cout << "Validating pieces" << std::endl;
    if (!piecesValidation()) exit(0);
    std::cout << "Generating piece orientations" << std::endl;
    generatePieceOrientations();
}