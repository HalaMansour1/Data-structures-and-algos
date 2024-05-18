/**
 * @file puzzle.cpp
 * Implementation of puzzle class.
 */
#include "puzzle.h"
#include <vector>
#include <array>
#include <iostream>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <algorithm> 


PuzzleState::PuzzleState() {
    // Initialize the puzzle to the solved state.
    for (int i = 0; i < 15; i++) {
        this->state[i] = i + 1;  
    }
    this->state[15] = 0;  // The empty space
}

PuzzleState::PuzzleState(const std::array<uint8_t, 16> inputState) {
  
        this->state = inputState;
    
}

bool PuzzleState::operator==(const PuzzleState &rhs) const {
    
    for (int i = 0; i < 16; i++) {
        if (this->state[i] != rhs.state[i]) {
            return false;  
        }
    }
    return true;  
}

bool PuzzleState::operator!=(const PuzzleState &rhs) const {
    
    for (int i = 0; i < 16; i++) {
        if (this->state[i] != rhs.state[i]) {
            return true;  
        }
    }
    return false;  
}

bool PuzzleState::operator<(const PuzzleState &rhs) const {
    
  for (size_t i = 0; i < state.size(); i++) {
        if (this->state[i] != rhs.state[i]) {
            return this->state[i] < rhs.state[i];
        }
    }
    return false; // Return false i
    
   // return false; // Return false if no difference is found,
}


PuzzleState PuzzleState::getNeighbor(Direction direction) const {
    int empty = -1;

    for (int i = 0; i < 16; i++) {
        if (this->state[i] == 0) {
            empty = i;
            break;
        }
      //  std::cout<< this->state[i]<< std::endl ;
        
       
    }
     
    
    int row = empty / 4;  
    int col = empty % 4;  

    int newRow = row, newCol = col;

    switch (direction) {
        case Direction::UP:
            newRow = row + 1;
            break;
        case Direction::DOWN:
            newRow = row - 1;
            break;
        case Direction::LEFT:
            newCol = col + 1;
            break;
        case Direction::RIGHT:
            newCol = col - 1;
            break;
    }
    
    if (newRow < 0 || newRow >= 4 || newCol < 0 || newCol >= 4) {
        return PuzzleState(std::array<uint8_t, 16>{}); // Return state with all zeros 
    }

    // Compute new index for the empty space 
    int newIndex = newRow * 4 + newCol;
    std::array<uint8_t, 16> newState = this->state; // Copy current state
    // Swap empty space with the target tile
    std::swap(newState[empty], newState[newIndex]);
    // for(int i =0; i < 15 ; i++){
    //  std::cout<< newState[i]<< std::endl ; 
    // }
    // for(int i =0; i < 15 ; i++){
    //  std::cout<< this->state[i]<< std::endl ; 
    // }
    return PuzzleState(newState); // Return new state
}
std::array<uint8_t, 16> PuzzleState::asArray() const {
    return this->state;
}
std::vector<PuzzleState> PuzzleState::getNeighbors() const {
    std::vector<PuzzleState> neighbors;
    int empty = -1;
    
    // Find the index of the empty space
    for (int i = 0; i < 16; i++) {
        if (this->state[i] == 0) {
            empty = i;
            break;
        }
    }

    // Calculate the row and column of the empty space
    int row = empty / 4;
    int col = empty % 4;

    // potential moves 
    std::vector<std::pair<int, int>> moves = {
        {row - 1, col}, 
        {row + 1, col}, 
        {row, col - 1}, 
        {row, col + 1}  
    };

    for (const auto& move : moves) {
        int newRow = move.first;
        int newCol = move.second;
        
        // within the grid boundaries
        if (newRow >= 0 && newRow < 4 && newCol >= 0 && newCol < 4) {
            int newIndex = newRow * 4 + newCol;
            std::array<uint8_t, 16> newState = this->state;
            // Swap the empty space 
            std::swap(newState[empty], newState[newIndex]);
            neighbors.push_back(PuzzleState(newState));
        }
    }

    return neighbors;
}



