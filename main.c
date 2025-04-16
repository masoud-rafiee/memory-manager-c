/**   Assignment_02 |  Masoud Rafiee | Operating Systems
 *  -->   Managing a contiguous region of memory
 *  which consists of Contiguous memeory allocation:
 *              1. Request for a contiguous block of memory
 *              2. Release of a contiguous block of memory
 *              3. Compact unused holes of memory into one single block
 *              4. Report the regions of free and allocated memory
 */

#include <stdio.h> //i,o for printing stats ,etc.
#include <stdlib.h> //malloc general memory allocation
#include <unistd.h> //sys calls like read
#include <ctype.h> //char handling
#include <string.h> //for string
#include <limits.h> // for int MAX

#define UNUSED "Unused"

struct Node *head; //tracking how much available is the memory
struct Node *block_of_space; //entire memory i am managing
struct Node *temp; //to traverse through the contiguous memory

int last_address_space; // keep track of the highest possible mem addr.

//MEMORY STRUCTURES
// i need a linked list for efficient deletion / insertion for add/remove bloclks
// defining each memory block in the program
struct Node
{
    int available_space;//how many bytes in x block
    int start_address; //the first address in thi block
    int end_address; //end of this block
    struct Node *next;//a pointer to the next mem block in my linked list
    char process_id[100];////which process owns this memory: mark unused if free
};

//FUNCTION PROTOTYPES
void create_node(int,int);//creating left over space
void merge();
void swap_nodes();
int find_process(char[3]);
void first_fit(char[3],int);
void best_fit(char[3], int);
void worst_fit(char[3], int);
void request_memory(char[3],int, char[2]);//PID,size needed,and Strategy
void release_memory(char[3]);//the pid
void compact();
void status_report();
void prepare_to_exit();

////starting the functions from here
/**
* Function create_node creates unused node if there is leftover space after adding requested node.
*
* @param space_requested Requested space for node with process.
* @param left_over_space Leftover space for creating new unused node.
*/
void create_node(int space_requested, int left_over_space)
{
 struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
strcpy(newNode->process_id, UNUSED);
    newNode->available_space = left_over_space;
    temp->next->available_space = space_requested;

    newNode->start_address = temp->next->end_address + 1;
    newNode->end_address = newNode->start_address + left_over_space;
    //check if end_address is bigger than inital space
    if(newNode->end_address > last_address_space)
    {
        newNode->end_address = last_address_space;
    }
    newNode->next = temp->next->next;
    temp->next->next = newNode;

    //edit addresses of nodes after created node
    //check if created node is not initial block of memory
    if(temp->next->next->next != NULL)
    {
        //check if node after created node have same start address as end address of created node
        if(temp->next->next->end_address == temp->next->next->next->start_address)
        {
            //point temp to created node
            temp = temp->next->next;
            //change all addresses until reach initial block of memory
            while(temp->next->next != NULL)
            {
                temp->next->start_address = temp->end_address + 1;
                temp->next->end_address = temp->next->start_address + temp->next->available_space;
                temp = temp->next;
            }
            //change start address of initial block of memory
            temp->next->start_address = temp->end_address + 1;
        }
    }
}

/**
* Function merge merges released node with adjacent node if adjacent node is unused.
* <BR>
*/
void merge()
{
    struct Node *node_to_remove;
    //check if only previous node for released node is unused
    if(strcmp(temp->process_id, UNUSED) == 0)
    {
        //merge released node and previous node
        temp->end_address = temp->next->end_address;
        temp->available_space += temp->next->available_space;
        //remove released node
        node_to_remove = temp->next;
        temp->next = temp->next->next;
        free(node_to_remove);
    }
    //check if next node exists
    else if(temp->next->next != NULL)
    {
        //check if only next node for released node is unused
        if(strcmp(temp->next->next->process_id, UNUSED) == 0)
        {
            //merge released node and next node
            temp->next->next->start_address = temp->next->start_address;
            temp->next->next->available_space += temp->next->available_space;
            //remove released node
            node_to_remove = temp->next;
            temp->next = temp->next->next;
            free(node_to_remove);
        }
    }

    //check if adjacent nodes for released node are unused
    //this if entered only if released and previous nodes already merged
    if(strcmp(temp->process_id, UNUSED) == 0 && strcmp(temp->next->process_id, UNUSED) == 0)
    {
        //merge next node and node merged before
        temp->end_address = temp->next->end_address;
        temp->available_space += temp->next->available_space;
        //remove next node
        node_to_remove = temp->next;
        temp->next = temp->next->next;
        free(node_to_remove);
    }
}

