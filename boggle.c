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
#include "adjacency.h"

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

char word[32] = {'\0'};

unsigned long long calls = 0;
double time_in_loop = 0.0;


//root of lookup trie
struct Node *root;
struct Node *foundRoot;

void initDice();
void buildBoard();
void findWords(int start, int* used, int* checked, int** graph, jmp_buf solved);
void append(char* s, char c);
void depend(char* s);

int main(int argc, char *argv[]) {
  printf("test\n");
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
  //strcpy(board,"LEIUNXMOIHWEMSIL");
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
  char dictWord[128] = "";
  while(fgets(dictWord,128,dict)){
    if(strlen(dictWord) > 2){
      dictWord[strlen(dictWord)-1] = '\0';
      insert(root, dictWord);
    }
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
  int** adList;
  adList = adjacencyList(dimension);

  /*printf("\n");
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++)
     printf("%d ",adMat[i][j]);
    printf("\n");
  }*/

  //initialize found words file
  foundWords = fopen("foundWords.txt", "w");

  //initialize neighbour arrays with proper values
  neighbours = malloc(sizeof(int) * size);
  for(int i = 0; i < size; i++)
    neighbours[i] =0;
  for(int i = 0; i < size; i++){
    for(int j = 0; j < 9; j++){
      if(adList[i][j] != -1)
        neighbours[i]++;
    }
  }

  used = malloc(sizeof(int) * size);
  checked = malloc(sizeof(int) * size);
  prevStack = malloc(sizeof(int) * size);

  /*for(int i = 0; i < size; i++){
    for(int j = 0; j < 9; j++){
      printf("%d ",adList[i][j]);
    }
    printf("\n");
  }*/
  foundRoot = getNode();
  for(int i = 0; i < size; i++){
    memset(used, 0, size * sizeof(used[0]));
    memset(checked, 0, size * sizeof(checked[0]));
    memset(prevStack, -1, size * sizeof(prevStack[0]));
    //printf("after other mallocs\n");
    word[0] = '\0';
    jmp_buf solved;
    startstart = i;
    if(!setjmp(solved)) findWords(i,used,checked,adList,solved);
  }


  /*int input2 = 0;
  printf("Enter a letter index to search from: ");
  scanf("%d", &input2);
  while(input2 != 16){
    findWords(input2,adMat);
    scanf("%d",&input2);
  }*/

  fclose(foundWords);
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Found all words in a %d x %d board in %f seconds\n",dimension,dimension,cpu_time_used);
  printf("took %llu calls\n",calls);

  int level = 0;
  char str[20];
  display(foundRoot,str,level);

  printf("Found all words in a %d x %d board in %f seconds\n",dimension,dimension,cpu_time_used);
  printf("took %llu calls\n",calls);
  printf("Time in loop in %f seconds\n",time_in_loop);

  //trimWords();




  /*printf("\nHere is your boggle board.\n");
  for(int i = 0; i < size; i++){
      printf("%c ", board[i]);
      if((i+1) % dimension == 0)
        printf("\n");
  }*/





}

//search w/out error printing
void findWords(int start, int* used, int* checked, int** graph, jmp_buf solved){
  calls++;
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
    if(start == startstart)
      longjmp(solved, 1);

    //dead-end procedure
    for(int i = start+1; i < start+dimension+3; i++){
      if(i < size)
      checked[i] = 0;
    }
    checked[start] = 1;
    used[start] = 0;
    depend(word);

    for(int i = 0; i < 9; i++)
      if(graph[prevStack[top]][i] != -1 && graph[prevStack[top]][i] < start &&
         !used[graph[prevStack[top]][i]])
        checked[graph[prevStack[top]][i]] = 1;

    start = prevStack[top];
    for(int i = top; i < size; i++)
      prevStack[i] = -1;
    top--;

    //////////////////
    clock_t loopstart,loopend;
    loopstart = clock();
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
    loopend = clock();
    time_in_loop += ((double) (loopend - loopstart)) / CLOCKS_PER_SEC;

    findWords(start,used,checked,graph,solved);
  }

  //modified dfs with used and checked instead of visited
  //used is part of the word, checked is a box that does not
  //create a valid prefix
  for(int i = 0; i < 9; i++){

    if(graph[start][i] != -1 && (!used[graph[start][i]] && !checked[graph[start][i]])){
      append(word,board[graph[start][i]]);
      if(searchSubstring(root,word)){
        if(search(root,word)){
          if(!search(foundRoot,word))
            insert(foundRoot,word);
        }
        used[graph[start][i]] = 1;
        top++;
        prevStack[top] = start;

        for(int j = 0; j < 9; j++){
          checked[graph[start][j]] = 0;
        }

        findWords(graph[start][i],used,checked,graph,solved);
      }
      else{
        depend(word);
        checked[graph[start][i]] = 1;

        findWords(start,used,checked,graph,solved);
      }
    }
  }
}

