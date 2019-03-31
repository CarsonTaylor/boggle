# BOGGLE
This project allows the user to play boggle on any size boggle board.  For my video walkthrough see [here](https://alabama.box.com/s/77s1y07a8tax2xayzssgafqyfgmdmxdk)

## Getting Started
If you are on a unix system, run the loadDict.sh bash script with the command

```
$ bash loadDict.sh
```
to build the dictionary file.  If for some reason this doesn't work you can download the dictionary file from [here](https://alabama.box.com/s/6vm2svlcuoi757uon3972mbxv5ptuejr).
The dictionary file is named "dict.txt" and contains only words consisting of lowercase characters.

Once the dictionary file is loaded, compile the project with the command

```
$ make
```
then run
```
$ ./boggle
```
to start a game of boggle.

## Gameplay

You can input any integer for the size of the board, but be warned that values greater than 100 will take longer than 10 seconds to run and may not look great on the screen. Resizing the terminal window can sometimes help with this. A board of 200x200 will take approximately 10 minutes to run.  Inputting an invalid value here will cause the program to terminate. Entering an integer followed by other non-integer characters will take the integer portion and run the program.

From there, you can input any string to check on the board but only strings containing lower case letters exclusively will be checked for validity.  Input strings longer than 50 will also not be checked as the longest word in dict.txt is 22 characters.  After inputting a word, the program informs the player how much time is remaining for the current game.

When you are done finding words, or want to check the computer's output, input DONE to see the words the computer found, along with the program runtime. If the player tries to input a word once 3 minutes has elapsed, the program tells them time is up and continues.

Once the game is complete, the program will print the player's score along with the computer's score.  You can then either choose to play another game or exit the program.  Inputting invalid values here will prompt for input again.

## Thanks!
