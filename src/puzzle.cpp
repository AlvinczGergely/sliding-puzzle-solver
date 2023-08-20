#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <random>
#include <chrono>
#include <cmath>
#include <queue>
#include <map>
#include <set>



struct puzzle
{
  std::vector<int> numbers = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  std::vector<int> goal_numbers = {1, 2, 3, 4, 5, 6, 7, 8, 0};
  std::vector<std::vector<int>> vec
	{
		{0, 1, 2},  
		{3, 4, 5},  
		{6, 7, 8}   
	};

  struct Puzzle {
    std::vector<int> state;
    int g = 0;
    int h = 0;
    int f = 0;
    Puzzle* parent;
  };

  struct comparePuzzle
  {
    bool operator() (Puzzle* p1, Puzzle* p2)
    {
      return p1->f > p2->f;
   }
};

  //randomize the numbers from 0 to 8
  void vec_shuffl(std::vector<int>& numbers)
  {
    // Generate seed based on current time
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    unsigned seed = static_cast<unsigned>(duration.count());

    // Shuffle the vector
    std::mt19937 gen(seed);
    std::shuffle(numbers.begin(), numbers.end(), gen);
  }

  //Filling up the 2D array with the rander ordered numbers from 0 to 8
  void fill(std::vector<int>& numbers, std::vector<std::vector<int>>& vec)
  {
    int index = 0;
    for(int i = 0; i < 3; ++i)
      {
        for(int j = 0; j < 3; ++j)
        {
          vec[i][j] = numbers[index++];
        }
      }
  }

  //Pring function for the 2D vector
  void print( std::vector<std::vector<int>> vec)
  {
    for(int i = 0; i < 3; ++i)
      {
        for(int j = 0; j < 3; ++j)
        {
          std::cout << vec[i][j] << ", " ;
        }
        std::cout << std::endl;
      }
  }

  //Check that the puzzle is solvabel
  bool puzzle_solvabel(const std::vector<int>& puzzle)
  {
    bool solva = false;
    int inversions = 0;
    int size = puzzle.size();
    
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (puzzle[i] != 0 && puzzle[j] != 0 && puzzle[i] > puzzle[j]) {
                inversions++;
            }
        }
    }
    
    if(inversions % 2 == 0)
    {
      solva = true;
    }
    return solva;
  }

  int manhattanDistance(const std::vector<int>& puzzle)
  {
    int distance = 0;
    int size = puzzle.size();
    
    for (int i = 0; i < size; i++) {
        if (puzzle[i] != 0) { // Don't calculate Manhattan distance for the empty tile
            // Actual position of the tile in a solved puzzle
            int x = (puzzle[i] - 1) / 3;
            int y = (puzzle[i] - 1) % 3;
            
            // Current position of the tile
            int curr_x = i / 3;
            int curr_y = i % 3;
            
            // Add the Manhattan distance for this tile to the total distance
            distance += abs(x - curr_x) + abs(y - curr_y);
        }
    }
    
    return distance;
  }

  // Function to swap two elements in a puzzle state
  void swap(std::vector<int>& state, int i, int j)
  {
    int temp = state[i];
    state[i] = state[j];
    state[j] = temp;
  }


  // Function to generate the successors of a puzzle state
    std::vector<Puzzle*> generateSuccessors(Puzzle* puzzle) {
    std::vector<Puzzle*> successors;
    std::vector<int> state = puzzle->state;
    int size = state.size();
    int i = 0;

    // Find the position of the empty tile (0)
    for (; i < size; i++) {
        if (state[i] == 0) {
            break;
        }
    }

    // Generate successors
    // Move the empty tile up if possible
    if (i >= 3) {
        std::vector<int> newState = state; // Create a copy of the state
        swap(newState, i, i - 3);
        Puzzle* successor = new Puzzle{newState, puzzle->g + 1, puzzle->h, 0, puzzle};
        successors.push_back(successor);
    }

    // Move the empty tile down if possible
    if (i < size - 3) {
        std::vector<int> newState = state; // Create a copy of the state
        swap(newState, i, i + 3);
        Puzzle* successor = new Puzzle{newState, puzzle->g + 1, puzzle->h, 0, puzzle};
        successors.push_back(successor);
    }

    // Move the empty tile left if possible
    if (i % 3 != 0) {
        std::vector<int> newState = state; // Create a copy of the state
        swap(newState, i, i - 1);
        Puzzle* successor = new Puzzle{newState, puzzle->g + 1, puzzle->h, 0, puzzle};
        successors.push_back(successor);
    }

    // Move the empty tile right if possible
    if (i % 3 != 2) {
        std::vector<int> newState = state; // Create a copy of the state
        swap(newState, i, i + 1);
        Puzzle* successor = new Puzzle{newState, puzzle->g + 1, puzzle->h, 0, puzzle};
        successors.push_back(successor);
    }

    return successors;
}


  struct ComparePuzzle
  {
    bool operator()(const Puzzle* lhs, const Puzzle* rhs) const {
        return lhs->g + lhs->h > rhs->g + rhs->h;
    }
  };


 Puzzle* solvePuzzle(const Puzzle& startState, const Puzzle& goalState) {
    std::set<std::vector<int>> closedSet;
    std::vector<Puzzle*> openQueue;
    std::map<std::vector<int>, Puzzle*> nodeMap;

    Puzzle* start = new Puzzle{ startState.state, 0, manhattanDistance(startState.state), 0};

    openQueue.push_back(start);
    nodeMap[start->state] = start;

    while (!openQueue.empty()) {
        std::sort(openQueue.begin(), openQueue.end(), ComparePuzzle());

        Puzzle* current = openQueue.back();
        openQueue.pop_back();

        if (current->state == goalState.state) {
            return current;
        }

        closedSet.insert(current->state);

        std::vector<Puzzle*> successors = generateSuccessors(current);

        for (Puzzle* successor : successors) {
            if (closedSet.find(successor->state) != closedSet.end()) {
                // Skip this successor if it is already in the closed set
                continue;
            }

            int tentative_g = current->g + 1;

            if (nodeMap.find(successor->state) == nodeMap.end() || tentative_g < nodeMap[successor->state]->g) {
                successor->g = tentative_g;
                successor->h = manhattanDistance(successor->state);
                successor->parent = current;

                if (nodeMap.find(successor->state) != nodeMap.end()) {
                    // Erase the element from the vector using the erase-remove idiom
                    openQueue.erase(std::remove(openQueue.begin(), openQueue.end(), nodeMap[successor->state]), openQueue.end());
                }

                nodeMap[successor->state] = successor;
                openQueue.push_back(successor);
            }
        }
    }

    return nullptr;
}

void printPath(const Puzzle* goalState) {
    std::vector<const Puzzle*> path;

    // Follow the parent pointers from the goal state to the start state
    const Puzzle* current = goalState;
    while (current != nullptr) {
        path.push_back(current);
        current = current->parent;
    }

    // Print the path in reverse order
    for (int i = path.size() - 1; i >= 0; i--)
    {
        const Puzzle* state = path[i];
        int counter = 1;

        for (int num : state->state) {
            std::cout << num << " ";
            if(counter%3 == 0){std::cout << std::endl;}
            counter++;
        }
        std::cout << std::endl;
    }
}

};
