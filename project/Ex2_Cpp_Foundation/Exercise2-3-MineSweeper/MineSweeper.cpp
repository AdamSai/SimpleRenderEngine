#include "MineSweeper.hpp"
#include <iostream>

MineSweeper::MineSweeper()
{
	// todo implement
	_numberOfBombs = 0;
	gameShouldEnd = false;
}


Field::Field()
{
	this->isBomb = false;
	this->fieldType = Button;
}


void MineSweeper::initLevel(int size)
{
	playField = std::vector<std::vector<Field>>(size, std::vector<Field>(size));
	spawnBombs(size);
}

void MineSweeper::spawnBombs(int size)
{
	int numberOfBombs = size * 2 * 0.2f;
	_numberOfBombs = numberOfBombs;

	while (numberOfBombs > 0)
	{
		// TODO: Make it random on every run
		int randRow = rand() % size;
		int randColumn = rand() % size;
		if (playField[randRow][randColumn].isBomb)
			continue;

		playField[randRow][randColumn].isBomb = true;
		numberOfBombs--;
	}
	_numberOfHidden = (size * size) - _numberOfBombs;

}

void MineSweeper::print()
{
	for (int i = 0; i < playField[0].size(); i++)
	{
		for (int j = 0; j < playField[0].size(); j++)
		{
			FieldType current = getField(i, j);
			std::string message = " ";
			switch (current)
			{
			case BombExploded:
				message += "B";
				break;
			case Empty:
				message += "0";
				break;
			case Button:
				message += "X";
				break;
			case Flag:
				message += "F";
				break;
			default:
				message += std::to_string(current - 1); // -1 because the enumtypes index 0 is the empty undiscovered, and index 1 is the discovered field with 0 bombs nearby
			}

			std::cout << message;
		}
		std::cout << std::endl;

	}
}

FieldType MineSweeper::getField(int row, int column)
{
	return playField[row][column].fieldType;
}

bool MineSweeper::isRunning()
{
	if (numberOfHidden() == 0)
	{
		std::cout << std::endl << "You won! Congrats!!";
		return false;
	}
	return !gameShouldEnd;
}

// Look at sorrounding neighbours and if they are an empty field with no nearby bombs, and reveal them. Continue recursively for every neighbour
void MineSweeper::floodFill(int row, int column)
{
	int mapSize = playField[0].size();
	for (int x = row - 1; x <= row + 1; x++)
	{
		for (int y = column - 1; y <= column + 1; y++)
		{
			// Skip out of bounds and the position the player clicked
			if ((x == row && y == column) ||
				x < 0 ||
				x >= mapSize ||
				y < 0 ||
				y >= mapSize
				)
				continue;
			if (playField[x][y].fieldType == Button && !playField[x][y].isBomb && bombsNearby(x, y) == 0)
			{
				take(x, y);
				floodFill(x, y);
			}

		}
	}
}

std::string MineSweeper::take(int row, int column)
{
	if (row >= playField[0].size() || column >= playField[0].size())
		return "Invalid input.";
	Field current = playField[row][column];

	if(current.fieldType == Flag)
		return "Can't select a flagged element";

	if (current.isBomb)
	{
		playField[row][column].fieldType = BombExploded;
		gameShouldEnd = true;
		return "Ouch you hit a bomb!";
	}
	else
	{
		int neighbouringBombs = bombsNearby(row, column);
		FieldType fType = (FieldType)(neighbouringBombs + 1); // +1 because the enumtypes index 0 is the empty undiscovered, and index 1 is the discovered field with 0 bombs nearby 
		playField[row][column].fieldType = fType;

		if (bombsNearby(row, column) == 0)
			floodFill(row, column);
		_numberOfHidden--;
		return std::to_string(neighbouringBombs) + " bombs nearby! ";
	}
	playField[row][column].fieldType = Empty;
	return "Hit an empty field";
}

int MineSweeper::numberOfBombs()
{
	return _numberOfBombs;
}

int MineSweeper::numberOfHidden()
{
	std::cout << std::endl << std::to_string(_numberOfHidden);
	return _numberOfHidden;
}

int MineSweeper::bombsNearby(int row, int column)
{
	int count = 0;
	int mapSize = playField[0].size();
	for (int x = row - 1; x <= row + 1; x++)
	{
		for (int y = column - 1; y <= column + 1; y++)
		{
			// Skip out of bounds and the position the player clicked
			if ((x == row && y == column) ||
				x < 0 ||
				x >= mapSize ||
				y < 0 ||
				y >= mapSize
				)
				continue;
			if (playField[x][y].isBomb)
				count++;

		}
	}
	return count;
}

void MineSweeper::flagField(int row, int column)
{
	if (playField[row][column].fieldType == Button)
	{
		playField[row][column].fieldType = Flag;
	}
	else if (playField[row][column].fieldType == Flag)
	{
		playField[row][column].fieldType = Button;
	}

}


