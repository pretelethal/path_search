#include "path.h"

PathFinder::PathFinder() {
    srand(time(NULL));
}

PathFinder::PathFinder(int l) : PathFinder::PathFinder() {
    SIDE_LEN = l;
}

PathFinder::~PathFinder() {
}

/**
 * Convert vector to multi dim vector
 */
std::vector<std::vector<int>> PathFinder::convertVectorToMultiVector(std::vector<int>& vect, int side_length) {
    std::vector<std::vector<int>> multiVector;

    std::vector<int> tempVect;
    for (int i = 0; i < vect.size(); i++) {
        tempVect.push_back(vect[i]);
        if ((i+1) % side_length == 0) {
            multiVector.push_back(tempVect);
            tempVect.clear();
        }
    }

    return multiVector;
}

/**
 * Generate random multi vector
 */
void PathFinder::generateRandomMap()
{
    int len = SIDE_LEN * SIDE_LEN;
    std::vector<int> randVector(len, -1);
    for (int i = 0; i < len; i++) {
        int v1 = rand() % 5;
        if (v1 == 1)
            randVector[i] = 4;
    }

    board = convertVectorToMultiVector(randVector, SIDE_LEN);
}

/**
 * Open and parse the json doc if formatted correctly
 */
void PathFinder::openMapFromFile(std::string filePath) {
    std::vector<int> vect;

    FILE* fp = fopen(filePath.c_str(), "rb");

    if (fp == NULL) {
        return;
    }
  
    char readBuffer[65536]; 
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer)); 
  
    rapidjson::Document doc; 
    doc.ParseStream(is); 
  
    fclose(fp);

    if (doc.HasMember("layers") && doc["layers"].IsArray()) { 
        const rapidjson::Value& layers = doc["layers"]; 
        for (rapidjson::SizeType i = 0; i < layers.Size(); i++) { 
            if (layers[i].IsObject()) { 
                auto itr = layers[i].FindMember("data");
                if (itr != layers[i].MemberEnd() && itr->value.IsArray()) {
                    auto arr = itr->value.GetArray();
                    for (auto& a : arr)
                        vect.push_back(a.GetInt());
                }
            } 
        } 
    }

    board = convertVectorToMultiVector(vect, SIDE_LEN);
}

/**
 * Print the multi vector
 */
void PathFinder::printBoard() {
    int i = 0;
    for (auto it = begin(board); it != end(board); ++it) {

        std::stringstream ss;
        std::copy( it->begin(), it->end(), std::ostream_iterator<int>(ss, " ") );
        std::string s = ss.str();
        s = s.substr(0, s.length()-1);  // get rid of the trailing space

        std::cout << "Row " << i << ": [" << s << "]" << std::endl;
        i++;
    }
}

/**
 * Check if board is empty
 */
bool PathFinder::isBoardEmpty() {
    if (board.size() == 0)
        return true;

    return false;
}

/**
 * Check if there is a natural obstacle on the grid
 */
bool PathFinder::isObstacle(int x, int y) {
    if (board[y][x] == 4)
        return false;
 
    return true;
}

/**
 * Check if postion is a valid coordinate on the grid
 */
bool PathFinder::isValid(int x, int y) {
    if (x >= 0 && y >= 0 && x < SIDE_LEN && y < SIDE_LEN)
        return true;
 
    return false;
}

/**
 * Check if the distance traveled will exceed distance weight
 */
bool PathFinder::isShorter(std::vector<std::vector<int>>& visited, int dist, int x, int y) {
    if (dist < visited[y][x] || visited[y][x] == 0)
        return true;
 
    return false;
}

/**
 * Check if the distance traveled will exceed already stored path distance
 */
bool PathFinder::distExceeded(int& min_dist, int dist) {
    if (dist >= min_dist)
        return true;

    return false;
}

/**
 * Check if we can move from the current position
 */
