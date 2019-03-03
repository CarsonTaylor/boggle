/*
Carson Taylor
This program allows the user
to play the game boggle
*/

/////////TODO////////////
//add classic or new boggle variations
//Handle Qu piece

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "trie.h"


//build a trie from dictionary


char** dice;
int size;
char** board;
void initDice();
void buildBoard();

int main(int argc, char *argv[]) {
  //initializes board with size of 4x4, add code
  //for arbitrary board size later.
  initDice();
  //builds random board
  buildBoard();

  printf("\nHere is your boggle board.\n");
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      printf("%c ", board[i][j]);
    }
    printf("\n");
  }

  //buildTrie
  FILE* dict = fopen("dict.txt", "r");
  struct Node *root = getNode();
  for (int i = 0; i < 369646; i++){
      char* word = malloc(sizeof(char) * 30);
      fscanf(dict, "%s", word);
      insert(root, word);
  }
  fclose(dict);

  //test and search trie
  char* input = malloc(sizeof(char) * 30);
  printf("Enter a word to find in the dictionary: ");
  scanf("%s", input);
  while(strcmp(input, "zzz") != 0){
    bool result = search(root,input);
    printf("%d\n", result);
    scanf("%s",input);
  }
}

void initDice(){
  //6 is number of sides on a dice

  size = 16;
  dice = malloc(sizeof(char*) * size);
  for(int i = 0; i < size; i++)
    dice[i] = malloc(sizeof(char) * 6);

  //assigns dice values for classic boggle
  strcpy(dice[0], "AACIOT");
  strcpy(dice[1], "ABILTY");
  strcpy(dice[2], "ABJMOQ");
  strcpy(dice[3], "ACDEMP");
  strcpy(dice[4], "ACELRS");
  strcpy(dice[5], "ADENVZ");
  strcpy(dice[6], "AHMORS");
  strcpy(dice[7], "BIFORX");
  strcpy(dice[8], "DENOSW");
  strcpy(dice[9], "DKNOTU");
  strcpy(dice[10], "EEFHIY");
  strcpy(dice[11], "EGKLUY");
  strcpy(dice[12], "EGINTV");
  strcpy(dice[13], "EHINPS");
  strcpy(dice[14], "ELPSTU");
  strcpy(dice[15], "GILRUW");
}

void buildBoard(){
  //builds randomly generated boards
  srand(time(0));
  //randDice is an array holding random order
  //of numbers 0-size
  int randDice[size];
  for(int i = 0; i < size; i++){
    randDice[i] = i;
  }
  //fisher yates shuffle
  for(int i = size - 1; i > 0; i--){
    int j = rand() % (i+1);
    int temp = randDice[j];
    randDice[j] = randDice[i];
    randDice[i] = temp;
  }

  board = malloc(sizeof(char*) * 4);
  for(int i = 0; i < 4; i++)
    board[i] = malloc(sizeof(char) * 4);

  int count = 0;
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      int diceFace = rand() % 6;
      board[i][j] = dice[randDice[count]][diceFace];
      count++;
    }
  }
}
