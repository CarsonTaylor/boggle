int dim;
int sz;

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

void setCornerNeighbours(int** list, int index){
  if(index == 0){
    int k = 0;
    list[index][k] = index + 1;
    k++;
    list[index][k] = index + dim;
    k++;
    list[index][k] = index + dim + 1;
  }
  else if(index == dim-1){
    int k = 0;
    list[index][k] = index - 1;
    k++;
    list[index][k] = index + dim - 1;
    k++;
    list[index][k] = index + dim;
  }
  else if(index == sz - dim){
    int k = 0;
    list[index][k] = index - dim;
    k++;
    list[index][k] = index - dim + 1;
    k++;
    list[index][k] = index + 1;
  }
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

int** adjacencyList(int n){
  int** list;
  dim = n;
  sz = n*n;

  list = (int **)malloc(sz * sizeof(int *));
  for(int i=0; i<sz; i++)
       list[i] = (int *)malloc(9 * sizeof(int));
  for(int i = 0; i < sz; i++)
    for(int j = 0; j < 9; j++)
      list[i][j] = -1;

  int **type = (int **)malloc(dim * sizeof(int *));
  for(int i=0; i<dim; i++)
       type[i] = (int *)malloc(dim * sizeof(int));

   for(int i = 0; i < dim; i++)
     for(int j = 0; j < dim; j++)
       type[i][j] = 0;

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
