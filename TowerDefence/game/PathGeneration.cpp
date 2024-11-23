#include <iostream>
#include <vector>
#include <random>
#include <time.h>

using namespace std;

struct  gridspace {
    int type;
    int i;
    int j;
    float local;
    float global;
    gridspace* parent;
    bool isVisited;
    // https://stackoverflow.com/questions/14047191/overloading-operators-in-typedef-structs-c
    int operator-(const gridspace& other) const {
        // Calculates Manhattan distance
        return abs(other.i - i) + abs(other.j - j);
    }
    int dis(const gridspace* other) const {
        // Calaculates Pythagoras distance
        return sqrt(pow(other->i - i, 2) + pow(other->j - j, 2));
    }
};


class PathGenerator {
    // Credit to Ewan for the help with creating a Singleton
public:
    static PathGenerator* Get() {
        static PathGenerator* generator = new PathGenerator();
        return generator;
    }
private:
    vector<vector<gridspace>> grid;
    vector<vector<gridspace>> GenerateNoiseGrid(int noise, int height, int width) {
        vector<vector<gridspace>> noiseGrid = {};
        for (int i = 0; i < height + 2; i++) {
            vector<gridspace> row = {};
            for (int j = 0; j < width + 2; j++) {
                int random = 1 + rand() % 100;
                gridspace point; point.i = i; point.j = j; point.local = INFINITY; point.global = INFINITY; point.isVisited = false; point.parent = nullptr;
                if (random > noise) {
                    point.type = 0;
                    row.push_back(point);
                }
                else {
                    point.type = 1;
                    row.push_back(point);
                }
            }
            noiseGrid.push_back(row);
        }
        return noiseGrid;
    }

    void ShowGrid(vector<vector<gridspace>> grid) {
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[i].size(); j++) {
                cout << grid[i][j].type << " ";
            }
            cout << endl;
        }
    }


    void ApplyCellularAutomata(int iterations) {
        for (int i = 0; i < iterations; i++) {
            vector<vector<gridspace>> copyGrid;
            copyGrid.assign(grid.begin(), grid.end());
            for (int j = 0; j < grid.size(); j++) {
                for (int k = 0; k < grid[j].size(); k++) {
                    // Checks all the neighbours of a space
                    int numberOfNeighboursWalls = 0;
                    for (int y = j - 1; y <= j + 1; y++) {
                        for (int x = k - 1; x <= k + 1; x++) {
                            if (y >= 0 && y < grid.size() && x >= 0 && x < grid[j].size()) {
                                if (y != j || x != k) {
                                    // If it is a 1 it adds this to the number of Neighbours
                                    if (copyGrid[y][x].type == 1) {
                                        numberOfNeighboursWalls++;
                                    }
                                }
                            }
                            else {
                                numberOfNeighboursWalls++;
                            }
                        }
                    }
                    // Follows the rules that I have decided to use: if there are more than four 1s around a space it is a 1, instead the space becomes a 0
                    if (numberOfNeighboursWalls > 4) {
                        grid[j][k].type = 1;
                    }
                    else {
                        grid[j][k].type = 0;
                    }
                }
            }
            cout << endl;
            ShowGrid(grid);
        }
    }

    vector<gridspace> GetStartingPositions() {
        vector<gridspace> startingPoints = {};
        // Cannot decide top and bottom row
        for (int i = 1; i < grid.size() - 1; i++) {
            if (grid[i][1].type == 0 && grid[i][2].type == 0) {
                startingPoints.push_back(grid[i][1]);
                //cout << endl << startingPoints.back()->i << " " << startingPoints.back()->j;
            }
        }
        return startingPoints;
    }

    vector<gridspace> GetEndingPositions() {
        vector<gridspace> endingPoints = {};
        // Cannot decide top and bottom row
        for (int i = 1; i < grid.size() - 1; i++) {
            if (grid[i][grid[i].size() - 2].type == 0 && grid[i][grid[i].size() - 3].type == 0) {
                endingPoints.push_back(grid[i][grid[i].size() - 2]);
                //cout << endl << endingPoints.back()->i << " " << endingPoints.back()->j;
            }
        }
        return endingPoints;
    }

    // A* algorithm
    int FindBestPath(vector<vector<gridspace>>& grid, gridspace* start, gridspace* end) {
        start->local = 0;
        start->global = end - start;

        vector<gridspace*> notVisitedSpaces = { start };
        while (notVisitedSpaces.size() != 0) {
            // Removes the spaces that have already been visited, this is done, to make sure we do not search the same block multiple times
            if (notVisitedSpaces.front()->isVisited) {
                notVisitedSpaces.erase(notVisitedSpaces.begin());
                continue;
            }
            // Makes sure that the space that we are searching is the better space to search
            sort(notVisitedSpaces.begin(), notVisitedSpaces.end(), [](gridspace* a, gridspace* b) { return a->global < b->global; });
            // Selects the first in the spaces that have to be visited, and sets visited to true, again to make sure that it is not search more than once
            gridspace* currentSearchSpace = notVisitedSpaces.front();
            currentSearchSpace->isVisited = true;
            // Looks at the neighbouring blocks and checks them, if they are present
            for (int y = 0; y < grid.size(); y++) {
                for (int x = 0; x < grid[y].size(); x++) {
                    // A space will be a neighbour only if the Manhattan distance between them is 1
                    if (grid[y][x] - *currentSearchSpace == 1) {
                        if (grid[y][x].type == 0) {
                            if (!grid[y][x].isVisited) notVisitedSpaces.push_back(&grid[y][x]);
                            // Then it checks if this neighbour is closer to the end than what it has in the local variable
                            float checkIfBetterSpace = currentSearchSpace->local + (grid[y][x] - *currentSearchSpace);
                            if (checkIfBetterSpace < grid[y][x].local) {
                                grid[y][x].parent = currentSearchSpace;
                                grid[y][x].local = checkIfBetterSpace;
                                grid[y][x].global = grid[y][x].local + (*end - grid[y][x]);
                            }
                        }
                    }
                }
            }
            notVisitedSpaces.erase(notVisitedSpaces.begin());
        }
        // Starts from the end and looks at the parent of it, until it gets back to the start, which has the neighbour set to nullptr
        int pathLength = 0;
        gridspace* lookSpace = end;
        do {
            // Marks the grid with a 2 where the path is and keeps track of the length of the path
            lookSpace->type = 2;
            pathLength++;
            lookSpace = lookSpace->parent;
        } while (lookSpace != nullptr);
        return pathLength;
    }

    int FindNumberOfTurns(gridspace* end) {
        int numberOfTurns = 0;
        gridspace* lookSpace = end;
        do {
            // By using the pythagoras distance between the current space and the parent of the next space, we can find if the parent of the current space is a turn.
            // This is because it will be 2 when there is no turn, but it will be sqrt(2) when there is a turn.
            if (lookSpace->dis(lookSpace->parent->parent) != 2) {
                lookSpace->parent->type = 3;
                numberOfTurns++;
            }
            lookSpace = lookSpace->parent;
        } while (lookSpace->parent->parent != nullptr);

        return numberOfTurns;
    }

    vector<gridspace> GetPath(vector<vector<gridspace>>& grid, gridspace* end) {
        vector<gridspace> path = {};
        gridspace* lookSpace = end;
        while (lookSpace != nullptr) {
            // Removes every 1 near the path.
            for (int y = lookSpace->i - 1; y <= lookSpace->i + 1; y++) {
                for (int x = lookSpace->j - 1; x <= lookSpace->j + 1; x++) {
                    if (y >= 0 && y < grid.size() && x >= 0 && x < grid[lookSpace->i].size()) {
                        if (y != lookSpace->i || x != lookSpace->j) {
                            if (grid[y][x].type == 1) {
                                grid[y][x].type = 0;
                            }
                        }
                    }
                }
            }
            path.push_back(*lookSpace);
            lookSpace = lookSpace->parent;
        }
        reverse(path.begin(), path.end());
        return path;
    }
    void AddOuterBorder() {
        // Adds a coloumn of 4, both in the front and back of the grid.
        for (int i = 0; i < grid.size(); i++) {
            gridspace point2; point2.i = grid[i][0].i; point2.j = -1; point2.type = 4;
            grid[i].insert(grid[i].begin(), point2);
            gridspace point; point.i = grid[i][0].i; point.j = grid[i].size(); point.type = 4;
            grid[i].push_back(point);
        }

        // Adds a row of 4, both at the top and bottom of the grid.
        vector<gridspace> lastrow = {};
        for (int i = 0; i < grid[0].size(); i++) {
            gridspace point; point.i = grid.size(); point.j = i; point.type = 4;
            lastrow.push_back(point);
        }
        grid.push_back(lastrow);
        vector<gridspace> firstrow = {};
        for (int i = 0; i < grid[0].size(); i++) {
            gridspace point; point.i = -1; point.j = i; point.type = 4;
            firstrow.push_back(point);
        }
        grid.insert(grid.begin(), firstrow);
    }