bool PathFinder::canTraverse(std::vector<std::vector<int>>& visited, int& min_dist, int dist, int x, int y) {
    if (isValid(x, y) && isObstacle(x, y) && isShorter(visited, dist, x, y) && !distExceeded(min_dist, dist))
        return true;

    return false;
}

/**
 * Check if element exists in multi vector
 */
bool PathFinder::elementExistsOnBoard(int e) {
    for (int y = 0; y < board.size(); y++) {
        for (int x = 0; x < board[0].size(); x++) {
            if (board[y][x] == e) {
                return true;
            }
        }
    }
    return false;
}

/**
 * Print the shortest path
 */
void PathFinder::printShortestPath() {
    if (shortest_path.size() == 0) {
        std::cout << "No path found." << std::endl;
        return;
    } else {
        std::cout << "Path found!" << std::endl;
    }

    int k = 0;
    for (auto it = std::begin(shortest_path); it != std::end(shortest_path); ++it) {
        if (k == 16) {
            k = 0;
            std::cout << std::endl;
        } else {
            k++;
        }
        std::cout << "{" << it->first << ", " << it->second << "}, ";
    }
    std::cout << std::endl;
}

/**
 * Set a randomly generated position on the board
 */
std::pair<int,int> PathFinder::generatePostion() {
    int a = -1;
    int b = -1;

    while (!isValid(a,b) || isObstacle(a,b)) {
        a = rand() % SIDE_LEN;
        b = rand() % SIDE_LEN;
    }

    return std::make_pair(a,b);
}

/**
 * Set's the position of an element onto the board
 */
void PathFinder::setPostion(std::pair<int,int> coord, int type) {
    board[coord.second][coord.first] = type;
}

/**
 * Manually set the grid side length, riskylab json doesn't seem to export that data
 */
void PathFinder::setGridSideLength(int l) {
    SIDE_LEN = l;
}

/**
 * Recursively find all viable paths via backtracking, shortest of which will be the one stored
 * This algorithm is the core of the project
 */
void PathFinder::findShortestPathUtil(std::vector<std::vector<int>>& visited, std::deque<std::pair<int,int>> path, std::deque<std::pair<int,int>>& min_path, int i, int j, int x, int y, int& min_dist, int dist) {
    if (i == x && j == y) {
        if (dist < min_dist) {
            path.push_back(std::make_pair(i,j));
            min_dist = dist;
            min_path = path;
        }
        return;
    }

    visited[j][i] = dist;
    path.push_back(std::make_pair(i,j));

    if (canTraverse(visited, min_dist, dist + 1, i + 1, j)) {
        findShortestPathUtil(visited, path, min_path, i + 1, j, x, y, min_dist, dist + 1);
    }
    if (canTraverse(visited, min_dist, dist + 1, i, j + 1)) {
        findShortestPathUtil(visited, path, min_path, i, j + 1, x, y, min_dist, dist + 1);
    }
    if (canTraverse(visited, min_dist, dist + 1, i - 1, j)) {
        findShortestPathUtil(visited, path, min_path, i - 1, j, x, y, min_dist, dist + 1);
    }
    if (canTraverse(visited, min_dist, dist + 1, i, j - 1)) {
        findShortestPathUtil(visited, path, min_path, i, j - 1, x, y, min_dist, dist + 1);
    }
}

/**
 * Finds shortest path to the destination from the source if the path is unblocked
 */
void PathFinder::findShortestPath(std::pair<int, int>& src, std::pair<int, int>& dest) {
    if (board.size() == 0 || board[src.second][src.first] != 0 || board[dest.second][dest.first] != 6) 
        return;

    int row = board.size(); //y
    int col = board[0].size(); //x

    std::vector<std::vector<int>> visited;
    visited.resize(row, std::vector<int>(col));

    std::deque<std::pair<int,int>> path;
    std::deque<std::pair<int,int>> min_path;

    int max_dist = (row * col) / 2;
    int min_dist = max_dist;
    findShortestPathUtil(visited, path, min_path, src.first, src.second, dest.first, dest.second, min_dist, 0);

    if (min_dist < max_dist)
        shortest_path = min_path;
}