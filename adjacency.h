int dim;
int size;


int isMiddleNeighbour(int count, int k){
  if(k == count - dim - 1 || k == count - dim || k == count - dim + 1 ||
     k == count - 1       ||                     k == count + 1 ||
     k == count + dim - 1 || k == count + dim || k == count + dim + 1)
     return 1;
  else
     return 0;
}

int isCornerNeighbour(int count, int k){
  if(count == 0){
    if(k == 1 || k == count+dim || k == count+dim+1)
      return 1;
  }
  else if(count == dim-1){
    if(k == count - 1 || k == count + dim -1 || k == count + dim)
      return 1;
  }
  else if(count == size - dim){
    if(k == count - dim || k == count - dim + 1 || k == count + 1)
      return 1;
  }
  else if(count == size - 1){
    if(k == count - dim - 1 || k == count - dim || k == count - 1)
      return 1;
  }
  return 0;
}

int isEdgeNeighbour(int count, int k){
  //left edge
  if(count%dim == 0){
    if(k == count - dim || k == count - dim + 1 || k == count + 1 ||
       k == count + dim || k == count + dim + 1)
       return 1;
  }
  //top edge
  else if(count < dim){
    if(k == count - 1 ||  k == count + 1 || k == count + dim - 1 ||
       k == count + dim || k == count + dim + 1)
       return 1;
  }
  //right edge
  else if((count%dim) == (dim-1)){
    if(k == count - dim - 1 || k == count - dim || k == count - 1 ||
       k == count + dim - 1 || k == count + dim)
       return 1;
  }
  //bottom edge
  else if(count > size-dim){
    if(k == count - dim - 1 || k == count - dim || k == count - dim + 1 ||
       k == count - 1 || k == count + 1)
       return 1;
  }
  return  0;
}

int** adjacencyMatrix(int n){
  dim = n;
  size = n*n;

  int **matrix = (int **)malloc(size * sizeof(int *));
  for(int i=0; i<size; i++)
       matrix[i] = (int *)malloc(size * sizeof(int));
  for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++)
      matrix[i][j] = 0;


 //type could be corner, edge or middle
 //1 = corner, 2 = edge, 0 = middle
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

 //assign proper adjacency matrix values
 int count = 0;
 for(int i = 0; i < dim; i++){
   for(int j = 0; j < dim; j++){
     if(type[i][j]==0)
       for(int k = 0; k < size; k++)
          matrix[count][k] = isMiddleNeighbour(count,k);
     else if(type[i][j]==1)
       for(int k = 0; k < size; k++)
          matrix[count][k] = isCornerNeighbour(count,k);
     else if(type[i][j]==2)
       for(int k = 0; k < size; k++)
          matrix[count][k] = isEdgeNeighbour(count,k);

     count++;
   }
 }

 return matrix;
}

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
  else if(index == size - dim){
    int k = 0;
    list[index][k] = index - dim;
    k++;
    list[index][k] = index - dim + 1;
    k++;
    list[index][k] = index + 1;
  }
  else if(index == size - 1){
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
  else if(index > size-dim){
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
  size = n*n;

  list = (int **)malloc(size * sizeof(int *));
  for(int i=0; i<size; i++)
       list[i] = (int *)malloc(9 * sizeof(int));
  for(int i = 0; i < size; i++)
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