public:
    vector<gridspace> GenerateRandomPath(int minimumPathLength, int minimumNumberOfTerms) {
        while (true) {
            grid = GenerateNoiseGrid(47, 20, 22);
            cout << endl;
            ShowGrid(grid);
            ApplyCellularAutomata(1);

            vector<gridspace> startPoints = GetStartingPositions();
            vector<gridspace> endPoints = GetEndingPositions();
            // Loops through every starting and ending point
            for (gridspace& start : startPoints) {
                for (gridspace& end : endPoints) {
                    // Takes a copy of the grid, so it can apply the path finding in only that version, without affecting subsequent checks.
                    vector<vector<gridspace>> gridCopy = {};
                    gridCopy.assign(grid.begin(), grid.end());
                    int pathLength = FindBestPath(gridCopy, &gridCopy[start.i][start.j], &gridCopy[end.i][end.j]);
                    // If a path exists
                    if (pathLength != 1) {
                        // Sets the starting and ending point to 5 and 6 respectively
                        gridCopy[start.i][start.j - 1].type = 5;
                        gridCopy[start.i][start.j - 1].parent = nullptr;
                        gridCopy[start.i][start.j].parent = &gridCopy[start.i][start.j - 1];
                        gridCopy[end.i][end.j + 1].type = 6;
                        gridCopy[end.i][end.j + 1].parent = &gridCopy[end.i][end.j];
                        pathLength += 2;
                        // It considers the path only if the minimum path length is met
                        if (pathLength >= minimumPathLength) {
                            // Gets the numbers of turn present in the path and marks them in the grid
                            int numberOfTurns = FindNumberOfTurns(&gridCopy[end.i][end.j + 1]);
                            // Again there a minimum amount of turns have to be met
                            if (numberOfTurns > minimumNumberOfTerms) {
                                cout << endl;
                                ShowGrid(gridCopy);
                                cout << endl << pathLength << " " << numberOfTurns << endl;
                                // Gets the paths from the grid and removes and 1s around it
                                vector<gridspace> selectedPath = GetPath(gridCopy, &gridCopy[end.i][end.j + 1]);
                                ShowGrid(gridCopy);
                                cout << endl;
                                grid = gridCopy;
                                // Adds the outer border of 4s around the grid
                                AddOuterBorder();
                                ShowGrid(grid);
                                return selectedPath;
                            }
                        }
                    }
                }
            }
        }
    }
    vector<vector<gridspace>> GetGrid() {
        return grid;
    }
};






