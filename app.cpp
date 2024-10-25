// Author:         Francis K. Ferdinard

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <fstream>

using namespace std;

int N;

const int dx[] = {0, 0, -1, 1};
const int dy[] = {-1, 1, 0, 0};

bool isSafe(int x, int y, vector<vector<int>>& maze, vector<vector<bool>>& visited) {
    return (x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 0 && !visited[x][y]);
}

void solveMazeUtil(int x, int y, vector<vector<int>>& maze, vector<vector<bool>>& visited,
                   vector<pair<int, int>>& path, vector<vector<pair<int, int>>>& allPaths) {
    if (x == N - 1 && y == N - 1) {
        path.push_back({x, y});
        allPaths.push_back(path);
        path.pop_back(); 
        return;
    }

    if (isSafe(x, y, maze, visited)) {
        visited[x][y] = true;
        path.push_back({x, y});

        for (int i = 0; i < 4; i++) {
            int newX = x + dx[i];
            int newY = y + dy[i];
            solveMazeUtil(newX, newY, maze, visited, path, allPaths);
        }

        path.pop_back();
        visited[x][y] = false;
    }
}

void printMazeWithPaths(vector<vector<int>>& maze, const vector<pair<int, int>>& path) {
    vector<vector<char>> displayMaze(N, vector<char>(N, ' '));
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            displayMaze[i][j] = (maze[i][j] == 1) ? '#' : '.';
        }
    }

    for (const auto& p : path) {
        displayMaze[p.first][p.second] = '*';
    }

    cout << "Maze with paths:\n";
    for (const auto& row : displayMaze) {
        for (char cell : row) {
            cout << cell << ' ';
        }
        cout << endl;
    }
}

void solveMaze(vector<vector<int>>& maze) {
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    vector<pair<int, int>> path;
    vector<vector<pair<int, int>>> allPaths;

    solveMazeUtil(0, 0, maze, visited, path, allPaths);

    if (allPaths.empty()) {
        cout << "No path to the exit exists." << endl;
    } else {
        cout << "Paths to the exit found: " << allPaths.size() << endl;
        for (const auto& p : allPaths) {
            for (const auto& coords : p) {
                cout << "(" << coords.first << ", " << coords.second << ") ";
            }
            cout << endl;
        }
        printMazeWithPaths(maze, allPaths[0]); // Print only the first path
    }
}

bool isValidMaze(const vector<vector<int>>& maze) {
    return (N > 0 && maze.size() == N && maze[0].size() == N);
}

int main() {
    cout << "Enter the filename of the maze: ";
    string filename;
    cin >> filename;

    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    infile >> N; // Read the size of the maze
    vector<vector<int>> maze(N, vector<int>(N));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            infile >> maze[i][j];
            if (maze[i][j] != 0 && maze[i][j] != 1) {
                cerr << "Invalid value in maze at (" << i << ", " << j << "). Must be 0 or 1." << endl;
                return 1;
            }
        }
    }

    infile.close(); 

    if (!isValidMaze(maze)) {
        cout << "Invalid maze input." << endl;
        return 1;
    }

    solveMaze(maze);
    return 0;
}