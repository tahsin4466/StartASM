#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

#include ".Secrets.h"

void displayAsciiArt() {
    std::string art = R"(
      __  __                                                            _
     |  \/  |_ __ ___  _ __ ___  _ __ ___        ___  ___  ___ _ __ ___| |_ ___
     | |\/| | '_ ` _ \| '_ ` _ \| '_ ` _ \       / __|/ _ \/ __| '__/ _ \ __/ __|
     | |  | | | | | | | | | | | | | | | | |_ _ _\__ \  __/ (__| | |  __/ |_\__ \
     |_|  |_|_| |_| |_|_| |_| |_|_| |_| |_(_|_|_)___/\___|\___|_|  \___|\__|___/
    )";
    std::cout << art << std::endl;
}

void displayHiddenMessage() {
    std::cout << "You've found the secret! Here's an absolutely real quote for you:\n";
    std::cout << "\"Don't believe everything you read on the internet.\" - Abraham Lincoln\n";
}

void playGuessingGame() {
    std::srand(std::time(nullptr));
    int secretNumber = std::rand() % 100 + 1;
    int guess;

    std::cout << "Welcome to the guessing game! I'm thinking of a number between 1 and 100.\n";
    std::cout << "Can you guess what it is?\n";

    do {
        std::cout << "Enter your guess: ";
        std::cin >> guess;

        if (guess > secretNumber) {
            std::cout << "Too high! Try again.\n";
        } else if (guess < secretNumber) {
            std::cout << "Too low! Try again.\n";
        } else {
            std::cout << "Congratulations! You guessed the number.\n";
        }
    } while (guess != secretNumber);
}

void displayRandomJoke() {
    std::vector<std::string> jokes = {
            "Why do programmers prefer dark mode? Because light attracts bugs!",
            "How many programmers does it take to change a light bulb? None, that's a hardware problem.",
            "Why do Java developers wear glasses? Because they don't see sharp.",
    };

    std::srand(std::time(nullptr));
    int index = std::rand() % jokes.size();

    std::cout << jokes[index] << std::endl;
}

void activateSecretFeature() {
    std::cout << "Secret feature activated! You now have access to the 'advanced mode'. It just...doesn't do anything yet.\n";
}