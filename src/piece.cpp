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

    std::vector<std::pair<int, int>> cyan_coords = {
        {-2,-1},{-2,0},
                {-1,0},
         {0,-1},{0,0},{0,1},
                {1,0},
    };

    std::vector<std::pair<int, int>> pink_coords = {
        {-2,-1},{-2,0},
                {-1,0},
         {0,-1},{0,0},{0,1},{0,2},
                {1,0},
    };

    std::vector<std::pair<int, int>> yellow_coords = {
        {-2,-1},{-2,0},
                {-1,0},
         {0,-1},{0,0},{0,1},{0,2},
                {1,0},
                {2,0},
    };

    std::vector<std::pair<int, int>> green_coords = {
        {-2,-1},{-2,0},
                {-1,0},
  {0,-2},{0,-1},{0,0},{0,1},{0,2},
                {1,0},
                {2,0},
    };

    std::vector<std::pair<int, int>> lime_coords = {
        {-2,-1},{-2,0},
                {-1,0},
                {0,0},
  {1,-2},{1,-1},{1,0},{1,1},
                {2,0},
    };

    std::vector<std::pair<int, int>> purple_coords = {
             {-3,-1},{-3,0},
                     {-2,0},
                     {-1,0},
{0,-3},{0,-2},{0,-1},{0,0},{0,1},
                     {1,0}
    };

    //Names and symbols must be unique! Also, symbols can't be ' '. (Both of these requirements are checked in stage 0)
    Piece red = Piece("red", 'r', {Orientation(red_coords, genericBB)});
    Piece blue = Piece("blue", 'b', {Orientation{blue_coords, genericBB}});
    Piece orange = Piece("orange", 'o', {Orientation(orange_coords, genericBB)});
    Piece cyan = Piece("cyan", 'c', {Orientation(cyan_coords, genericBB)});
    Piece pink = Piece("pink", 'k', {Orientation(pink_coords, genericBB)});
    Piece yellow = Piece("yellow", 'y', {Orientation(yellow_coords, genericBB)});
    Piece green = Piece("green", 'g', {Orientation(green_coords, genericBB)});
    Piece lime = Piece("lime", 'l', {Orientation(lime_coords, genericBB)});
    Piece purple = Piece("purple", 'p', {Orientation(purple_coords, genericBB)});

    //recall that if keys are repeated, only the more recent entry will be included
    const std::unordered_map<std::string, Piece*> piece_names = {
        {"red", &red},
        {"blue", &blue},
        {"orange", &orange},
        {"cyan", &cyan},
        {"pink", &pink},
        {"yellow", &yellow},
        {"green", &green},
        {"lime", &lime},
        {"purple", &purple},
    };
}