/**
* Function swap_nodes swaps unused node (after temp node) with process after the unused node.
* <BR>
*/
void swap_nodes()
{
    struct Node *newNode;

    //change addresses of process node
    temp->next->next->start_address = temp->next->start_address;
    temp->next->next->end_address = temp->next->next->start_address + temp->next->next->available_space;

    //change addresses of unused node
    temp->next->start_address = temp->next->next->end_address + 1;
    temp->next->end_address = temp->next->start_address + temp->next->available_space;

    //change pointers
    newNode = temp->next->next;
    temp->next->next = temp->next->next->next;
    newNode->next = temp->next;
    temp->next = newNode;
}

/**
 * Function find_process finds if there is process with passed id in memory.
 * @param process_id ID of the process to check in memory
 * @return Returns <code>0</code> if process id is not found in memory, and returns <code>1</code> otherwise.
 */
int find_process(char process_id[3])
{
    int found = 0;
    temp = head;

    while(temp->next != NULL)
    {
        if(strcmp(temp->next->process_id, process_id) == 0)
        {
            found = 1;
            break;
        }

        temp = temp->next;
    }

    return found;
}

/**
* Function first_fit allocates the first hole that is big enough.
* <BR>
* @param process_id ID of the process to insert in memory.
* @param space_requested Requested space for process to insert.
*/
void first_fit(char process_id[3], int space_requested)
{
    temp = head;
    int left_over_space = 0;

    while(temp->next != NULL)
    {
        //check if the block is being unused and big enough place the process in that memory space
        if(strcmp(temp->next->process_id, UNUSED) == 0 && temp->next->available_space >= space_requested)
        {
            //calculating avalible space left over
            head->available_space = head->available_space - space_requested;

            //writing over the unused node
            strcpy(temp->next->process_id, process_id);
            temp->next->end_address = temp->next->start_address + space_requested;

            //make new node with leftover space, if left_over_space > 0
            left_over_space = temp->next->available_space - space_requested;
            if(left_over_space > 0)
            {
                create_node(space_requested, left_over_space);
            }

            return; //halt the function, when space is found and inserted
        }
        else
        {
            temp = temp->next;
        }
    }

    //if temp pointer reached the end of the LL without inserting requested process
    printf("There is no space to place process %s, of %d bytes\n", process_id, space_requested);
}

/**
* Function best_fit allocates the smallest hole that is big enough.
* <BR>
* @param process_id ID of the process to insert in memory.
* @param space_requested Requested space for process to insert.
*/
void best_fit(char process_id[3], int space_requested)
{
    int smallest_space = INT_MAX;
    int left_over_space = 0;
    temp = head;

    //find the smallest possible avaliable space
    while(temp->next != NULL)
    {
        //if space is unused and big enough to fit, track min space
        if(strcmp(temp->next->process_id, UNUSED) == 0 && temp->next->available_space >= space_requested)
        {
            //track min space
            if(temp->next->available_space <= smallest_space)
            {
                smallest_space = temp->next->available_space;
            }
        }

        temp = temp->next;
    }

    temp = head;
    //place requested process in smallest possible available space found
    while(temp->next != NULL)
    {
        //check if node is unused and it have smallest possible available space found
        if(strcmp(temp->next->process_id, UNUSED) == 0 && temp->next->available_space == smallest_space)
        {
            //calculating avalible space left over
            head->available_space = head->available_space - space_requested;

            //writing over the space
            strcpy(temp->next->process_id, process_id);
            temp->next->end_address = temp->next->start_address + space_requested;

            //make new node with leftover space, if left_over_space > 0
            left_over_space = temp->next->available_space - space_requested;
            if(left_over_space > 0)
            {
                create_node(space_requested, left_over_space);
            }

            return; //halt the function, when space is found and inserted
        }
        else
        {
            temp = temp->next;
        }
    }

    //if temp pointer reached the end of the LL without inserting requested process
    printf("There is no space to place process %s, of %d bytes\n", process_id, space_requested);
}

