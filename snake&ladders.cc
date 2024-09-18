#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <random>

// Jump class
class Jump {
private:
    int start;
    int end;

public:
    Jump(int s = 0, int e = 0) : start(s), end(e) {}

    int getStart() const { return start; }
    void setStart(int s) { start = s; }

    int getEnd() const { return end; }
    void setEnd(int e) { end = e; }
};

// Cell class
class Cell {
public:
    Jump* jump = nullptr;

    Cell() : jump(nullptr) {}
    ~Cell() { delete jump; }
};

// Board class
class Board {
public:
    std::vector<std::vector<Cell>> cells;

    Board(int boardSize, int numberOfSnakes, int numberOfLadders);

    Cell& getCell(int playerPosition);
};

Board::Board(int boardSize, int numberOfSnakes, int numberOfLadders) {
    cells.resize(boardSize, std::vector<Cell>(boardSize));
    
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(1, boardSize * boardSize - 1);

    // Add snakes
    while (numberOfSnakes > 0) {
        int snakeHead = dist(rng);
        int snakeTail = dist(rng);
        if (snakeTail >= snakeHead) continue;

        Jump* snakeObj = new Jump(snakeHead, snakeTail);
        Cell& cell = getCell(snakeHead);
        cell.jump = snakeObj;

        numberOfSnakes--;
    }

    // Add ladders
    while (numberOfLadders > 0) {
        int ladderStart = dist(rng);
        int ladderEnd = dist(rng);
        if (ladderStart >= ladderEnd) continue;

        Jump* ladderObj = new Jump(ladderStart, ladderEnd);
        Cell& cell = getCell(ladderStart);
        cell.jump = ladderObj;

        numberOfLadders--;
    }
}

Cell& Board::getCell(int playerPosition) {
    int boardRow = playerPosition / cells.size();
    int boardColumn = playerPosition % cells.size();
    return cells[boardRow][boardColumn];
}

// Dice class
class Dice {
private:
    int diceCount;
    const int min = 1;
    const int max = 6;
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    Dice(int count);

    int rollDice();
};

Dice::Dice(int count) : diceCount(count), dist(min, max) {
    std::random_device rd;
    rng.seed(rd());
}

int Dice::rollDice() {
    int totalSum = 0;
    for (int i = 0; i < diceCount; ++i) {
        totalSum += dist(rng);
    }
    return totalSum;
}

// Player class
class Player {
private:
    std::string id;
    int currentPosition;

public:
    Player(const std::string& id, int position);

    std::string getId() const;
    void setId(const std::string& newId);

    int getCurrentPosition() const;
    void setCurrentPosition(int newPosition);
};

Player::Player(const std::string& id, int position) : id(id), currentPosition(position) {}

std::string Player::getId() const { return id; }
void Player::setId(const std::string& newId) { id = newId; }

int Player::getCurrentPosition() const { return currentPosition; }
void Player::setCurrentPosition(int newPosition) { currentPosition = newPosition; }

// Game class
class Game {
private:
    Board board;
    Dice dice;
    std::deque<Player> playersList;
    Player* winner;

    void initializeGame();
    void addPlayers();
    Player* findPlayerTurn();
    int jumpCheck(int playerNewPosition);

public:
    Game();

    void startGame();
};

Game::Game() : board(10, 5, 4), dice(1), winner(nullptr) {
    initializeGame();
}

void Game::initializeGame() {
    addPlayers();
}

void Game::addPlayers() {
    playersList.emplace_back("p1", 0);
    playersList.emplace_back("p2", 0);
}

Player* Game::findPlayerTurn() {
    Player* playerTurn = &playersList.front();
    playersList.pop_front();
    playersList.push_back(*playerTurn);
    return playerTurn;
}

int Game::jumpCheck(int playerNewPosition) {
    if (playerNewPosition > board.cells.size() * board.cells.size() - 1) {
        return playerNewPosition;
    }

    Cell& cell = board.getCell(playerNewPosition);
    if (cell.jump != nullptr && cell.jump->getStart() == playerNewPosition) {
        std::string jumpBy = (cell.jump->getStart() < cell.jump->getEnd()) ? "ladder" : "snake";
        std::cout << "Jump done by: " << jumpBy << std::endl;
        return cell.jump->getEnd();
    }
    return playerNewPosition;
}

void Game::startGame() {
    while (winner == nullptr) {
        Player* playerTurn = findPlayerTurn();
        std::cout << "Player turn is: " << playerTurn->getId() << " Current position is: " << playerTurn->getCurrentPosition() << std::endl;

        int diceNumbers = dice.rollDice();

        int playerNewPosition = playerTurn->getCurrentPosition() + diceNumbers;
        playerNewPosition = jumpCheck(playerNewPosition);
        playerTurn->setCurrentPosition(playerNewPosition);

        std::cout << "Player turn is: " << playerTurn->getId() << " New position is: " << playerNewPosition << std::endl;

        if (playerNewPosition >= board.cells.size() * board.cells.size() - 1) {
            winner = playerTurn;
        }
    }

    std::cout << "WINNER IS: " << winner->getId() << std::endl;
}

// Main function
int main() {
    Game game; // Create an instance of Game
    game.startGame(); // Start the game

    return 0; // Indicate successful execution
}
