#include "path.h"

int main () {
    PathFinder pathFinder = PathFinder();
    
    int l = -1;
    std::cout << "Enter length/width of the map: ";
    std::cin >> l;

    while (!std::cin.good() || l < 0)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (l < 0)
            break;
        std::cin >> l;
    }

    pathFinder.setGridSideLength(l);
    
    std::string filePath = "map.json";
    std::cout << "Enter file path and name to JSON Map: ";
    std::cin >> filePath;

    pathFinder.openMapFromFile(filePath);
    if (pathFinder.isBoardEmpty()) {
        std::cout << "File path not found or map data empty. Defaulting to random generated map." << std::endl;
        pathFinder.generateRandomMap();
    } else {
        std::cout << "File found!" << std::endl;
    }
    if (!pathFinder.elementExistsOnBoard(-1)) {
        std::cout << "No valid space on map!" << std::endl;
        return 0;
    }
    pathFinder.printBoard();

    std::cout << "Enter a valid x, y position for the Unit. (0,0 starts at top left)" << std::endl;
    std::cout << "Enter -1 and -1 for random values." << std::endl;

    int a, b = -1;
    std::cin >> a >> b;
    while (!std::cin.good())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> a >> b;
    }

    std::pair<int,int> unitPair;
    if (a == -1 || b == -1) {
        unitPair = pathFinder.generatePostion();
        std::cout << "Random unit position: " << "{" << unitPair.first << "," << unitPair.second << "}" << std::endl;
    } else {
        unitPair = std::make_pair(a,b);
        std::cout << "Unit position: " << "{" << unitPair.first << "," << unitPair.second << "}" << std::endl;
    }
    pathFinder.setPostion(unitPair, 0);

    std::cout << "Enter a valid x, y position for the end position. (0,0 starts at top left)" << std::endl;
    std::cout << "Enter -1 and -1 for random values." << std::endl;

    a,b = -1;
    std::cin >> a >> b;
    while (!std::cin.good())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> a >> b;
    }

    std::pair<int,int> destPair;
    if (a == -1 || b == -1) {
        destPair = pathFinder.generatePostion();
        std::cout << "Random end position: " << "{" << destPair.first << "," << destPair.second << "}" << std::endl;
    } else {
        destPair = std::make_pair(a,b);
        std::cout << "End position: " << "{" << destPair.first << "," << destPair.second << "}" << std::endl;
    }
    pathFinder.setPostion(destPair, 6);

    std::cout << "Determining path..." << std::endl;
    pathFinder.findShortestPath(unitPair, destPair);
    pathFinder.printShortestPath();

    return 0;
}