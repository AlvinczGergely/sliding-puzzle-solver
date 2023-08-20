#include <iostream>

#include "puzzle.cpp"


int main() {
    puzzle test;

    do {
        test.vec_shuffl(test.numbers);
        if (test.puzzle_solvabel(test.numbers) == false) {
            std::cout << "not solvable" << std::endl;
        } else {
            std::cout << "solvable" << std::endl;
        }
    } while (test.puzzle_solvabel(test.numbers) == false);

    for (int i = 0; i < test.numbers.size(); i++) {
        std::cout << test.numbers[i] << ", ";
    }
    std::cout << std::endl;

    test.fill(test.numbers, test.vec);
    test.print(test.vec);

    std::cout << std::endl;

    std::cout << "The Manhattan distance: " << test.manhattanDistance(test.numbers) << std::endl << std::endl;

    // Solve the puzzle
    puzzle::Puzzle startState{test.numbers, 0, test.manhattanDistance(test.numbers), 0};
    puzzle::Puzzle goalState{test.goal_numbers, 0, 0, 0};
    puzzle::Puzzle* solution = test.solvePuzzle(startState, goalState);

    if (solution != nullptr) {
        // Print the steps of the solution
        test.printPath(solution);
    } else {
        std::cout << "No solution found." << std::endl;
    }

    return 0;
}