/**
* Function worst_fit allocates the largest hole that is big enough.
* <BR>
* @param process_id ID of the process to insert in memory.
* @param space_requested Requested space for process to insert.
*/
void worst_fit(char process_id[3], int space_requested)
{
    int largest_space = INT_MIN;
    int left_over_space = 0;
    temp = head;

    //find the largest possible avaliable space
    while(temp->next != NULL)
    {
        //if space is unused and big enough to fit, track max space
        if(strcmp(temp->next->process_id, UNUSED) == 0 && temp->next->available_space >= space_requested)
        {
            //track max space
            if(temp->next->available_space >= largest_space)
            {
                largest_space = temp->next->available_space;
            }
        }

        temp = temp->next;
    }

    temp = head;
    //place requested process in largest possible available space found
    while(temp->next != NULL)
    {
        //check if node is unused and it have largest possible available space found
        if(strcmp(temp->next->process_id, UNUSED) == 0 && temp->next->available_space == largest_space)
        {
            //calculating avalible space left over
            head->available_space = head->available_space - space_requested;

            //writing over the space
            strcpy(temp->next->process_id, process_id);
            temp->next->end_address = temp->next->start_address + space_requested;

            //make new node with leftover space, if left_over_space > 0
            left_over_space = temp->next->available_space - space_requested;
            if(left_over_space > 0)
            {
                create_node(space_requested, left_over_space);
            }

            return; //halt the function, when space is found and inserted
        }
        else
        {
            temp = temp->next;
        }
    }

    //if temp pointer reached the end of the LL without inserting requested process
    printf("There is no space to place process %s, of %d bytes\n", process_id, space_requested);
}

/**
* Function request_memory decides which algorithm to use based on request.
* <BR>
* @param process_id ID of the process to request.
* @param space_requested Requested space for requested process.
* @param algo Algorithm type to use.
*/
void request_memory(char process_id[3], int space_requested, char algo[2])
{
    if(find_process(process_id) == 1){
        printf("Process %s alredy in memory. Try again\n", process_id);
    }
    else if(strcmp("W", algo) == 0)
    {
        worst_fit(process_id, space_requested);
    }
    else if(strcmp("B", algo) == 0)
    {
        best_fit(process_id, space_requested);
    }
    else if(strcmp("F", algo) == 0)
    {
        first_fit(process_id, space_requested);
    }
    else
    {
        printf("Choose between best_fit, worst_fit, and first_fit. Try again\n");
        return;
    }

}

/**
* Function release_memory releases requested process and merges nodes if there is ajecent unused nodes.
* <BR>
* @param process_id ID of the process to release.
*/
void release_memory(char process_id[3])
{
    //if the node have needed process_id release the process
    if(find_process(process_id) == 1)
    {
        //calculating avalible space left over
        head->available_space = head->available_space + temp->next->available_space;

        //releasing the process node
        strcpy(temp->next->process_id, UNUSED);

        //merge process and adjacent nodes if there is Unused adjacent node
        merge();

        return; //halt the function, when process is found and released
    }

    //if temp pointer reached the end of the LL and did not find the process requested to release
    printf("There is no process %s in memory\n", process_id);
}

