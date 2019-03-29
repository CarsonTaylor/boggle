# BOGGLE
This project allows the user to play boggle on any size boggle board.

## Getting Started
If you are on a unix system, run the loadDict.sh bash script with the command

```
$ bash loadDict.sh
```
to build the dictionary file.  If for some reason this doesn't work you can download the dictionary file from [here](https://alabama.box.com/s/6vm2svlcuoi757uon3972mbxv5ptuejr).
The dictionary file is named "dict.txt" and contains only words consisting of lowercase characters.

Once the dictionary file is loaded, compile the project with the command

```
$ gcc -std=c11 boggle.c -o boggle
```
then run
```
$ ./boggle
```
to start the game.

## Gameplay

You can input any integer for the size of the board, but be warned that values greater than 100 will take longer than 10 seconds to run.  A board of 200x200 will take approximately 10 minutes to run.

From there, you can input any string to check on the board but only strings containing lower case letters exclusively will be checked for validity.

When you are done finding words, or want to check the computer's output, input DONE to see the words the computer found, along with some runtime statistics.

Once the game is complete, the program will print your score along with the computers score (which is the maximum.)

## Thanks!
