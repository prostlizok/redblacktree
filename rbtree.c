#include <stdlib.h>
#include <stdio.h>
#include "rbtree.h"

// Colours for terminal
#define RED_COLOUR       "\x1B[31m"
#define WHITE_COLOUR     "\x1B[37m"


// Internal functions
static RB_Node *insert_value(RB_Node *tree, int value);
static void left_rotate(RB_Node *pivot);
static void right_rotate(RB_Node *pivot);
static void rebalance(RB_Node *node, RB_Node **root);
static void rotate(RB_Node *pivot, RB_Node **root);


// Printing functions
void padding(char ch, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        putchar(ch);
    }
}

void tree_print(RB_Node *root, int level)
{
    int i;

    if (root == NULL)
    {
        padding('\t', level);
        puts("◯");
    }
    else
    {
        tree_print(root->right, level + 1);
        padding('\t', level);
        if (root->colour == RED)
        {
            printf("%s%d%s\n", RED_COLOUR, root->value, WHITE_COLOUR);
        }
        else
        {
            printf("%d\n", root->value);
        }
        tree_print(root->left, level + 1);
    }
}


// Tree family initiazers
RB_Node *father(RB_Node *node)
{
    return node->parent;
}

RB_Node *grandpa(RB_Node *node)
{
    if (node->parent == NULL)
    {
        return NULL;
    }
    else
    {
        return node->parent->parent;
    }
}

RB_Node *brother(RB_Node *node)
{
    if (node->parent == NULL)
    {
        return NULL;
    }
    else
    {
        if (node == node->parent->left)
        {
            return node->parent->right;
        }
        else if (node == node->parent->right)
        {
            return node->parent->left;
        }
        else
        {
            exit(1);
        }
    }
}

RB_Node *uncle(RB_Node *node)
{
    RB_Node *father_node;
    father_node = father(node);

    if (father_node == NULL)
    {
        return NULL;
    }
    else
    {
        return brother(father_node);
    }
}


// Creating new node
RB_Node *new_node()
{
    RB_Node *ptr = malloc(sizeof(RB_Node));
    ptr->left = NULL;
    ptr->right = NULL;
    ptr->parent = NULL;
    return ptr;
}


// Function to free space after malloc
void free_tree(RB_Node *node)
{
    if (node != NULL)
    {
        free_tree(node->left);
        free_tree(node->right);
        free(node);
    }
}
bool value_less(int a, int b)
{
    return a < b;
}
bool value_great(int a, int b)
{
    return a > b;
}


/*  Inserting functions
    Rebalance function - changes colours of nodes and do rortations
    Value less/great - comparing two nodes by values
    Insert value - inserts node's value in tree
    Insert function - inserts value with checks and creating necessery conditions for new node
*/
void rebalance(RB_Node *node, RB_Node **root)
{
    RB_Node *father_node, *grandpa_node, *uncle_node;

    father_node = father(node);
    grandpa_node = grandpa(node);
    uncle_node = uncle(node);

    // Three cases when we dont have to do anything 
    if (node->colour == BLACK)
    {
        return;
    }
    if (father_node == NULL)
    {
        node->colour = BLACK;
        return;
    }
    if (father_node->colour == BLACK)
    {
        return;
    }

    if (uncle_node != NULL && uncle_node->colour == RED) // Case 1
    {
        father_node->colour = BLACK;
        uncle_node->colour = BLACK;
        grandpa_node->colour = RED;
        rebalance(grandpa_node, root);
        return;
    }

    // if father and son are not on the same side
    if (is_left_pos(father_node) ^ is_left_pos(node))
    {
        // rotate son to put them on the same side
        rotate(node, root);
        node->colour = BLACK;
        grandpa_node->colour = RED;
        rotate(node, root);
        rebalance(node, root);
    }
    else
    {
        father_node->colour = BLACK;
        grandpa_node->colour = RED;
        rotate(father_node, root);
        rebalance(father_node, root);
    }
}

void insert(RB_Node **tree, int value)
{

    // Initiation of new tree
    if (*tree == NULL)
    {
        *tree = new_node();
        (*tree)->value = value;
        (*tree)->colour = BLACK;
    }
    else
    {
        rebalance(insert_value(*tree, value), tree);
    }
}

