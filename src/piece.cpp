#include "../include/piece.hpp"

namespace pcs {

    const BB genericBB = BB(0,0,0,0);

    //Define each piece with a single starting orientation. I'll give it more orientations later, don't worry
    std::vector<std::pair<int, int>> red_coords = {
              {-2,-1},{-2,0},
                      {-1,0},
                      {0,0},
        {1,-2},{1,-1},{1,0},{1,1},{1,2},
                      {2,0}
    };

    std::vector<std::pair<int, int>> blue_coords = {
        {-2,-1},{-2,0},
                {-1,0},
        {0,-1},{0,0},{0,1},
                {1,0},
                {2,0}    
    };

    std::vector<std::pair<int, int>> orange_coords = {
               {-1,0},
        {0,-1},{0,0},{0,1},
               {1,0}, 
    };

/*
    std::vector<std::pair<int, int>> stupid_coords = {
        {-2,-1},{-2,0}, {-2, 5},

        {0,-1},{0,0},{0,1},
                
                {2,0} ,
                {100,0} 
    };
*/

    //Names and symbols must be unique! Also, symbols can't be ' '.
    Piece red = Piece("red", 'r', {Orientation(red_coords, genericBB)});
    Piece blue = Piece("blue", 'b', {Orientation{blue_coords, genericBB}});
    Piece orange = Piece("orange", 'o', {Orientation(orange_coords, genericBB)});
    //Piece stupid = Piece("stupid", 's', {Orientation(stupid_coords, genericBB)});

    //recall that if keys are repeated, only the more recent entry will be included
    const std::unordered_map<std::string, Piece*> piece_names = {
        {"red", &red},
        {"blue", &blue},
        {"orange", &orange},
        //{"stupid", &stupid} //I was just using this for testing the displayPiece functionality
    };

}