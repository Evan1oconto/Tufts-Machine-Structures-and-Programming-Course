/*

 * code.c
 * Authors:      Duke Alf (ealf01), Evan Loconto (elocon01)
 * Date:         11/08/2020
 * Assignment:   Bomb

 * Implementation of phase 5 and 6 for the bomb assignment.

*/

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
        long value;
        struct node *next;
} node;

static char *TARGET = "ravens"; 
static char *PARSER = "isrveawhobpnutfg";

node node_9 = {
        710,
        NULL
};

node node_8 = {
        539,
        &node_9 
};

node node_7 = {
        353,
        &node_8 
};

node node_6 = {
        871,
        &node_7 
};

node node_5 = {
        435,
        &node_6 
};

node node_4 = {
        239,
        &node_5 
};

node node_3 = {
        196,
        &node_4 
};

node node_2 = {
        594,
        &node_3
};

node node_1 = {
        935,
        &node_2
};


void phase_5 (char *input);
void fun6(struct node *root);
void phase_6(char *input);


/* phase_5
*   Arguments: input string 
*   Return: none
*   Description: reads in a string and maps it to another string,
*                  then checks if this is equal to "ravens"
*   Assumptions: string length is equal to 6
*
*/
void phase_5 (char *input)
{
        int i;
        int index;
        int is_equal;

        /* makes sure input is correct string length */
        if (string_length(input) != 6) {
                explode_bomb();
        }

        /* change each character */
        for (i = 0; i < 6; i++) {
                int k = (input[i] % 16);
                
                input[i] = PARSER[k];
        }

        is_equal = strings_not_equal(input, TARGET);
        
        if (is_equal != 0){
                explode_bomb();
        }

        return;
}

/* fun6
*   Arguments: root node
*   Return: none
*   Description: helper function for phase 6 that attaches a node then 
*                  uses bubble sort to sort a linked list
*   Assumptions: root not null
*
*/
void fun6(node *root)
{
        if (root == NULL) {
                return;
        }

        /* add new node to start of list */
        root->next = &node_1;
        
        node *bubble = root;
        node *curr = bubble->next;
        long placehold;

        /* bubble sort */ 
        while (bubble->next != NULL) {
                while (curr != NULL) {
                        if (curr->value > bubble->value) {
                                placehold = curr->value;
                                curr->value = bubble->value;
                                bubble->value = placehold;
                        }
                        curr = curr->next;
                }
                bubble = bubble->next;
        }

        return;
}

/* phase_6
*   Arguments: input string 
*   Return: none
*   Description: reads in a string, converts it to a long, and then creates a
                 node with the value to be inserted into the linked list. The
                 new node should be placed in as the 5th value in the list, if
                 it is not, then it will return false.
*   Assumptions: an int is being passed in
*
*/
void phase_6(char *input)
{
        long in = strtol(input, NULL, 10);
        
        node node_0;
        node_0.value = in;

        /* call helper to sort list */ 
        fun6(&node_0);

        node *curr = &node_0;

        /* check that inputted node is now the fifth node in linked list */
        int node_check = (curr->next->next->next->next->value);

        if (node_check != in){
                explode_bomb();
        }

        return;
}