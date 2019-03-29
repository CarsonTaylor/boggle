//trie node struct
struct Node {
  struct Node *children[26];
  int isLeaf;
};


int charToIndex(char c){
  return (int)c - (int)'a';
}

//borrowed from geeksforgeeks
// Returns new trie node (initialized to NULLs)
struct Node *getNode(){
  struct Node *pNode = NULL;

  pNode = (struct Node *)malloc(sizeof(struct Node));

  if (pNode){
      pNode->isLeaf = 0;

      for (int i = 0; i < 26; i++)
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

//modified from geeksforgeeks
void display(struct Node* root, char str[], int level, int* tab, unsigned long* wordCount)
{
    // If node is leaf node, it indiicates end
    // of string, so a null charcter is added
    // and string is displayed
    if (root->isLeaf){
        *wordCount = *wordCount + 1;
        str[level] = '\0';
        if(strlen(str) < 8)
          printf("%s\t\t\t",str);
        else if(strlen(str) < 16)
          printf("%s\t\t",str);
        else
          printf("%s\t",str);
        *tab = *tab + 1;
        if(*tab%4==0)
          printf("\n");
    }

    int i;
    for (i = 0; i < 26; i++){
        // if NON NULL child is found
        // add parent key to str and
        // call the display function recursively
        // for child node
        if (root->children[i]){
            str[level] = i + 'a';
            display(root->children[i], str, level + 1,tab,wordCount);
        }
    }
}
