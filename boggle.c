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
char* board;
int discovered[16];
int searchTime;
char color[16];
int pi[16];
int found[16];
int visited[16];
char word[16] = {'\0'};

void initDice();
void buildBoard();
void dfsVisit(int graph[16][16],int u);

void append(char* s, char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

int main(int argc, char *argv[]) {
  //initializes board with size of 4x4, add code
  //for arbitrary board size later.
  initDice();
  //builds random board
  buildBoard();

  printf("\nHere is your boggle board.\n");
  for(int i = 0; i < size; i++){
      printf("%c ", board[i]);
      if((i+1) % 4 == 0)
        printf("\n");
  }

  //buildTrie
  FILE* dict = fopen("dict.txt", "r");
  struct Node *root = getNode();
  for (int i = 0; i < 369646; i++){
      char* word = malloc(sizeof(char) * 30);
      fscanf(dict, "%s", word);
      insert(root, word);
      free(word);
  }
  fclose(dict);

  //test and search trie
  char* input = malloc(sizeof(char) * 30);
  printf("Enter a substring to find in the dictionary: ");
  scanf("%s", input);
  while(strcmp(input, "zzz") != 0){
    bool result = searchSubstring(root,input);
    printf("%d\n", result);
    scanf("%s",input);
  }


  //adjacency matrix for graph of board
  int adMat[16][16] = {
      //   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
          {0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0},//0
          {1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0},//1
          {0,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0},//2
          {0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0},//3
          {1,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0},//4
          {1,1,1,0,1,0,1,0,1,1,1,0,0,0,0,0},//5
          {0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,0},//6
          {0,0,1,1,0,0,1,0,0,0,1,1,0,0,0,0},//7
          {0,0,0,0,1,1,0,0,0,1,0,0,1,1,0,0},//8
          {0,0,0,0,1,1,1,0,1,0,1,0,1,1,1,0},//9
          {0,0,0,0,0,1,1,1,0,1,0,1,0,1,1,1},//10
          {0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1},//11
          {0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0},//12
          {0,0,0,0,0,0,0,0,1,1,1,0,1,0,1,0},//13
          {0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1},//14
          {0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0} //15
        };






  //DFS textbook version

  //old dynamic handling
  /*color = malloc(sizeof(char)*size);
  pi = malloc(sizeof(int)*size);
  discovered = malloc(sizeof(int)*size);*/

  //web dfs
  /*for(int i = 0; i < 16; i++)
    visited[i]=0;*/



  for(int i = 0; i < 16; i++){
    color[i] = 'w';
    pi[i] = -1;
  }
  searchTime = 0;



  dfsVisit(adMat,0);
  printf("\nHere is your boggle board.\n");
  for(int i = 0; i < size; i++){
      printf("%c ", board[i]);
      if((i+1) % 4 == 0)
        printf("\n");
  }

}




void dfsVisit(int graph[16][16],int u){

  char tmp = board[u];
  append(word,tmp);
  printf("\n%s", word);

  /*
  int i;
  visited[u] = 1;
  for(i = 0; i < 16; i++){
    if(!visited[i] && graph[u][i] == 1)
      dfsVisit(graph,i);
  }*/


  //textbook version don't work
  searchTime++;
  discovered[u] = searchTime;
  color[u] = 'g';
  for(int v = 0; v < size; v++){
    if(graph[u][v] == 1 && color[v] == 'w'){
      pi[v] = u;
      dfsVisit(graph,v);
    }
  }
  color[u] = 'b';
  searchTime++;
  found[u] = searchTime;
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

  //allocate memory for board
  board = malloc(sizeof(char) * size);

  int count = 0;
  for(int i = 0; i < size; i++){
      int diceFace = rand() % 6;
      board[i] = dice[randDice[count]][diceFace];
      count++;
  }
}
