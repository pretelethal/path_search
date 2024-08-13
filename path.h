#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <iterator>
#include <climits>
#include <limits>
#include <stdlib.h>
#include <time.h>
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"

class PathFinder
{
   public:
      PathFinder();
      PathFinder(int);
      ~PathFinder();
      void openMapFromFile(std::string);
      void setGridSideLength(int);
      void generateRandomMap();
      std::pair<int,int> generatePostion();
      void setPostion(std::pair<int,int>, int);
      bool elementExistsOnBoard(int);
      void findShortestPath(std::pair<int, int>&, std::pair<int, int>&);
      void printShortestPath();
      void printBoard();
      bool isBoardEmpty();

    private:
      void findShortestPathUtil(std::vector<std::vector<int>>&, std::deque<std::pair<int,int>>, std::deque<std::pair<int,int>>&, int, int, int, int, int&, int);
      std::vector<std::vector<int>> convertVectorToMultiVector(std::vector<int>&, int);
      bool isObstacle(int, int);
      bool isValid(int, int);
      bool isShorter(std::vector<std::vector<int>>&, int, int, int);
      bool distExceeded(int&, int);
      bool canTraverse(std::vector<std::vector<int>>&, int&, int, int, int);
      std::deque<std::pair<int,int>> shortest_path;
      std::vector<std::vector<int>> board;
      int SIDE_LEN = 64;
};