# carGame
A simple car game running on Arduino To run the game, you first need to build your own circuit first, the circuit diagram is shown below:

![alt text](https://github.com/lsyssso/carGame/blob/master/circuit.png)

# There are four major functionalities:

# 1.Display
All of the in-game elements are displayed on a LCD screen.

# 2.Switch lane
The player is able to shift the car to another lane (two lanes in total) by pressing the button, the player
should see the car is shifted to another lane immediately after that have pressed the button.

# 3.Game speed adjustment
By using a light sensor, the game retrieves light data and determines the speed of the oncoming
vehicle, the brighter the environment is, the faster the oncoming vehicles are. The reason for this is to
encourage the player to play the game under a good light condition, as it can be harmful to our eyes to
play the game in the dark(even though a lot of people use their smartphone in the bed at night). When
the environment is extremely dark, the oncoming vehicles run very slowly that the game is boring and
not challenging at all.

# 4.Score calculation
The score is continuously incremented as long as the game is not over, and the amount of
incrementation depends on the light condition, the brighter it is, the more score being added to the
total score
