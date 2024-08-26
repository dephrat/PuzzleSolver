#include "../include/stages.hpp"

//THIS FILE SPECIFICALLY SHOULD NOT HAVE ACCESS TO THE DISPLAY CLASS BECAUSE DISPLAY RELIES ON VALIDATION AND ORIENTATIONS

bool piecesValidation() {
    //check:
    //all pieces have exactly one orientation (for now)
    //each piece name in piece_names matches the name of the piece it maps to
    //each piece has a symbol other than ' ' (the empty char)
    //each piece's symbol is unique
    //UNCHECKED each piece contains a cross centered at 0,0 (how do I even do this with a vector?)
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
    }
    //Check that this piece's symbol is unique
    if (symbols.size() != pcs::piece_names.size()) {
        std::cerr << msgs::repeatedPieceSymbolErrorMessage << std::endl;
        return false;
    }
    return true;
}


void rotate(std::vector<std::pair<int, int>>& coordinates) {
    for (auto& [x, y] : coordinates) {
        y -= x;
        x += y;
        y -= x;
    }
}
void generateOrientationCoords(pcs::Piece* piece) {
    std::vector<std::pair<int, int>> coordinates = piece->orientations[0].coordinates; //Don't make this a reference! We leave orientations[0].coordinates untouched! (Until sortOrientations)
    //rotate once
    rotate(coordinates);
    //create Orientation and add to piece.orientations
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    rotate(coordinates);
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    rotate(coordinates);
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    //flip the piece: y stays the same, x becomes negative of itself
    for (auto& [x, y] : coordinates) x = -x;
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    rotate(coordinates);
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    rotate(coordinates);
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    rotate(coordinates);
    piece->orientations.emplace_back(coordinates, pcs::genericBB);
    
}

void sortOrientations(pcs::Piece* piece) {
    for (auto& orientation : piece->orientations) {
        auto& coordinates = orientation.coordinates;
        std::sort(coordinates.begin(), coordinates.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            if (a.first != b.first) return a.first < b.first;
            else return a.second < b.second;
        });
    }
}

inline std::string pairToString(std::pair<int, int>& p) {
    return std::to_string(p.first) + std::to_string(p.second);
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

// Equality function for std::vector<Location>
struct VectorPairEqual {
    bool operator()(const std::vector<Location>& lhs, const std::vector<Location>& rhs) const {
        return lhs == rhs;
    }
};

//maybe theoretically possible for two orientations with different coordinates but the same shape? I haven't analyzed that
void trimOrientations(pcs::Piece* piece) {
    //use special unordered_set to check for duplicates, store indices of duplicates in duplicate_indices
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
            int x = coordinate.first;
            int y = coordinate.second;
            l = std::min(l, x);
            r = std::max(r, x);
            u = std::min(u, y);
            d = std::max(d, y);
        }
        orientation.boundingBox = pcs::BB(l, r, u, d);
    }
}

//generates orientations, sorts orientation coordinates internally, and removes duplicate orientations. 
//STILL UNTESTED
void generatePieceOrientations() {
    for (auto& [name, piece] : pcs::piece_names) {
        generateOrientationCoords(piece);
        sortOrientations(piece); //internal sorting, makes trimOrientations easier
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