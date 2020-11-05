// Minimax Tic Tac Toe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <string>

int rowLength = 3;

struct Move {
    int index;
    int score;
    int position[2];
};
struct EmptySpots {
    int arr[25][2];
};

struct Field
{
    int arr[5][5];

    Field() {
        for (int i = 0; i < rowLength; i++) {
            for (int o = 0; o < rowLength; o++) {
                arr[i][o] = 0;
            }
        }
    }

    Field(int field[5][5]) {
        for (int i = 0; i < rowLength; i++) {
            for (int o = 0; o < rowLength; o++) {
                arr[i][o] = field[i][o];
            }
        }
    }
};

std::map<std::string, Move> storage;

std::string repeat(char charr, int count) {
    std::string result = "";

    for (int i = 0; i < count; i++) {
        result += charr;
    }

    return result;
}

int checkWinner (Field points) {
    std::string player1WinCondition = repeat(*"1", rowLength);
    std::string player2WinCondition = repeat(*"2", rowLength);
    std::string strings[12];
    int length = 2;
    int winner = 0;

    for (int o = 0; o < rowLength; o += 1) {
        std::string string1;
        std::string string2;

        strings[0] += std::to_string(points.arr[o][o]);
        strings[1] += std::to_string(points.arr[o][(rowLength - 1) - o]);

        for (int i = 0; i < rowLength; i += 1) {
            strings[length] += std::to_string(points.arr[o][i]);
            strings[length + 1] += std::to_string(points.arr[i][o]);
        }

        length += 2;
    }


    for (int rowIndex = 0; rowIndex < 12; rowIndex++) {
        if (strings[rowIndex] == player1WinCondition) {
            winner = 1;
        }

        if (strings[rowIndex] == player2WinCondition) {
            winner = 2;
        }
    }

    return winner;
};

std::map<int, int[2]> getEmptySpots (Field fields) {
    std::map<int, int[2]> emptySpots;
    int emptySpotCount = 0;

    for (int rowIndex = 0; rowIndex < rowLength; rowIndex++) {
        for (int fieldIndex = 0; fieldIndex < rowLength; fieldIndex++) {
            if (fields.arr[rowIndex][fieldIndex] == '0' || fields.arr[rowIndex][fieldIndex] == NULL) {
                emptySpots[emptySpotCount][0] = rowIndex;
                emptySpots[emptySpotCount][1] = fieldIndex;
                emptySpotCount++;
            }
        }
    }

    return emptySpots;
};

 std::string fieldToText (Field field)  {
    std::string result = "";

    for (int rowIndex = 0; rowIndex < rowLength; rowIndex++) {
        for (int fieldIndex = 0; fieldIndex < rowLength; fieldIndex++) {
            switch (field.arr[rowIndex][fieldIndex]) {
            case 1:
                result += 'x';
            case 2:
                result += 'o';
            case 0:
                result += ' ';
            }
        }
    }

    return result;
};

Move minimax(Field fields, bool isPlayer1 = false, int depth = 0) {
    int currentPlayer = isPlayer1 ? 1 : 2;
    std::map<int, int[2]> emptySpots = getEmptySpots(fields);
    Move moves[25];
    int moveCount = 0;

    int winner = checkWinner(fields);

    if (storage.find(fieldToText(fields)) != storage.end()) {
        return storage[fieldToText(fields)];
    }
    
    if (winner == 1) {
        Move move;

        move.score = -10;
        return move;
    }

    if (winner == 2) {
        Move move;

        move.score = 10;
        return move;
    }

    if (emptySpots.size() == 0) {
        Move move;

        move.score = 0;
        return move;
    }

    for (int i = 0; i < emptySpots.size(); i += 1) {
        Field newField = *new Field(fields.arr);
        Move move;

        move.position[0] = emptySpots[i][0];
        move.position[1] = emptySpots[i][1];

        newField.arr[move.position[0]][move.position[1]] = currentPlayer;

        Move newMove = minimax(newField, currentPlayer == 2, depth + 1);
        move.score = newMove.score;

        moves[moveCount] = move;
        moveCount++;
    }

    int bestMove;

    if (currentPlayer == 2) {
        int bestScore = -10000;

        for (int i = 0; i < moveCount; i += 1) {
            if (moves[i].score > bestScore) {
                bestScore = moves[i].score;
                bestMove = i;
            }
        }
    }
    else {
        int bestScore = 10000;

        for (int i = 0; i < moveCount; i += 1) {
            if (moves[i].score < bestScore) {
                bestScore = moves[i].score;
                bestMove = i;
            }
        }
    }

    if (depth == 0) {
        std::cout << "" << std::endl;
    }

    storage[fieldToText(fields)] = moves[bestMove];
    return moves[bestMove];
};

void drawFields(Field field, bool draw) {
    int emptyCount = 1;

    for (int i = 0; i < rowLength; i++)
    {
        std::cout << "|";
        
        for (int o = 1; o < rowLength * 2; o++)
        {
            std::cout << "-";
        }

        std::cout << "|" << std::endl << "|";

        for (int o = 0; o < rowLength; o += 1)
        {
            char afterChar = (o == rowLength - 1) ? *"" : *"|";

            switch (field.arr[i][o])
            {
            case 0:
                
                std::cout << (draw ? std::to_string(emptyCount++) : " ") << afterChar;
                break;
            case 1:
                std::cout << "o" << afterChar;
                break;
            case 2:
                std::cout << "x" << afterChar;
                break;
            default:
                break;
            }
        }

        std::cout << "|" << std::endl;
    }

    std::cout << "|";

    for (int o = 1; o < rowLength * 2; o++)
    {
        std::cout << "-";
    }

    std::cout << "|" << std::endl;
}

int main()
{
    std::string userinput;

    std::cout << "Field Size (max 5)" << std::endl;
    std::cin >> userinput;

    rowLength = std::stoi(userinput);
    bool gameRunning = true;

    Field field = *(new Field());


    while (gameRunning) {
        int winner = 0;

        std::string userinput;
        drawFields(field, true);
        std::cout << "Make your move" << std::endl;
        std::cin >> userinput;

        int choice = std::stoi(userinput) - 1;

        std::map<int, int[2]> emptySpots = getEmptySpots(field);

        field.arr[emptySpots[choice][0]][emptySpots[choice][1]] = 1;

        winner = checkWinner(field);

        if (winner) {
            std::cout << "player " << winner << "won";
            gameRunning = false;
        }

        if (gameRunning) {
            Move move = minimax(field, false, 0);

            if (move.position[0] < 0) {
                std::cout << "draw" << std::endl;
                gameRunning = false;
            }
            if (gameRunning) {
                field.arr[move.position[0]][move.position[1]] = 2;
                winner = checkWinner(field);
            }

            if (winner) {
                std::cout << "player " << winner << " won" << std::endl;
                gameRunning = false;
            }
        }
    }

    drawFields(field, false);
}