void initDice(){
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
/*void findWords(int start, int* used, int* checked, int** graph, jmp_buf solved){
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
          printf("\n");
    }
    printf("\n");

    for(int i = 0; i<size;i++)
      printf("%d ", prevStack[i]);
    printf("\n");

    //dead-end procedure
    for(int i = start+1; i < start+dim+3; i++){
      if(i < size)
        checked[i] = 0;
    }

    checked[start] = 1;
    used[start] = 0;
    depend(word);


    //error check printing
    printf("current tail %d\n",start);
    for(int i = 0; i<size;i++)
      printf("%d ", prevStack[i]);

    for(int i = 0; i < 9; i++){
      if(graph[prevStack[top]][i] != -1 && graph[prevStack[top]][i] < start &&
         !used[graph[prevStack[top]][i]])
        checked[graph[prevStack[top]][i]] = 1;
    }

    ///////
    //find cells adjacent to previous but not adjacent to start and
    //set to 0
    int flag = 0;
    for(int i = 0; i < 9; i++){
      for(int j = 0; j < 9; j++){
        if(graph[prevStack[top]][i] == graph[start][j])


      }
    }

    start = prevStack[top];
    for(int i = top; i < size; i++)
      prevStack[i] = -1;
    top--;
    printf("\nnew tail %d\n",start);

    for(int i = 0; i < size; i++){
      for(int j = 0; j < 9; j++){
        if(graph[start][j] == i)
          break;
        if(graph[start][j] == -1){
          checked[i] = 0;
          break;
        }
      }
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
          printf("\n");
    }
    printf("\n");

    findWords(start,used,checked,graph,solved);
  }




  //modified dfs with used and checked instead of visited
  //used is part of the word, checked is a box that does not
  //create a valid prefix
  for(int i = 0; i < 9; i++){

    if(graph[start][i] != -1 && (!used[graph[start][i]] && !checked[graph[start][i]])){
      append(word,board[graph[start][i]]);
      if(searchSubstring(root,word)){
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
              printf("\n");
        }
        printf("\n");

        if(search(root,word)){
          if (foundWords == NULL){
            printf("Error opening file!\n");
            exit(1);
          }
          fprintf(foundWords,"%s\n",word);
        }
        used[graph[start][i]] = 1;
        top++;
        prevStack[top] = start;

        for(int j = 0; j < 9; j++){
          checked[graph[start][j]] = 0;
        }


        printf("if %s\n",word);
        printf("start %d top %d\n",start,top);
        for(int j = 0; j<size;j++)
          printf("%d ", prevStack[j]);
        printf("\n\n");

        findWords(graph[start][i],used,checked,graph,solved);
      }
      else{
        printf("el %s\n",word);
        printf("start %d top %d\n",start,top);
        for(int j = 0; j<size;j++)
          printf("%d ", prevStack[j]);
        printf("\n\n");

        depend(word);
        checked[graph[start][i]] = 1;

        findWords(start,used,checked,graph,solved);
      }
    }
  }
}*/
