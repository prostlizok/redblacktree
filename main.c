#include <stdlib.h>
#include <stdio.h>
#include "rbtree.h"


void write_txt(FILE *file, int *arr, int n)
{
    file = fopen("data.txt", "w");

    if (file == NULL)
    {
        printf("Error. Can't read the file:(");
    }

    int i;
    fprintf(file, "[");

    for (i = 0; i < n; i++)
    {
        if (i == (n - 1))
        {
            fprintf(file, "%d]\n", arr[i]);
        }
        else
        {
            fprintf(file, "%d, ", arr[i]);
        }
    }
    
    fclose(file);
}


void print_array(int *array, int n)
{
    int i;
    printf("[");
    for (i = 0; i < n; i++)
    {
        if (i == (n - 1))
        {
            printf("%d]\n", array[i]);
        }
        else
        {
            printf("%d, ", array[i]);
        }
    }
}


void menu_print()
{
    printf("1 - Show the RB tree\n");
    printf("2 - Get sorted array\n");
    printf("3 - Exit\n");
}


// Check if string is a number
int check_num(char *msg)
{
    long j = 0;

    if (msg[0]== '-')
    {
        j=1;
    }

    while (msg[j]!= 0 && msg[j]!='\n')
    {
        if(!(msg[j]>= '0'  && msg[j] <='9'))
        {
            return 0;
        }
        j++;
    }
    return 1;
}


// Input int function
int input_int(const char *msg)
{
    char buffer[128];

    printf("%s ", msg);
    fgets(buffer, 128, stdin);
    if (buffer[0] == '\n')
    {
        return 0;
    }

    if (check_num(buffer))
    {
        char *end;
        int number = strtol(buffer,&end,10);
        return number;
    }
    else
    {
        printf("Error. Enter an integer.\n");
        return -1;
    }
}

// Testing tree functions
int main()
{
    // Initializing a tree
    RB_Node *node = NULL;
    FILE *file = NULL;

    int status = 1;
    int choice;
    int elements_num = 0;


    // Adding elements in tree 
    printf("Red-black tree test!\n");
    int add_status = 1;
    printf("We'll start from adding elements to the tree.\n");
    printf("Attention: you're can't type numbers, that've been added previously.\nProgramm will inform you about error and end.\n");
    printf("To stop entering typt 0 or Enter\n");
    while (add_status)
    {
        add_status = input_int("Type a number(only integers): ");
        if (add_status != 0)
        {
            insert(&node, add_status);
            elements_num++;
        }
    }

    int arr[elements_num];
    while (status != 0)
    {

        menu_print();
        choice = input_int("Your choice: ");

        if (choice == 1)
        {
            // Tree visualization
            tree_print(node, 0);
        }
        else if (choice == 2)
        {
            if (node == NULL)
            {
                printf("Tree doesn't contain any elements.\n");
            }
            else
            {
                
                // Sort tree
                sorted_tree(node, arr);
                printf("\n");

                // Print sorted array
                printf("Sorted elements: ");
                print_array(arr, elements_num);
                puts(" ");
            }

        }
        else if (choice == 3)
        {
            write_txt(file, arr, elements_num);
            printf("Array will be saved in file\n");
            free_tree(node);
            exit(1);
        }
        else
        {
            printf("There's no command like this. Try again!\n");
            continue;
        }

    }
}
