//make dyanmic for large found word sets
void trimWords() {
  FILE *words;
  FILE *newWords;
  char** dictionary;
  int numWords = 0;

  clock_t start,end;
  double cpu_time_used;
  start = clock();

  words = fopen("foundWords.txt","r");
  if(words == NULL){
    printf("bad file\n");
    exit(1);
  }



  char str[35];
  while(fgets(str,35,words)!= NULL){
    numWords++;
  }

  printf("num found words %d\n",numWords);

  fclose(words);
  words = fopen("foundWords.txt","r");
  newWords = fopen("trimmedWords.txt","w");

  dictionary = malloc(sizeof(char*) * numWords);
  for(int i = 0; i < numWords; i++){
    dictionary[i] = malloc(sizeof(char) * 35);
  }

  char* word = malloc(sizeof(char) * 35);

  for(int i = 0; i < numWords; i++){
    int flag = 0;
    fscanf(words, "%s", word);
    for(int j = 0; j <= i; j++){
      if(strcmp(word,dictionary[j])==0)
        flag = 1;
    }
    if(!flag){
      strcpy(dictionary[i],word);
      fprintf(newWords, "%s\n", word);
      //printf("%s\n",word);
    }

  }
  fclose(words);
  fclose(newWords);

  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Trimmed words in %f seconds\n",cpu_time_used);
}
