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
//any size board

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <setjmp.h>

#include "trie.h"
#include "trimWords.h"
#include "adjacency.c"

//variables for board
char** dice;
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

int startstart;

char word[17] = {'\0'};


//root of lookup trie
struct Node *root;

void initDice();
void buildBoard();
void findWords(int start, int* used, int* checked, int** graph, jmp_buf solved);
void append(char* s, char c);
void depend(char* s);

int main(int argc, char *argv[]) {
  //gets and sets dimension and size of board
  size = 0;
  dimension = 0;
  printf("Enter board dimension n -- (board will be n x n squares)\n");
  scanf("%d",&dimension);
  size = dimension * dimension;

  clock_t start,end;
  double cpu_time_used;
  start = clock();

  //initializes board with size of 4x4, add code
  //for arbitrary board size later.
  initDice();
  //builds random board
  buildBoard();

  ///////////TEST BOARD/////////
  ///////////////////////////////
  //board = malloc(sizeof(char) * size);
  //strcpy(board,"COXTBHGAIONENWMC");
  ////////////////////////////////

  printf("\nHere is your boggle board.\n");
  for(int i = 0; i < size; i++){
      printf("%c ", board[i]);
      if((i+1) % dimension == 0)
        printf("\n");
  }
  printf("\n");

  //buildTrie
  FILE* dict = fopen("dict.txt", "r");
  root = getNode();
  for (int i = 0; i < 194206; i++){
      char* word = malloc(sizeof(char) * 32);
      fscanf(dict, "%s", word);
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
  /*int adMat[16][16] = {
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
        };*/
  int** adMat;
  adMat = adjacencyMatrix(dimension);

  printf("\n");
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++)
     printf("%d ",adMat[i][j]);
    printf("\n");
  }

  //initialize found words file
  foundWords = fopen("foundWords.txt", "w");

  //initialize neighbour arrays with proper values
  neighbours = malloc(sizeof(int) * size);
  for(int i = 0; i < size; i++)
    neighbours[i] =0;
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      if(adMat[i][j])
        neighbours[i]++;
    }
  }

  printf(" -neighbours- \n");
  for(int i = 0; i < size; i++){
      printf("%d ", neighbours[i]);
      if((i+1) % dimension == 0)
        printf("\n");
  }

  printf("after neighbour malloc\n");

  used = malloc(sizeof(int) * size);
  checked = malloc(sizeof(int) * size);
  prevStack = malloc(sizeof(int) * size);

  for(int i = 0; i < size; i++){
    for(int i = 0; i < size; i++){
      used[i] = 0;
      checked[i] = 0;
      prevStack[i] = -1;
    }
    printf("after other mallocs\n");
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

  fclose(foundWords);
  trimWords();



  printf("\nHere is your boggle board.\n");
  for(int i = 0; i < size; i++){
      printf("%c ", board[i]);
      if((i+1) % dimension == 0)
        printf("\n");
  }

  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Found all words in a %d x %d board in %f seconds\n",dimension,dimension,cpu_time_used);


}

//search w/out error printing
/*void findWords(int start, int* used, int* checked, int** graph, jmp_buf solved){
  //checks if first call
  if(!used[start]){
    append(word,board[start]);
    used[start] = 1;
  }

  //determines number of neighbours visited
  int neighboursVisited = 0;
  for(int i = 0; i < size; i++){
    if(graph[start][i] && (checked[i] || used[i]))
      neighboursVisited++;
  }

  //checks to see if all neighbours visited, if so, runs dead end procedure
  if(neighboursVisited == neighbours[start]){
    //BASE CASE - if back at beginning search index, hard return
    if(start == startstart)
      longjmp(solved, 1);

    //dead-end procedure
    //uncheck appropiate squarea
    for(int i = start+1; i < size; i++){
      checked[i] = 0;
    }

    checked[start] = 1;
    used[start] = 0;
    depend(word);

    //set appropiate checked squares for new tail
    for(int i = 0; i < size; i++)
      if(graph[prevStack[top]][i] && i < start && !used[i])
        checked[i] = 1;

    //set new tail of word from stack
    start = prevStack[top];
    for(int i = top; i < size; i++)
      prevStack[i] = -1;
    top--;

    //reset non-adjacent squares
    for(int i = 0; i < size; i++){
      if(!graph[start][i])
        checked[i] = 0;
    }

    findWords(start,used,checked,graph,solved);
  }

  //modified dfs with used and checked instead of visited
  //used is part of the word, checked is a box that does not
  //create a valid prefix
  for(int i = 0; i < size; i++){
    if(graph[start][i] && (!used[i] && !checked[i])){
      append(word,board[i]);
      //if new word is valid prefix, check for word and continue
      if(searchSubstring(root,word)){
        if(search(root,word)){
          if (foundWords == NULL){
            printf("Error opening file!\n");
            exit(1);
          }
          fprintf(foundWords,"%s\n",word);
        }

        used[i] = 1;
        top++;
        prevStack[top] = start;

        for(int i = 0; i < size; i++){
          checked[i] = 0;
        }

        findWords(i,used,checked,graph,solved);
      }
      //else mark that square checked and backtrack
      else{
        depend(word);
        checked[i] = 1;

        findWords(start,used,checked,graph,solved);
      }
    }
  }
}*/

void initDice(){
  printf("top of initDice\n");
  //6 is number of sides on a dice
  dice = malloc(sizeof(char*) * 16);
  for(int i = 0; i < 16; i++)
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

//this method works, with error check printing
void findWords(int start, int* used, int* checked, int** graph, jmp_buf solved){
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
    //BASE CASE - if back at beginning search index, hard return
    if(start == startstart)
      longjmp(solved, 1);

    //error check printing
    printf("\n");
    for(int i = 0; i < size; i++){
      printf("%d ", used[i]);
      if((i+1) % dimension == 0)
        printf("\n");
    }
    printf("\n");
    printf("\n");
    for(int i = 0; i < size; i++){
        printf("%d ",checked[i]);
        if((i+1) % dimension == 0)
          printf("\n");;
    }
    printf("\n");

    //dead-end procedure
    for(int i = start+1; i < size; i++){
      checked[i] = 0;
    }
    checked[start] = 1;
    used[start] = 0;
    depend(word);

    //error check printing
    printf("current tail %d\n",start);
    for(int i = 0; i<size;i++)
      printf("%d ", prevStack[i]);

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
      if((i+1) % dimension == 0)
        printf("\n");
    }
    printf("\n");
    printf("\n");
    for(int i = 0; i < size; i++){
        printf("%d ",checked[i]);
        if((i+1) % dimension == 0)
          printf("\n");;
    }
    printf("\n");

    printf("%d\n", start);
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
          if (foundWords == NULL){
            printf("Error opening file!\n");
            exit(1);
          }
          fprintf(foundWords,"%s\n",word);
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
