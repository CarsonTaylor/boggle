//variables for index calculations
int dim;
int sz;

//sets neighbouring indexes in ascending order for squares in the middle
void setMiddleNeighbours(int** list, int index){
  int k = 0;
  list[index][k] = index - dim - 1;
  k++;
  list[index][k] = index - dim;
  k++;
  list[index][k] = index - dim + 1;
  k++;
  list[index][k] = index - 1;
  k++;
  list[index][k] = index + 1;
  k++;
  list[index][k] = index + dim - 1;
  k++;
  list[index][k] = index + dim;
  k++;
  list[index][k] = index + dim + 1;

  return;
}

//sets neighbouring indexes in ascending order for squares in the corners
void setCornerNeighbours(int** list, int index){
  //top left corner
  if(index == 0){
    int k = 0;
    list[index][k] = index + 1;
    k++;
    list[index][k] = index + dim;
    k++;
    list[index][k] = index + dim + 1;
  }
  //top right corner
  else if(index == dim-1){
    int k = 0;
    list[index][k] = index - 1;
    k++;
    list[index][k] = index + dim - 1;
    k++;
    list[index][k] = index + dim;
  }
  //bottom left corner
  else if(index == sz - dim){
    int k = 0;
    list[index][k] = index - dim;
    k++;
    list[index][k] = index - dim + 1;
    k++;
    list[index][k] = index + 1;
  }
  //bottom right corner
  else if(index == sz - 1){
    int k = 0;
    list[index][k] = index - dim - 1;
    k++;
    list[index][k] = index - dim;
    k++;
    list[index][k] = index - 1;
  }
  return;
}

//sets neighbouring indexes in ascending order for squares on the edges
void setEdgeNeighbours(int** list, int index){
  //left edge
  if(index%dim == 0){
     int k = 0;
     list[index][k] = index - dim;
     k++;
     list[index][k] = index - dim + 1;
     k++;
     list[index][k] = index + 1;
     k++;
     list[index][k] = index + dim;
     k++;
     list[index][k] = index + dim + 1;
  }
  //top edge
  else if(index < dim){
     int k = 0;
     list[index][k] = index - 1;
     k++;
     list[index][k] = index + 1;
     k++;
     list[index][k] = index + dim - 1;
     k++;
     list[index][k] = index + dim;
     k++;
     list[index][k] = index + dim + 1;
  }
  //right edge
  else if((index%dim) == (dim-1)){
     int k = 0;
     list[index][k] = index - dim - 1;
     k++;
     list[index][k] = index - dim;
     k++;
     list[index][k] = index - 1;
     k++;
     list[index][k] = index + dim - 1;
     k++;
     list[index][k] = index + dim;
  }
  //bottom edge
  else if(index > sz-dim){
     int k = 0;
     list[index][k] = index - dim - 1;
     k++;
     list[index][k] = index - dim;
     k++;
     list[index][k] = index - dim + 1;
     k++;
     list[index][k] = index - 1;
     k++;
     list[index][k] = index + 1;
  }
  return;
}

//returns an adjacency list representing a square boggle
//board of dimension n
int** adjacencyList(int n){
  int** list;
  dim = n;
  sz = n*n;

  //allocate and initialize list
  list = (int **)malloc(sz * sizeof(int *));
  for(int i=0; i<sz; i++)
       list[i] = (int *)malloc(9 * sizeof(int));
  for(int i = 0; i < sz; i++)
    for(int j = 0; j < 9; j++)
      list[i][j] = -1;

  //allocate array for types of squares
  int **type = (int **)malloc(dim * sizeof(int *));
  for(int i=0; i<dim; i++)
       type[i] = (int *)malloc(dim * sizeof(int));
  //initialize type array
  for(int i = 0; i < dim; i++)
    for(int j = 0; j < dim; j++)
      type[i][j] = 0;

  //set type array values for corners and edges
  for(int i = 0; i < dim; i++){
    for(int j = 0; j < dim; j++){
      //corners
      if((i==0 && (j == 0 || j == dim-1)) || (i==dim-1 && (j == 0 || j == dim-1)))
       type[i][j] = 1;
      //edges
      else if((j == 0 && type[i][j] == 0) || (j == dim-1 && type[i][j] == 0) ||
              (i == 0 && type[i][j] == 0) || (i == dim-1 && type[i][j] == 0))
       type[i][j] = 2;
    }
  }

  //assign appropiate neighbours for each index based on type
  int count = 0;
  for(int i = 0; i < dim; i++){
    for(int j = 0; j < dim; j++){
      if(type[i][j]==0)
        setMiddleNeighbours(list,count);
      else if(type[i][j]==1)
        setCornerNeighbours(list,count);
      else if(type[i][j]==2)
        setEdgeNeighbours(list,count);
      count++;
    }
  }

  return list;
}
