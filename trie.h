//trie node struct
struct Node {
  struct Node *children[26];
  int isLeaf;
};


int charToIndex(char c){
  return (int)c - (int)'A';
}

//borrowed from geeksforgeeks
// Returns new trie node (initialized to NULLs)
struct Node *getNode(){
  struct Node *pNode = NULL;

  pNode = (struct Node *)malloc(sizeof(struct Node));

  if (pNode){
      int i;

      pNode->isLeaf = 0;

      for (i = 0; i < 26; i++)
          pNode->children[i] = NULL;
  }

  return pNode;
}

//borrowed from geeksforgeeks
void insert(struct Node *root, const char *key) {
    int level;
    int length = strlen(key);
    int index;

    struct Node *pCrawl = root;

    for (level = 0; level < length; level++){
        index = charToIndex(key[level]);
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isLeaf = 1;
}

//borrowed from geeksforgeeks
// Returns true if key presents in trie, else false
int search(struct Node *root, const char *key){
    int level;
    int length = strlen(key);
    int index;
    struct Node *pCrawl = root;

    for (level = 0; level < length; level++){
        index = charToIndex(key[level]);

        if (!pCrawl->children[index])
            return 0;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isLeaf);
}

int searchSubstring(struct Node *root, const char *key){
  int level;
  int length = strlen(key);
  int index;
  struct Node *pCrawl = root;

  for(level = 0; level < length; level++){
    index = charToIndex(key[level]);

    if (!pCrawl->children[index])
      return 0;

    pCrawl = pCrawl->children[index];
  }

  return (pCrawl != NULL);
}