RB_Node *insert_value(RB_Node *tree, int value)
{
    RB_Node *parent;
    while (tree != NULL)
    {
        parent = tree;
        if (value_great(value, tree->value))
        {
            tree = tree->right;
        }
        else if (value_less(value, tree->value))
        {
            tree = tree->left;
        }
        else
        {
            printf("This value already in the tree!\n");
            exit(1);
        }
    }

    tree = new_node();
    tree->value = value;
    tree->colour = RED;
    tree->parent = parent;


    if (value_great(value, parent->value))
    {
        parent->right = tree;
    }
    else
    {
        parent->left = tree;
    }
    return tree;
}


// Rotation functions
bool is_left_pos(RB_Node *node)
{
    RB_Node *father_node;
    father_node = father(node);
    if (node == father_node->left)
    {
        return true;
    }
    else if (node == father_node->right)
    {
        return false;
    }
    else
    {
        exit(1);
    }
}
bool is_right_pos(RB_Node *node)
{
    return !is_left_pos(node);
}

void rotate(RB_Node *pivot, RB_Node **root)
{

    if (is_left_pos(pivot))
    {
        right_rotate(pivot);
        if (pivot->right == *root)
        {
            *root = pivot;
        }
    }
    else
    {
        left_rotate(pivot);
        if (pivot->left == *root)
        {
            *root = pivot;
        }
    }
}

void left_rotate(RB_Node *pivot)
{
    RB_Node *parent, *root;

    parent = pivot->parent;

    root = parent->parent;

    // linking parent's right child to pivot's left child
    parent->right = pivot->left;
    if (pivot->left != NULL)
    {
        pivot->left->parent = parent;
    }

    // linking pivot to root's child
    pivot->parent = root;
    if (root != NULL)
    {
        if (parent == root->left)
        {
            root->left = pivot;
        }
        else
        {
            root->right = pivot;
        }
    }

    // linking pivot's left child to parent
    pivot->left = parent;
    parent->parent = pivot;

}
void right_rotate(RB_Node *pivot)
{
    RB_Node *parent, *root;

    parent = pivot->parent;
    root = parent->parent;

    // linking parent's left child to pivot's right child
    parent->left = pivot->right;
    if (pivot->right != NULL)
    {
        pivot->right->parent = parent;
    }

    // linking pivot to root's child
    pivot->parent = root;
    if (root != NULL)
    {
        if (parent == root->left)
        {
            root->left = pivot;
        }
        else
        {
            root->right = pivot;
        }
    }

    // linking pivot's right child to parent
    pivot->right = parent;
    parent->parent = pivot;

}


// Checking all properties of tree
int property_test(RB_Node *node)
{
    int left_depth, right_depth;

    if (node == NULL)
    {
        return 1;
    }

    // prop 1: Every node is either red or black.

    // prop 2: All NULL nodes are considered black.

    // prop 3: A red node does not have a red child.

    if (node->colour == RED)
    {
        if (node->left != NULL && node->left->colour == RED)
        {
            return -1;
        }
        if (node->right != NULL && node->right->colour == RED)
        {
            return -1;
        }
    }

    // prop 5: The root of a tree should be black.

    if (node->parent == NULL && node->colour == RED)
    {
        return -1;
    }

    // prop 4: Every path from a given node to any of its descendant NULL nodes
    //         goes through the same number of black nodes.

    left_depth = property_test(node->left);
    if (left_depth == -1)
    {
        return -1;
    }

    right_depth = property_test(node->right);
    if (right_depth == -1)
    {
        return -1;
    }

    if (left_depth != right_depth)
    {
        return -1;
    }

    if (node->colour == RED)
    {
        return left_depth;
    }
    else
    {
        return left_depth + 1;
    }
}


// Sort tree and return an array
void sorted_tree(RB_Node *node, int *arr)
{
    static int n = 0;
    if (node != NULL)
    {
        sorted_tree(node->left, arr);
        arr[n] = node->value;
        n++;
        sorted_tree(node->right, arr);
    }
}