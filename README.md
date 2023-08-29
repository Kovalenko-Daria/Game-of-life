# Game-of-life

## About

This is an implementation of the famous cellular automaton game invented by Cambridge mathematician John Conway.

***

## Rules

* To start, it is necessary to enter the relative path to the file with the starting configuration as a console argument.

* Next, the game will independently show a change in the state of the cells, which is performed according to the following rules:

    * _For a space that is populated:_
    * Each cell with one or no neighbors dies, as if by solitude.
    * Each cell with four or more neighbors dies, as if by overpopulation.
    * Each cell with two or three neighbors survives.

    * _For a space that is empty or unpopulated:_
    * Each cell with three neighbors becomes populated.

* Only things user cam do is changing speed and exit.

* To increase the speed, it is necessary to press "d", and to decrease it, it is necessary to press "a".

* To exit it is necessary to press "q".