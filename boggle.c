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
#include <setjmp.h>

#include "trie.h"
#include "trimWords.h"



//build a trie from dictionary
char** dice;
int size;
char* board;

FILE* words;



int* discovered;
char* color;
int* pi;
int* found;
int searchTime;

int used[16];
int checked[16];
int neighbours[16];

int prevStack[16];
int top = -1;

int startstart;

char word[17] = {'\0'};

struct Node *root;

void initDice();
void buildBoard();
void dfsVisit(int graph[16][16],int u);
void searchAdjacent(int graph[16][16],int i);
void findWords(int start, int used[16], int checked[16], int graph[16][16], jmp_buf solved);

void append(char* s, char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

void depend(char* s) {
        int len = strlen(s);
        s[len-1] = '\0';
}

int main(int argc, char *argv[]) {
  //initializes board with size of 4x4, add code
  //for arbitrary board size later.
  initDice();
  //builds random board
  buildBoard();

  ///////////TEST BOARD/////////
  ///////////////////////////////
  //board = malloc(sizeof(char) * size);
  //strcpy(board,"RORMOKTBFSAINENE");
  ////////////////////////////////

  printf("\nHere is your boggle board.\n");
  for(int i = 0; i < size; i++){
      printf("%c ", board[i]);
      if((i+1) % 4 == 0)
        printf("\n");
  }
  printf("\n");

  //buildTrie
  FILE* dict = fopen("dict.txt", "r");
  root = getNode();
  for (int i = 0; i < 194206; i++){
      char* word = malloc(sizeof(char) * 32);
      fscanf(dict, "%s", word);
      //printf("here %d ",i);
      insert(root, word);
      free(word);
  }
  fclose(dict);

  //test and search trie
  /*char* input = malloc(sizeof(char) * 30);
  printf("Enter a substring to find in the dictionary: ");
  scanf("%s", input);
  while(strcmp(input, "zzz") != 0){
    bool result = searchSubstring(root,input);
    printf("%d\n", result);
    scanf("%s",input);
  }*/


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

  //web dfs
  /*for(int i = 0; i < 16; i++)
    visited[i]=0;*/

  //DFS textbook version

  //old dynamic handling
  /*discovered = malloc(sizeof(int)*size);
  color = malloc(sizeof(char)*size);
  pi = malloc(sizeof(int)*size);
  found = malloc(sizeof(int)*size);

  for(int i = 0; i < 16; i++){
    color[i] = 'w';
    pi[i] = -1;
  }
  searchTime = 0;


  dfsVisit(adMat,0);*/

  //finding words
  words = fopen("foundWords.txt", "w");

  //initialize neighbour arrays with proper values
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      if(adMat[i][j])
        neighbours[i]++;
    }
  }

  for(int i = 0; i < size; i++){
    for(int i = 0; i < size; i++){
      used[i] = 0;
      checked[i] = 0;
      prevStack[i] = -1;
    }
    word[0] = '\0';
    jmp_buf solved;
    startstart = i;
    if(!setjmp(solved)) findWords(i,used,checked,adMat,solved);
  }


  /*int input2 = 0;
  printf("Enter a letter index to search from: ");
  scanf("%d", &input2);
  while(input2 != 16){
    findWords(input2,adMat);
    scanf("%d",&input2);
  }*/






  //first stab at finding words
  /*for(int i = 0; i < size; i++){
    append(word,board[i]);
    visited[i] = 1;
    char rootLetter[17] = {board[i]};

    for(int j = 0; j < size; j++){
      if(adMat[i][j] && !visited[j]){
        append(word,board[j]);
        visited[j] = 1;
        int valid = searchSubstring(root,word);
        if(valid){
          searchAdjacent(adMat,i);
          printf("valid prefix %s\n", word);
        }
        strcpy(word,rootLetter);
        visited[j] = 0;
      }
    }

    strcpy(word,"\0");
    visited[i] = 0;
  }*/

  fclose(words);
  trimWords();

  printf("\nHere is your boggle board.\n");
  for(int i = 0; i < size; i++){
      printf("%c ", board[i]);
      if((i+1) % 4 == 0)
        printf("\n");
  }


}

