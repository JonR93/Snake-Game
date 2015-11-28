# Snake-Game
C implementation of the game, Snake
Write a C program snake that implements the classic snake game.
The game must utilize all available space of the current terminal window.
There must be a visible border delineating the area where the snake can move.
The initial length of snake is one character.
Initial direction of the snake's movement is chosen randomly.
The user can press one of the four arrow keys to change the direction of the snake's movement.
Trophies are represented by a digit randomly chosen from 1 to 9.
There's always exactly one trophy at any given moment.
When the snake eats the trophy, its length is increased by the corresponding number of characters.
A trophy expires after a random interval from 1 to 9 seconds.
A new trophy is shown at a random location on the screen after the previous one has either expired or is eaten by the snake.
The snake's speed is increased in a linear proportion with its length.
The snake dies and the game ends if:
It runs into the border; or
It runs into itself; or
The user attempts to reverse the snake's direction.
The user wins the game if the snake's length grows to three times the perimeter of the border.
