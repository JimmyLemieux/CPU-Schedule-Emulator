#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define D printf("DEBUG\n");

typedef enum sts {RUNNING, READY, BLOCK, DEAD} STATUS;

struct Node {
    int process_id;
    int runningTime;
    int readyTime;
    int blockTime;
    int referencePointRunning;
    int referencePointReady;
    int referencePointBlock;
    STATUS status;
    struct Node *next;
};

struct Queue{
    int front;
    int rear;
    int size;
    unsigned capacity;
    struct Node *nodeArray; // For the array of nodes that will handle CPU scheduling
};


//This is the head nodes for the processes in the linked list
struct Node *head_ref1 = NULL; 
struct Node *head_ref2 = NULL;
struct Node *head_ref3 = NULL;
struct Node *head_ref4 = NULL;
struct Node *head_ref5 = NULL;


void printStatus(STATUS status) {
    switch(status) {
        case 0:
            printf("STATUS: RUNNING\n");
            break;
        case 1:
            printf("STATUS: READY\n");
            break;
        case 2:
            printf("STATUS: BLOCK\n");
            break;
        case 3:
            printf("STATUS: DEAD\n");
            break;
        default:
            printf("STATUS: DEFAULT\n");
    }
}


void push_linked_list(struct Node **head_ref, struct Node *node) {

    if((*head_ref) == NULL) {
        node->next = (*head_ref);
        (*head_ref) = node;
        printf("Node Pushed onto the Linked List Head!\n");
        return;
    }

    printf("Node Pushed onto the Linked List!\n");
    
    while((*head_ref)->next != NULL) {
        (*head_ref) = (*head_ref)->next;
    }
    node->next = (*head_ref)->next;
    (*head_ref)->next = node;
}


/* This is to remove a node */
struct Node * find_node_remove(struct Node **head_ref, int ID) {

    struct Node *temp = *head_ref, *prev;

    if(temp != NULL && temp->process_id == ID) {
        *head_ref = temp->next;
        return temp;
    }

    while(temp != NULL && temp->process_id != ID) {
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL) {
        printf("Resource was not found!\n");
        return NULL;
    }

    prev->next = temp->next;
    printf("Resource was found!\n");
    return temp;
} 


int getLength(struct Node **head_ref) {
    int count = 0;
    while((*head_ref) != NULL) {
        count++;
        (*head_ref) = (*head_ref)->next;
    }
    return count;
}

void print_list(struct Node **head_ref) {
    if((*head_ref) == NULL) {
        printf("LIST EMPTY\n");
        return;
    }

    while((*head_ref) != NULL) {
        printf("*********************************\n");
        printf("process_id: %d\n", (*head_ref)->process_id);
        printf("Running Time: %d\n", (*head_ref)->runningTime);
        printf("Ready Time: %d\n", (*head_ref)->readyTime);
        printf("Block Time: %d\n", (*head_ref)->blockTime);
        printStatus((*head_ref)->status);
        printf("*********************************\n");
        (*head_ref) = (*head_ref)->next;
    }
    printf("\n");
}


void print_lists() {
    printf("*******PRINTING LINKED LISTS*******\n");
    printf("Printing List 1\n");
    print_list(&head_ref1);

    printf("Printing List 2\n");
    print_list(&head_ref2);

    printf("Printing List 3\n");
    print_list(&head_ref3);

    printf("Printing List 4\n");
    print_list(&head_ref4);

    printf("Printing List 5\n");
    print_list(&head_ref5);


}

void free_list(struct Node *head_ref) {
    while(head_ref != NULL) {
        struct Node *temp = head_ref;
        head_ref = head_ref->next;
        free(temp);
        printf("Node Freed!\n");
    }
}

