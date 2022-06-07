# PACMAN

Pac-Man is a Japanese video game franchise created by Toru
Iwatani, the father of Pac-Man, but published, developed and
owned by Bandai Namco Entertainment (formerly Namco).

* Single Player.
* Food dots inside the board.
* Enemy ghosts move randomly.
* Pacman can eat enemies with a special food dots and .
* Pacman has three lives and loses when an enemy touches it.
* Pacman wins the game when it has taken all food dots in the map.

## Architecture and Technical details

* Programming language: C
* The game's maze layout can be static.
* Pacman gamer must be controlled by the user.
* Enemies are autonomous entities that will move a random way.
* Enemies and pacman should respect the layout limits and walls.
* Each enemy's behaviour will be implemented as a separated thread.
* Display obtained pacman's scores.

### Game Structure

Files:
* pacman.c
* Makefile

#### Operating Diagram

![Ejemplo de diagrama de flujo de algoritmo](https://user-images.githubusercontent.com/78834172/172476721-746a7fd0-676c-453c-b819-67423184c75a.png)

##### Requirements

* The game's maze layout can be static.
* The pacman gamer must be controlled by the user.
* Enemies are autonomous entities that will move a random way.
* Enemies and pacman should respect the layout limits and walls.
* Enemies number can be configured on game's start.
* Each enemy's behaviour will be implemented as a separated thread.
* Enemies and pacman threads must use the same map or game layout data structure resource.
* Display obtained pacman's scores.
* Pacman loses when an enemy touches it.
* Pacman wins the game when it has taken all coins in the map.