void findWords(int start, int used[16], int checked[16], int graph[16][16], jmp_buf solved){
  //checks if first call
  if(!used[start]){
    append(word,board[start]);
    used[start] = 1;
  }



  //checks to see if neighbours visited, if so, runs dead end procedure
  //prints checked and used arrays for error checking
  int neighboursVisited = 0;
  for(int i = 0; i < size; i++){
    if(graph[start][i] && (checked[i] || used[i]))
      neighboursVisited++;
  }
  printf("nv %d\n", neighboursVisited);
  if(neighboursVisited == neighbours[start]){
    if(start == startstart)
      longjmp(solved, 1);
    //error check printing
    printf("\n");
    for(int i = 0; i < size; i++){
      printf("%d ", used[i]);
      if((i+1) % 4 == 0)
        printf("\n");
    }
    printf("\n");
    printf("\n");
    for(int i = 0; i < size; i++){
        printf("%d ",checked[i]);
        if((i+1) % 4 == 0)
          printf("\n");;
    }
    printf("\n");

    //dead-end procedure
    /*for(int i = 0; i < size; i++){
      if(checked[i])
        used[i] = 0;
    }*/

    ///////////////////////
    //problem is here//////
    //////////////////////
    for(int i = start+1; i < size; i++){
      checked[i] = 0;
    }
    checked[start] = 1;
    used[start] = 0;
    depend(word);
    //find tail
    /*int tail = 0;
    for(int i = 0; i < size; i++){
      if(used[i])
        tail++;
      if(tail == strlen(word)){
        tail = i;
        break;
      }
    }*/
    printf("current tail %d\n",start);
    for(int i = 0; i<size;i++)
      printf("%d ", prevStack[i]);


    //findWords(start,used,checked,graph);


    //find here////////////////////
    //if(start > prevStack[top])
      for(int i = 0; i < size; i++)
        if(graph[prevStack[top]][i] && i < start && !used[i])
          checked[i] = 1;

    start = prevStack[top];
    for(int i = top; i < size; i++)
      prevStack[i] = -1;
    top--;
    printf("\nnew tail %d\n",start);
    for(int i = 0; i < size; i++){
      if(!graph[start][i])
        checked[i] = 0;
    }

    //error check printing
    printf("\n\n");
    for(int i = 0; i < size; i++){
      printf("%d ", used[i]);
      if((i+1) % 4 == 0)
        printf("\n");
    }
    printf("\n");
    printf("\n");
    for(int i = 0; i < size; i++){
        printf("%d ",checked[i]);
        if((i+1) % 4 == 0)
          printf("\n");;
    }
    printf("\n");

    findWords(start,used,checked,graph,solved);

  }




  //modified dfs with used and checked instead of visited
  //used is part of the word, checked is a box that does not
  //create a valid prefix
  for(int i = 0; i < size; i++){
    if(graph[start][i] && (!used[i] && !checked[i])){
      append(word,board[i]);
      if(searchSubstring(root,word)){
        if(search(root,word)){
          if (words == NULL){
            printf("Error opening file!\n");
            exit(1);
          }
          fprintf(words,"%s\n",word);
        }
        used[i] = 1;
        top++;
        prevStack[top] = start;

        for(int i = 0; i < size; i++){
          checked[i] = 0;
        }


        printf("if %s\n",word);
        printf("start %d top %d\n",start,top);
        for(int j = 0; j<size;j++)
          printf("%d ", prevStack[j]);
        printf("\n\n");

        findWords(i,used,checked,graph,solved);
      }
      else{
        printf("el %s\n",word);
        printf("start %d top %d\n",start,top);
        for(int j = 0; j<size;j++)
          printf("%d ", prevStack[j]);
        printf("\n\n");

        depend(word);
        checked[i] = 1;

        findWords(start,used,checked,graph,solved);
      }
    }

  }
}



void dfsVisit(int graph[16][16],int u){
  append(word,board[u]);
  printf("\n%s", word);

  /*
  int i;
  visited[u] = 1;
  for(i = 0; i < 16; i++){
    if(!visited[i] && graph[u][i] == 1)
      dfsVisit(graph,i);
  }*/


  //textbook version
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
