# Philosophers
A simulation of the [Dining philosphers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)

## Description
A C program that uses threads to represent philosphers and mutexes to represent forks. The problem can be simulated with user-supplied timings. 

## Installation
Clone this repository and run

    make

## Usage 

Takes the following arguments on he command line:

* Number of philosophers to simulate (this is also the number of forks)
* Time (in ms) after which philosphers die without food
* Time (in ms) that philosophers take to eat after they have acquired two forks
* Time (in ms) that philosophers will take to sleep after having eaten
* (Optional) Number of meals that philosophers need to eat before the simulation ends. 

<!-- bullet, be-gone -->
Like this: 

    `./philo NUMBER_OF_PHILOSOPHERS TIME_TO_DIE TIME_TO_EAT TIME_TO_SLEEP [NUMBER_OF_MEALS]`

### Examples

    ./philo 5 800 200 200
    ./philo 100 900 300 200 7