struct Queue* createQueue(unsigned capacity) {
    struct Queue *queue = calloc(1, sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = 0;
    queue->size = 0;
    queue->rear = capacity - 1;
    queue->nodeArray = calloc(queue->capacity, sizeof(struct Node));
    return queue;
}

struct Node createNode(int process_id) {
    struct Node node;
    node.process_id = process_id;
    node.runningTime = 0;
    node.readyTime = 0;
    node.blockTime = 0;
    node.referencePointBlock = 0;
    node.referencePointReady = 0;
    node.referencePointRunning = 0;
    node.next = NULL;
    return node;
}


int isFull(struct Queue *queue) {
    if(queue == NULL) {
        return 0;
    }
    return queue->size == queue->capacity;
} 

int isEmpty(struct Queue *queue) {
    if(queue == NULL) {
        return 0;
    }
    return queue->size == 0;
}

void pushQueue(struct Queue *queue, struct Node node) {
    if(queue == NULL || isFull(queue)) {
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity; 
    queue->nodeArray[queue->rear] = node;
    queue->size = queue->size + 1;
    printf("Node pushed!\n");
}   

struct Node popQueue(struct Queue *queue) {
    struct Node node = createNode(-1);
    if(isEmpty(queue) || queue == NULL) {
        return node;
    }
    node = queue->nodeArray[queue->front];
    queue->front = (queue->front + 1) %  queue->capacity;
    queue->size = queue->size - 1;
    return node;
}

void printQueue(struct Queue *queue) {
    if(queue->nodeArray == NULL || isEmpty(queue)) {
        return;
    }
    printf("*******PRINTING THE CURRENT READY QUEUE*******\n");
    for(int i = 0;i<queue->size;i++) {
        printf("node id -> %d\n", queue->nodeArray[i].process_id);
        printf("node Running Time -> %d\n", queue->nodeArray[i].runningTime);
        printf("node Ready Time -> %d\n", queue->nodeArray[i].readyTime);
        printf("node Block Time -> %d\n", queue->nodeArray[i].blockTime);
        printStatus(queue->nodeArray[i].status);
    }
    printf("**********************************************\n");
}

void clearQueue(struct Queue *queue) {
    if(queue == NULL || isEmpty(queue)) {
        return;
    }
    for(int i = 0;i<queue->size;i++) {
        memset(&queue->nodeArray[i], 0, sizeof(struct Node));
    }
    queue->front = 0;
    queue->rear = queue->capacity - 1;
    queue->size = 0;
}


void free_tokens(char **tokens, int size) {
    for(int i = 0;i<size;i++) {
        free(tokens[i]);
    }
    free(tokens);
}


int main() {
    //FILE *fp;
    char buffer[1000];

    struct Queue *q = createQueue(1000);

    //struct Node no_node = createNode(0);

    while(fgets(buffer, 1000, stdin) != NULL) {
        buffer[strlen(buffer) - 1] = '\0';
        char *token;
        char **input_line_tokens = (char **)calloc(1, sizeof(char *));
        int tokenSize = 0; 
        token = strtok(buffer, " ");
        while(token != NULL) {
            input_line_tokens[tokenSize] = calloc(1, sizeof(char) * strlen(token) + 10);
            strcpy(input_line_tokens[tokenSize], token);
            input_line_tokens = realloc(input_line_tokens, sizeof(char *) + (tokenSize + 10));
            tokenSize++;
            token = strtok(NULL, " ");
        }

        char command = input_line_tokens[1][0];

        // if(isEmpty(q)) {
        //     //Something for zero
        // } else {
            
        // }

        //We can have a switch statement here to handle the different inputs
        //For each different command you do a different thing
        switch(command) {
            int process_id;
            int resource;
            int timeStamp = 0;
            struct Node n;
            struct Node *node_return;
            //Update the current times here
            case 'C':
                //Calculate the updated times
                //Create a node and then push to the queue
                process_id = atoi(input_line_tokens[tokenSize - 1]);
                timeStamp = atoi(input_line_tokens[0]);
                n = createNode(process_id);
                if(isEmpty(q)) {
                    n.status = RUNNING;  
                    n.referencePointRunning = timeStamp;
                } else {
                    n.status = READY;
                    n.referencePointReady = timeStamp;
                    q->nodeArray[0].runningTime += abs(timeStamp - q->nodeArray[0].referencePointRunning); //Update the current running nodes time
                    for(int i = 0;i<q->size;i++) { //Update all of the ready nodes times
                        if(q->nodeArray[i].status == RUNNING) continue;
                        q->nodeArray[i].readyTime += abs(timeStamp - q->nodeArray[i].referencePointReady);
                    }
                }
                pushQueue(q, n);
                printf("Pushed process %d onto the ready queue\n", process_id);
                break;
            case 'R':
                //On Each of these calculate the updated time
                resource = atoi(input_line_tokens[tokenSize - 2]);
                process_id = atoi(input_line_tokens[tokenSize - 1]);
                n = popQueue(q);
                //Get the previous timestamp

                timeStamp = atoi(input_line_tokens[0]); // The timestamp that we will be adding

                printf("The current referencePoint is %d The current running time is %d\n", n.referencePointRunning, n.runningTime);
                n.runningTime += abs(timeStamp - n.referencePointRunning);


                if(!isEmpty(q)) {
                    q->nodeArray[0].status = RUNNING;
                    q->nodeArray[0].referencePointRunning = timeStamp;
                    q->nodeArray[0].readyTime += abs(timeStamp - q->nodeArray[0].referencePointReady);
                }
                
                //Update all of the ready an running times
                for(int i = 0;i<q->size;i++) {
                    if(q->nodeArray[i].status == RUNNING) continue;
                    q->nodeArray[i].readyTime += abs(timeStamp - q->nodeArray[i].referencePointReady);
                }

                n.status = BLOCK;
                n.referencePointBlock = timeStamp;
                struct Node *new_node = calloc(1, sizeof(struct Node));
                new_node->process_id = n.process_id;
                new_node->runningTime = n.runningTime;
                new_node->readyTime = n.readyTime;
                new_node->blockTime = n.blockTime;
                new_node->referencePointRunning = n.referencePointRunning;
                new_node->referencePointReady = n.referencePointReady;
                new_node->referencePointBlock = n.referencePointBlock; 
                new_node->status = n.status;
                new_node->next = n.next;



                if(resource == 1) {
                    printf("Pushing onto linked list 1\n");
                    push_linked_list(&head_ref1, new_node);
                }
                if(resource == 2) {
                    printf("Pushing onto linked list 2\n");
                    push_linked_list(&head_ref2, new_node);
                }
                if(resource == 3) {
                    printf("Pushing onto linked list 3\n");
                    push_linked_list(&head_ref3, new_node);
                }
                if(resource == 4) {
                    printf("Pushing onto linked list 4\n");
                    push_linked_list(&head_ref4, new_node);
                }
                if(resource == 5) {
                    printf("Pushing onto linked list 5\n");
                    push_linked_list(&head_ref5, new_node);
                }
                printf("Pushing process %d onto Resource %d\n", process_id,resource);
                break;
            case 'I':
                //calculate the updated times
                printf("Remove a resource and then push back to the queue\n");
                timeStamp = atoi(input_line_tokens[0]);
                resource = atoi(input_line_tokens[tokenSize - 2]);
                process_id = atoi(input_line_tokens[tokenSize - 1]);
                if(resource == 1) {
                    printf("Requesting %d from to resource 1\n", process_id);
                    node_return = find_node_remove(&head_ref1, process_id);
                }
                if(resource == 2) {
                    printf("Requesting %d from to resource 2\n", process_id);
                    node_return = find_node_remove(&head_ref2, process_id);
                }
                if(resource == 3) {
                    printf("Requesting %d from to resource 3\n", process_id);
                    node_return = find_node_remove(&head_ref3, process_id);
                }
                if(resource == 4) {
                    printf("Requesting %d from to resource 4\n", process_id);
                    node_return = find_node_remove(&head_ref4, process_id);
                }
                if(resource == 5) {
                    printf("Requesting %d from resource 5\n", process_id);
                    node_return = find_node_remove(&head_ref5, process_id);
                }

                if(node_return == NULL) {
                    printf("NULL VAL\n");
                }
                n = *node_return; 
                //Put this into the queue
                n.blockTime += abs(timeStamp - n.referencePointBlock);
                if(isEmpty(q)) {
                    n.status = RUNNING;  
                    n.referencePointRunning = timeStamp;
                } else  {
                    n.status = READY;
                    n.referencePointReady = timeStamp;   
                    q->nodeArray[0].runningTime = abs(timeStamp - q->nodeArray[0].referencePointRunning);
                    for(int i = 0;i<q->size;i++) {
                        if(q->nodeArray[i].status == RUNNING) continue;
                        q->nodeArray[i].readyTime += abs(timeStamp - q->nodeArray[i].referencePointReady);
                    }
                }
                      
                pushQueue(q, n);
                break;
            case 'T':
                //Calculate the updated times
                printf("Timer interrput\n");
                n = popQueue(q);
                n.status = READY;
                n.referencePointReady = timeStamp;
                n.runningTime += abs(timeStamp - n.referencePointRunning);

                 if(!isEmpty(q)) {
                    //  n.status = RUNNING;
                    //  n.referencePointRunning = timeStamp;
                     q->nodeArray[0].status = RUNNING;
                     q->nodeArray[0].referencePointRunning = timeStamp;
                 }

                //Update the current ready times
                for(int i = 0;i<q->size;i++) {
                    if(q->nodeArray[i].status == RUNNING) continue;
                    q->nodeArray[i].readyTime += abs(timeStamp - q->nodeArray[i].referencePointReady);
                }
                pushQueue(q,n);
                //Not worrying about other nodes right now
                break;
            case 'E':
                //Calculated the updated times
                //If not in Q then find it and bring it back to Q and then delete it from the Q
                n = popQueue(q);
                timeStamp = atoi(input_line_tokens[0]);
                n.runningTime += abs(timeStamp - n.referencePointRunning);
                //n.runningTime += abs(timeStamp - n.referencePointRunning);
                n.status = DEAD;

                if(!isEmpty(q)) { //Set the new Running and timestamp
                    q->nodeArray[0].status = RUNNING;
                    q->nodeArray[0].referencePointRunning = timeStamp;
                }

                for(int i = 0;i<q->size;i++) {
                    printf("called!\n");
                    if(q->nodeArray[i].status == RUNNING) continue;
                    q->nodeArray[i].readyTime += abs(timeStamp - q->nodeArray[i].referencePointReady);
                }


                printf("******************************\n");
                printf("PRINTING OUT THE FINAL NODE!\n");
                printf("RUNNING TIME: %d\n", n.runningTime);
                printf("READY TIME: %d\n", n.readyTime);
                printf("BLOCK TIME: %d\n", n.blockTime);
                printStatus(n.status);
                printf("******************************\n");
                break;
            default:
                printf("Invalid Command!\n");
                return 0;
        }        
        // printQueue(q);
        print_lists();
        free_tokens(input_line_tokens, tokenSize);

    }

    return 0;
}