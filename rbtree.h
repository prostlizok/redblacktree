#include <stdbool.h>

// Main structures
typedef enum Colour 
{
   RED, 
   BLACK 
} Colour;
       

typedef struct RB_Node 
{
    int value;
    Colour colour;
    struct RB_Node *left;
    struct RB_Node *right;
    struct RB_Node *parent;
} RB_Node;


// Printing functions
void padding(char ch, int n);
void tree_print(RB_Node *root, int level);


// Tree family initiazers
RB_Node *father(RB_Node *node);
RB_Node *grandpa(RB_Node *node);
RB_Node *brother(RB_Node *node);
RB_Node *uncle(RB_Node *node);

// Creating new node
RB_Node *new_node();

// Function to free space after malloc
void free_tree(RB_Node *node);

// Main functions
void insert(RB_Node **tree, int value);
void sorted_tree(RB_Node *node, int *arr);

// Rotation functions
bool is_left_pos(RB_Node *node);
bool is_right_pos(RB_Node *node);

// Test function
int property_test(RB_Node *node);

