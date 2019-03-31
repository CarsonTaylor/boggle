/*
Carson Taylor
This program allows the user
to play the game boggle using a trie for
word lookup and a modified DFS for
finding words
*/

/////////TODO////////////
//add classic or new boggle variations
//Handle Qu piece

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <setjmp.h>

#include "trie.h"
#include "adjacency.h"

//holds array of dice with proper letters
char** dice;

//variables for board
int size;
int dimension;
char* board;

//output file
FILE* foundWords;

//variables for search
int* used;
int* checked;
int* neighbours;
int* prevStack;
int top = -1;
int startLetter;
char word[50] = {'\0'};

//root of lookup trie
struct Node *root;
//root of trie for found words
struct Node *foundRoot;
//root of trie for user input words
struct Node *inputRoot;

//total score
unsigned long totalUserScore;
unsigned long totalComputerScore;

void initDice();
void buildBoard();
void findWords(int start, int* used, int* checked, int** graph, jmp_buf solved);
void append(char* s, char c);
void depend(char* s);
int isLower(char* s);
void welcome();
void displayBoard();
int score(char* word);

int main(int argc, char *argv[]) {
  //welcome sequence
  welcome();

  char* playAgain = malloc(sizeof(char) * 5);
  while(strncmp(playAgain,"no",2)!=0){
    //gets and sets dimension and size of board
    dimension = 0;
    printf("Enter the dimesion of your board, n -- (board will be n x n squares)\n\n");
    printf("Board sizes larger than 100 may not fit well on screen\n\n-> ");
    scanf("%d",&dimension);
    size = dimension*dimension;

    if(dimension == 0){
      printf("Invalid board size. Exiting program\n\n");
      exit(1);
    }

    //initializes dice with appropiate letters for
    //proper boggle letter distribution
    initDice();
    //build and display random board of specified size
    buildBoard();
    displayBoard();

    //build trie from dictionary file
    FILE* dict = fopen("dict.txt", "r");
    root = getNode();
    char *dictWord;
    dictWord = malloc(sizeof(char)*128);
    while(fgets(dictWord,128,dict)){
      dictWord[strlen(dictWord)-1] = '\0';
      if(strlen(dictWord) > 2 && isLower(dictWord)){
        insert(root, dictWord);
      }
    }
    fclose(dict);

    //initialize variables for runtime calculation
    clock_t start,end;
    double cpu_time_used;
    start = clock();

    //declare and initialize adjacency list for graph of board
    int** adList;
    adList = adjacencyList(dimension);

    //initialize neighbours array with proper values
    //each index holds the numbers of valid neighbours
    //at board[i]
    neighbours = malloc(sizeof(int) * size);
    for(int i = 0; i < size; i++)
      neighbours[i] =0;
    for(int i = 0; i < size; i++){
      for(int j = 0; j < 9; j++){
        if(adList[i][j] != -1)
          neighbours[i]++;
      }
    }

    //allocate space for search arrays
    //used is part of the word
    //checked is an index that does not create a valid prefix
    //prevStack holds the indexes of letters already used in the current word
    used = malloc(sizeof(int) * size);
    checked = malloc(sizeof(int) * size);
    prevStack = malloc(sizeof(int) * size);

    //initialize root of trie for found words
    foundRoot = getNode();

    //for each letter on the board, find all possible words starting
    //with that letter.  Reset search arrays and temporary
    //word variable before each search
    for(int i = 0; i < size; i++){
      memset(used, 0, size * sizeof(used[0]));
      memset(checked, 0, size * sizeof(checked[0]));
      memset(prevStack, -1, size * sizeof(prevStack[0]));
      //printf("after other mallocs\n");
      word[0] = '\0';
      jmp_buf solved;
      startLetter = i;
      if(!setjmp(solved)) findWords(i,used,checked,adList,solved);
    }

    //stop runtime calculations
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    //check words inputted by user
    inputRoot = getNode();
    char* input = malloc(sizeof(char) * 50);
    printf("Enter found words in lowercase letters (enter DONE to stop): ");
    scanf("%50s", input);
    while(strncmp(input, "DONE", 4) != 0){
      if(isLower(input)){
        int result = search(foundRoot,input);
        if(result){
          int duplicate = search(inputRoot,input);
          if(!duplicate){
            insert(inputRoot,input);
            printf("\nValid: Score - %d\n\n",score(input));
            totalUserScore += score(input);
          }
          else
            printf("\nWord already found\n\n");
        }
        else
          printf("\nInvalid word\n\n");
        scanf("%50s",input);
      }
      else{
        printf("\nInvalid input, please input words with only lowercase letters\n\n");
        scanf("%50s",input);
      }
    }

    //display all found words
    unsigned long wordCount = 0;
    int level = 0;
    char str[128];
    int tab = 0;
    printf("\n---------------Words found by computer----------------\n\n");
    display(foundRoot,str,level,&tab,&wordCount);
    printf("\n\n");

    //display endgame stats
    printf("Computer found all %lu words in a %d x %d board in %f seconds\n",wordCount,dimension,dimension,cpu_time_used);
    printf("for a total score of %lu\n\n", totalComputerScore);
    printf("Your total score is %lu\n\n",totalUserScore);

    printf("Would you like to play again? (enter \"yes\" or \"no\"): ");
    scanf("%5s", playAgain);
    while(strncmp(playAgain,"yes",3) != 0 && strncmp(playAgain,"no",2) != 0){
      printf("Invalid input\n\nWould you like to play again? (enter \"yes\" or \"no\"): ");
      scanf("%5s", playAgain);
    }
  }

  return(0);
}

