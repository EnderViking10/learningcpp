#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h>

std::string input(const std::string& message) {
    // Gets the users input
    std::string result;
    std::cout << message << std::endl;
    std::cin >> result;
    return result;
}

bool compare(std::string inA, std::string inB) {

    // Makes inA and inB lowercase
    std::transform(inA.begin(), inA.end(), inA.begin(), [](unsigned char c){return std::tolower(c);});
    std::transform(inB.begin(), inB.end(), inB.begin(), [](unsigned char c){return std::tolower(c);});

    // Does the win loss comparisons
    if(inA == inB) {
        std::cout << "You tied with the computer!" << std::endl;
    } else if((inA == "paper" && inB == "rock") // A wins
           || (inA == "rock" && inB == "scissors")
           || (inA == "scissors" && inB == "paper")) {
        std::cout << "You win!" << std::endl;
        return true;
    } else if((inB == "paper" && inA == "rock") // B wins
           || (inB == "rock" && inA == "scissors")
           || (inB == "scissors" && inA == "paper")) {
        std::cout << "You lose!" << std::endl;
        return false;
    } else {
        std::cout << "You did not enter an expected input." << std::endl;
        return false;
    }
    return false; // Clion told me this couldn't be in the else statement???
}

std::string randomize() {
    // Gets the random computer input
    std::string choices[] = {"rock", "paper", "scissors"};
    std::string choice = choices[rand() % 3];

    return choice;
}

int main() {
    int wins = 0;

    while(true) {
        // Gets the users input
        std::string userMoveMessage = "Options are rock, paper, scissors\nEnter your move:";
        std::string userMove = input(userMoveMessage);

        // Gets the computers input
        std::string computerMove = randomize();

        if (compare(userMove, computerMove)) {
            wins++;
        }

        // Ask for another round
        std::cout << "Would you like to play again?" << std::endl;
        std::string playAgain;
        std::cin >> playAgain;
        std::transform(playAgain.begin(), playAgain.end(), playAgain.begin(), [](unsigned char c){return std::tolower(c);});
        if(playAgain != "y" && playAgain != "yes") {
            std::cout << "You won " << wins << " times." << std::endl;
            exit(0);
        }
    }
}
