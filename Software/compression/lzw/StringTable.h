/* Header file for StringTable data structure for LZW compression algorithm
 * by Jared Katzman (11/9/14)
 */

typedef struct node *Node;

// Reserved codes: EMPTY = 0 ; ESC = 1; END = 2
enum code {EMPTY, ESC, END};

// Returns a node with PREFIX prefix CODE code and K k
// returned Node can be free'd using free()
// Returns NULL if malloc fails
Node createNode (unsigned char k, int prefix, int code);

// Initializes stringTable with 3 special codes
//   increases nCodes to 3 (current code number)
//   if E_FLAG inserts all 256 single-char strings
void initializeTable (int e_flag);

// == ENCODE methods == 

// Preforms pruning on stringTable
// #appearance is halved with the remainder ignored
// Unless ESCAPE is true - add every one-character string to the table
// Every string with #appearance > 0 is re-added to the table
void pruneTable(void);

// Inserts and returns a node with prefix CODE and char K into stringTable with code NCODES
// Returns NULL if invalid prefix CODE or the table is full
// if PRUNE_MODE is true:
//  will prune the table immiedietaly after the table is FILLED
Node insert (int code, unsigned char k, int prune_mode);

// Performs Binary search on PREFIX->Child based on the key K(Code)
// Returns the CODE of the node with (PREFIX,K)
// returns -1 if (PREFIX,K) is not in the stringTable
// Increases the COUNT of node (PREFIX, K)
int find (int prefix, unsigned char k);

// Performs binarySearch on array C looking for KEY which is K(NODE)
// L and U are the upper and lower index bounds
Node binarySearch (unsigned char key, Node *c, int l, int u);

// if nBits is 0 : nBits = tSize
// Output code C packaged into nBits
void output (int c, int nBits);

// === DECODE Methods ===

// Returns TRUE if CODE in stringTable
// FALSE otherwise
// if CODE is not a valid code DECODE can see - exits and prints 1-line error message to STDERR
bool findCode (int code);

// Finds finalK of code CODE in stringTable
// if OUTPUT: output to stdout the string associated with CODE
// if OUTPUT: increment COUNT of each node visited
unsigned char findK (int code, bool output);

// Runs findK (C, TRUE)
// Checks if in LOOK_AHEAD additional codes will prune/increase the size of the table
// If LOOK_AHEAD propagates a PRUNE - return EMPTY
// otherwise return C
int codeToChar (int C, int look_ahead);

// If nBits is 0: nBits = tSize
// Returns getBits (nBits)
int getcode (int nBits); 

// == Clean-up Methods == 

// Executes free(N->CHILD) and free(N)
void freeNode (Node n);

// Runs freeNode entries 0 to nCodes of StringTable
void destroyTable ();

// == Debug Methids == 

// Output stringTable to stdout in format:
// Code, Prefix, Char, Count
void printTable (void);