//recursive seach for words starting at index start
void findWords(int start, int* used, int* checked, int** graph, jmp_buf solved){
  //checks if first call
  if(!used[start]){
    append(word,board[start]);
    used[start] = 1;
  }

  //checks to see if neighbours visited, if so, runs dead end procedure
  //prints checked and used arrays for error checking
  int neighboursVisited = 0;
  for(int i = 0; i < 9; i++){
    if(graph[start][i] != -1 && (checked[graph[start][i]] || used[graph[start][i]]))
      neighboursVisited++;
  }
  if(neighboursVisited == neighbours[start]){
    //BASE CASE - if back at beginning search index, hard return
    if(start == startLetter)
      longjmp(solved, 1);

    //dead-end procedure
    //reset checked boxes
    for(int i = start+1; i < start+dimension+3; i++){
      if(i < size)
      checked[i] = 0;
    }
    //mark current index as checked and unmark as used
    checked[start] = 1;
    used[start] = 0;
    //remove letter at current index from word
    depend(word);

    //mark adjacent indices as checked
    for(int i = 0; i < 9; i++)
      if(graph[prevStack[top]][i] != -1 && graph[prevStack[top]][i] < start &&
         !used[graph[prevStack[top]][i]])
        checked[graph[prevStack[top]][i]] = 1;

    //set new start as the previous letter in current word
    start = prevStack[top];
    for(int i = top; i < size; i++)
      prevStack[i] = -1;
    top--;

    //reset any remaining checked boxes
    for(int i = start - (dim*2) - 2; i < start + (dim*2) + 2; i++){
      if(i < size){
        for(int j = 0; j < 9; j++){
          if(graph[start][j] == i)
            break;
          if(graph[start][j] == -1){
            checked[i] = 0;
            break;
          }
        }
      }
    }

    findWords(start,used,checked,graph,solved);
  }

  //modified dfs with used and checked instead of visited
  //used is part of the word, checked is a box that does not
  //create a valid prefix
  for(int i = 0; i < 9; i++){
    if(graph[start][i] != -1 && (!used[graph[start][i]] && !checked[graph[start][i]])){
      append(word,board[graph[start][i]]);
      //if current word is valid substring
      if(searchSubstring(root,word)){
        //if current word is valid word
        if(search(root,word)){
          //if current word has not alread been found
          //add to found words and update score
          if(!search(foundRoot,word)){
            insert(foundRoot,word);
            totalComputerScore += score(word);
          }
        }

        //mark used and set new end of word
        used[graph[start][i]] = 1;
        top++;
        prevStack[top] = start;

        //reset adjacent checked squares
        for(int j = 0; j < 9; j++){
          checked[graph[start][j]] = 0;
        }

        findWords(graph[start][i],used,checked,graph,solved);
      }
      else{
        //remove last char from word and mark index as checked
        depend(word);
        checked[graph[start][i]] = 1;
        findWords(start,used,checked,graph,solved);
      }
    }
  }
}