/**
* Function compact moves unused nodes to bigger addresses and merges unused nodes together step by step.
* <BR>
*/
void compact()
{
    temp = head;

    while(temp->next != NULL)
    {
        //check if first node is unused and it is not an initial block of memory
        //or if next node is process and current node is unused
        if((strcmp(temp->next->process_id, UNUSED) == 0 && temp->next->next != NULL) ||
           (strcmp(temp->next->process_id, UNUSED) != 0 && strcmp(temp->process_id, UNUSED) == 0))
        {
            //swap unused node with process
            swap_nodes();

            //change temp to swapped unused node
            temp = temp->next;

            //merge nodes if after swaping next node is also unused
            merge();
        }
        else
        {
            temp = temp->next;
        }
    }
}

/**
* Function status_report prints regions of memory.
* <BR>
*/
void status_report()
{
    temp = head;

    printf("avaliable space left: %d\n", head->available_space);
    while(temp->next != NULL)
    {
        printf("Addresses [%d : %d] Process %s\n", temp->next->start_address, temp->next->end_address, temp->next->process_id);
        temp = temp->next;
    }
}

/**
* Function prepare_to_exit releases all memory before exiting the program.
* <BR>
*/
void prepare_to_exit()
{
    temp = head;
    struct Node *node_to_remove;

    while(temp->next != NULL)
    {
        node_to_remove = temp->next;
        temp->next = temp->next->next;
        free(node_to_remove);
    }
    free(head);
}

/**
* Function main is the starting point.
* <BR>
* @param argc Number of arguments.
* @param argv Pointer to array of arguments.
* @returns Returns <code>0</code> if program finished work successfully.
*/
int main(int argc, char *argv[])
{
    char request_type[5];
    char process[3];
    int space_requested;
    char algo_type[2];

    int inital_memory = atoi(argv[1]) - 1;

    char user_input[128];


    //read the user input
    printf("allocator>");
    fflush(stdout);

    if(read(0, user_input, 128) < 0)
    {
        write(2, "An error occurred in the read.\n", 31);
    }

    //the head node is a dummy node
    head = (struct Node *)malloc(sizeof(struct Node));
    //block_of_space is the amount of space in memory
    block_of_space = (struct Node *)malloc(sizeof(struct Node));

    strcpy(head->process_id, "Dummy Node");
    head->start_address = -1;
    head->end_address = -1;
    head->available_space = inital_memory;
    head->next = block_of_space;

    strcpy(block_of_space->process_id, UNUSED);
    block_of_space->start_address = 0;
    block_of_space->end_address = inital_memory;
    block_of_space->available_space = block_of_space->end_address - block_of_space->start_address;
    block_of_space->next = NULL;

    last_address_space = inital_memory;

    while(user_input[0] != 'X')
    {
        if((sscanf(user_input, "%s", request_type)) < 0)
        {
             write(2, "An error occurred in the read.\n", 31);
        }

        //exit the program
        if(strcmp("X", request_type) == 0)
        {
            break;
        }

        //request new process
        if(strcmp("RQ", request_type) == 0)
        {
            sscanf(user_input, "%s %s %d %s", request_type, process, &space_requested, algo_type);
            request_memory(process, space_requested, algo_type);
        }
        //release process in memeory
        else if(strcmp("RL", request_type) == 0)
        {
            sscanf(user_input, "%s %s", request_type, process);
            release_memory(process);
        }
        //compact unused nodes in one node
        else if(strcmp("C", request_type) == 0)
        {
            compact();
        }
        //show status of memory
        else if(strcmp("STAT", request_type) == 0)
        {
            status_report();
        }
        //invalid command entered
        else
        {
            printf("This command is not recognized, try again\n");
        }

        printf("allocator>");
        fflush(stdout);

        if(read(0, user_input, 128) < 0)
        {
            write(2, "An error occurred in the read.\n", 31);
        }
    }

    //release memory before exiting program
    prepare_to_exit();

    return 0;
}