int PuzzleState::manhattanDistance(const PuzzleState desiredState) const {
    int distance = 0;
    
    int gridSize = std::sqrt(state.size());

    std::array<int, 16> currentRow, currentCol;
     std::array<int, 16> newRow, newCol;
    // initialize the positions in the current state
    for (unsigned long i = 0; i < state.size(); i++) {
        currentRow[state[i]] = i / gridSize; // Row
        currentCol[state[i]] = i % gridSize; // Column 
    }

    // initialize goal state
    for (unsigned long i = 0; i < desiredState.state.size(); i++) {
        newRow[desiredState.state[i]] = i / gridSize; // Row 
        newCol[desiredState.state[i]] = i % gridSize; // Column 
    }

    for (unsigned long tile = 1; tile < state.size(); tile++) {
        distance += abs(currentRow[tile] - newRow[tile]) + abs(currentCol[tile] - newCol[tile]);
    }

    return distance;
}

namespace std {
    template<>
    struct hash<PuzzleState> { // making a slay hash function 
        size_t operator()(const PuzzleState& p) const {
            size_t h = 0;
            for (uint8_t val : p.state) {
                h ^= std::hash<uint8_t>()(val) + 0x2 + (h << 8) ;
            }
            return h;
        }
    };
}
std::vector<PuzzleState> solveBFS(const PuzzleState& startState, const PuzzleState& desiredState, size_t* iterations) {
    std::queue<PuzzleState> q;
    std::unordered_map<PuzzleState, PuzzleState> parent;
    std::vector<PuzzleState> path;
    size_t iter = 0;

    q.push(startState);
    parent[startState] = startState;

    while (!q.empty()) {
        PuzzleState current = q.front();
        q.pop();
        iter++;

        if (current == desiredState) {
            break;
        }

        for (PuzzleState neighbor : current.getNeighbors()) {
            if (parent.find(neighbor) == parent.end()) {
                q.push(neighbor);
                parent[neighbor] = current;
            }
        }
    }

    if (iterations) {
        *iterations = iter;
    }


    PuzzleState step = desiredState;
    while (step != startState) {
        path.push_back(step);
        step = parent[step];
    }
    path.push_back(startState);
    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<PuzzleState> solveAstar(const PuzzleState& startState, const PuzzleState& desiredState, size_t* iterations) {
    auto comp = [](const std::pair<PuzzleState, int>& a, const std::pair<PuzzleState, int>& b) { return a.second > b.second; };
    std::priority_queue<std::pair<PuzzleState, int>, std::vector<std::pair<PuzzleState, int>>, decltype(comp)> frontier(comp);
    std::unordered_map<PuzzleState, PuzzleState> parent;
    std::unordered_map<PuzzleState, int> cost_so_far;
    std::vector<PuzzleState> result;  // declaring of path
    size_t iter = 0;
// initliaze everything to zero 
    frontier.emplace(startState, 0);
    parent[startState] = startState;
    cost_so_far[startState] = 0;

    while (!frontier.empty()) {
        PuzzleState current = frontier.top().first;
        frontier.pop();
        iter++;

        if (current == desiredState) {
            for (PuzzleState at = desiredState; at != startState; at = parent[at]) {
            result.push_back(at);  //  using path here
            }
            result.push_back(startState);
            std::reverse(result.begin(), result.end());
            if (iterations) *iterations = iter;
            return result;
        }

        for (PuzzleState next : current.getNeighbors()) {
            int new_cost = cost_so_far[current] + 1;
            if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                int priority = new_cost + next.manhattanDistance(desiredState);
                frontier.emplace(next, priority);
                parent[next] = current;
            }
        }
    }

    if (iterations) *iterations = iter;
    return result; 
}