//initializes dice with appropiate letters for
//proper boggle letter distribution
void initDice(){
  //6 is number of sides on a dice
  dice = malloc(sizeof(char*) * 16);
  for(int i = 0; i < 16; i++)
    dice[i] = malloc(sizeof(char) * 6);

  //assigns dice values for classic boggle
  strncpy(dice[0], "aaciot",7);
  strncpy(dice[1], "abilty",7);
  strncpy(dice[2], "abjmoq",7);
  strncpy(dice[3], "acdemp",7);
  strncpy(dice[4], "acelrs",7);
  strncpy(dice[5], "adenvz",7);
  strncpy(dice[6], "ahmors",7);
  strncpy(dice[7], "biforx",7);
  strncpy(dice[8], "denosw",7);
  strncpy(dice[9], "dknotu",7);
  strncpy(dice[10], "eefhiy",7);
  strncpy(dice[11], "egkluy",7);
  strncpy(dice[12], "egintv",7);
  strncpy(dice[13], "ehinps",7);
  strncpy(dice[14], "elpstu",7);
  strncpy(dice[15], "gilruw",7);
}

//builds randomly generated boards
void buildBoard(){
  //seee random
  srand(time(0));

  //allocate memory for board
  board = malloc(sizeof(char) * size);

  //randDice is an array holding random order
  //of numbers 0-size
  int randDice[16];

  //generate board
  int count = 0;
  while(count < size){
    //if all dice used, shuffle again
    if(count%16==0){

      for(int i = 0; i < 16; i++){
        randDice[i] = i;
      }
      //fisher yates shuffle
      for(int i = 15; i > 0; i--){
        int j = rand() % (i+1);
        int temp = randDice[j];
        randDice[j] = randDice[i];
        randDice[i] = temp;
      }
    }
    //assign random dice values to board
    int diceFace = rand() % 6;
    board[count] = dice[randDice[count%16]][diceFace];
    count++;
  }
}

//appends letter c to end of string s
void append(char* s, char c) {
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}

//removes last letter of string s
void depend(char* s) {
    int len = strlen(s);
    s[len-1] = '\0';
}

//returns 1 if all chars in s are lowercase letters
int isLower(char* s){
  int len = strlen(s);
  for(int i = 0; i < len; i++){
    if(s[i] < 97 || s[i] > 122)
      return 0;
  }
  return 1;
}

//welcome sequence printing
void welcome(){
  char ESC=27;
  printf("%c[1m",ESC);  /*- turn on bold */
  printf("%c[5m",ESC);  /*- turn on bold */
  printf("%c[92m",ESC);  /*- turn on bold */
  printf("\n\t\tWELCOME TO BOGGLE\n\n");
  printf("%c[0m",ESC); /* turn off bold */
}

//display board
void displayBoard(){
  printf("\nHere is your boggle board.\n\n\t");
  for(int i = 0; i < size; i++){
      printf("%c ", board[i]-32);
      if((i+1) % dimension == 0)
        printf("\n\t");
  }
  printf("\n");
}

//calculates score for a given word
int score(char* word){
  int length = strlen(word);
  if(length <= 4){
    return 1;
  }
  else if(length == 5){
    return 2;
  }
  else if(length == 6){
    return 3;
  }
  else if(length == 7){
    return 5;
  }
  else if(length >= 8){
    return 11;
  }
  return 0;
}
