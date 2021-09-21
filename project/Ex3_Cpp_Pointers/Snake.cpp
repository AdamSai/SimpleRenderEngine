#include "Snake.hpp"
#include "SnakeBody.hpp"

Snake::Snake()
{
}

// This is called every time the snake should move.
void Snake::move()
{

	SnakeBody* current = this->snakeHead;
	int previousX = current->x;
	int previousY = current->y;

	current->x += moveX;
	current->y += moveY;
	current->moveTo(current->x, current->y);
	current = current->next;

	while (current != nullptr)
	{
		int tempX = current->x;
		int tempY = current->y;

		current->x = previousX;
		current->y = previousY;

		previousX = tempX;
		previousY = tempY;
		current->moveTo(current->x, current->y);

		current = current->next;
	}



}

// This is called whenever one arrow key is pressed.
// Values are [-1,0,1] for x and y and describe how much the snake moves in each update
void Snake::setMoveDirection(int moveX, int moveY)
{
	// TODO prevent move direction to be the opposite of the current direction
	if (this->moveX != moveX && this->moveY != moveY)
	{
		this->moveX = moveX;
		this->moveY = moveY;
	}

}

// Initialize game. This is called every time the game starts or restarts.
void Snake::init(int initialLength, int x, int y, int moveX, int moveY)
{
	length = initialLength;
	this->moveX = moveX;
	this->moveY = moveY;

	if (snakeHead != nullptr)
	{
		delete this->snakeHead;
	}
	// TODO clean up and initialize the new snake. Make sure not to leak memory.
	snakeHead = new SnakeBody(x, y);
	SnakeBody* current = snakeHead;

	for (int i = 0; i < initialLength; i++)
	{
		if (i != initialLength - 1)
		{
			current->next = new SnakeBody(x - i, y);
			current = current->next;
		}
		else
			current->next = nullptr;
	}
}

// Should increment the length and append a SnakeBody
void Snake::grow()
{
	// TODO add a SnakeBody to the snake.
	SnakeBody* current = this->snakeHead;
	while (true)
	{

		if (current->next == nullptr)
		{
			current->next = new SnakeBody(current->x, current->y);
			break;
		}
		current = current->next;

	}

}

// Returns true if the snake collides with itself
bool Snake::collide(int x, int y)
{
	// TODO check if there is any snake segment at coord (x,y) and return true if that is the case
	SnakeBody* current = this->snakeHead;
	while (current->next != nullptr)
	{
		if (current->x == x && current->y == y)
			return true;

		current = current->next;

	}
	return false;
}
