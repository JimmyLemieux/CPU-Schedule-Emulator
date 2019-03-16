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

int cmpFunc(const void *a, const void * b) {
    struct Node * const *nA = a;
    struct Node * const *nB = b;
    return( (*nA)->process_id - (*nB)->process_id);
}


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
        return;
    }

    
    while((*head_ref)->next != NULL) {
        (*head_ref) = (*head_ref)->next;
    }
    node->next = (*head_ref)->next;
    (*head_ref)->next = node;
}


/* This is to remove a node */
struct Node * find_node_remove(struct Node **head_ref, int ID) {
    int length = 0;

    struct Node *temp = *head_ref, *prev;

    if(temp != NULL && temp->process_id == ID) {
        *head_ref = temp->next;
        return temp;
    }

    while(temp != NULL && temp->process_id != ID) {
        prev = temp;
        temp = temp->next;
        length++;
    }

    if(temp == NULL) {
        return NULL;
    }

    prev->next = temp->next;
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

    struct Queue *q = createQueue(10000);

    struct Node **output = (struct Node **)calloc(1, sizeof(struct Node *));
    int outIndex = 0;
    //struct Node no_node = createNode(0);

    struct Node no_node = createNode(0);

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
                    no_node.runningTime += abs(timeStamp - no_node.referencePointRunning);
                    no_node.referencePointRunning = timeStamp;



                    n.status = RUNNING;  
                    n.referencePointRunning = timeStamp;
                } else {
                    n.status = READY;
                    n.referencePointReady = timeStamp;
                    q->nodeArray[q->front].runningTime += abs(timeStamp - q->nodeArray[q->front].referencePointRunning); //Update the current running nodes time
                    q->nodeArray[q->front].referencePointRunning = timeStamp;
                }

                for(int i = q->front;i<q->size;i++) { //Update all of the ready nodes times
                    if(q->nodeArray[i].status == RUNNING) continue;
                    q->nodeArray[i].readyTime += abs(timeStamp - q->nodeArray[i].referencePointReady);
                    q->nodeArray[i].referencePointReady = timeStamp;
                }
                pushQueue(q, n);
                break;
            case 'R':
                //On Each of these calculate the updated time
                resource = atoi(input_line_tokens[tokenSize - 2]);
                process_id = atoi(input_line_tokens[tokenSize - 1]);
                timeStamp = atoi(input_line_tokens[0]); // The timestamp that we will be adding

                n = popQueue(q);
                //Get the previous timestamp
                n.next = NULL;
                n.status = BLOCK;
                n.referencePointBlock = timeStamp;

                if(isEmpty(q)) {
                    no_node.status = RUNNING;
                    no_node.referencePointRunning = timeStamp;
                }


                n.runningTime += abs(timeStamp - n.referencePointRunning);
                // n.referencePointRunning = timeStamp;

                //Update all of the ready an running times
                if(!isEmpty(q)) {
                    q->nodeArray[q->front].status = RUNNING;
                    q->nodeArray[q->front].referencePointRunning = timeStamp;
                    q->nodeArray[q->front].readyTime += abs(timeStamp - q->nodeArray[q->front].referencePointReady);
                }      


                for(int i = q->front;i<q->size;i++) {
                    if(q->nodeArray[i].status == RUNNING) continue;
                    q->nodeArray[i].readyTime += abs(timeStamp - q->nodeArray[i].referencePointReady);
                    q->nodeArray[i].referencePointReady = timeStamp;
                }
          

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
                    push_linked_list(&head_ref1, new_node);
                }
                if(resource == 2) {
                    push_linked_list(&head_ref2, new_node);
                }
                if(resource == 3) {
                    push_linked_list(&head_ref3, new_node);
                }
                if(resource == 4) {
                    push_linked_list(&head_ref4, new_node);
                }
                if(resource == 5) {
                    push_linked_list(&head_ref5, new_node);
                }
                break;
            case 'I':
                //calculate the updated times
                timeStamp = atoi(input_line_tokens[0]);
                resource = atoi(input_line_tokens[tokenSize - 2]);
                process_id = atoi(input_line_tokens[tokenSize - 1]);
                if(resource == 1) {
                    node_return = find_node_remove(&head_ref1, process_id);
                }
                if(resource == 2) {
                    node_return = find_node_remove(&head_ref2, process_id);
                }
                if(resource == 3) {
                    node_return = find_node_remove(&head_ref3, process_id);
                }
                if(resource == 4) {
                    node_return = find_node_remove(&head_ref4, process_id);
                }
                if(resource == 5) {
                    node_return = find_node_remove(&head_ref5, process_id);
                }
                node_return->next = NULL;

                n = *node_return; 
                //Put this into the queue
                n.blockTime += abs(timeStamp - n.referencePointBlock);

                if(isEmpty(q)) {
                    n.status = RUNNING;  
                    n.referencePointRunning = timeStamp;
                    no_node.runningTime += abs(timeStamp - no_node.referencePointRunning);
                    no_node.referencePointRunning = timeStamp;
                } else  {
                    n.status = READY;
                    n.referencePointReady = timeStamp;   
                    q->nodeArray[q->front].runningTime += abs(timeStamp - q->nodeArray[q->front].referencePointRunning);
                    q->nodeArray[q->front].referencePointRunning = timeStamp;
                    for(int i = q->front;i<q->size;i++) {
                        if(q->nodeArray[i].status == RUNNING) continue;
                        q->nodeArray[i].readyTime += abs(timeStamp - q->nodeArray[i].referencePointReady);
                        q->nodeArray[i].referencePointReady = timeStamp;
                    }
                }
                pushQueue(q, n);
                break;
            case 'T':
                //Calculate the updated times

                timeStamp = atoi(input_line_tokens[0]);
                n = popQueue(q);

                n.runningTime += abs(timeStamp - n.referencePointRunning);

                if(!isEmpty(q)) {
                    q->nodeArray[q->front].readyTime += abs(timeStamp - q->nodeArray[q->front].referencePointReady);
                    q->nodeArray[q->front].status = RUNNING;
                    q->nodeArray[q->front].referencePointRunning = timeStamp;
                    n.status = READY;
                    n.referencePointReady = timeStamp;
                    no_node.status = RUNNING;
                    no_node.referencePointRunning = timeStamp;
                } else {
                    n.status = RUNNING;
                    n.referencePointRunning = timeStamp;
                }

                for(int i = q->front;i<q->size;i++) {
                    if(q->nodeArray[i].status == RUNNING) continue;
                    q->nodeArray[i].readyTime += abs(timeStamp - q->nodeArray[i].referencePointReady);
                    q->nodeArray[i].referencePointReady = timeStamp;
                }

                pushQueue(q,n);
                //Not worrying about other nodes right now
                break;
            case 'E':
                //Calculated the updated times
                //If not in Q then find it and bring it back to Q and then delete it from the Q

                n = popQueue(q);

                timeStamp = atoi(input_line_tokens[0]);
                n.next = NULL;

                n.runningTime += abs(timeStamp - n.referencePointRunning);
                n.status = DEAD;

                if(isEmpty(q)) {
                    no_node.status = RUNNING;
                    no_node.referencePointRunning = timeStamp;
                }

                if(!isEmpty(q)) { //Set the new Running and timestamp
                    q->nodeArray[q->front].status = RUNNING;
                    q->nodeArray[q->front].referencePointRunning = timeStamp;
                    q->nodeArray[q->front].readyTime += abs(timeStamp - q->nodeArray[q->front].referencePointReady);
                }

                for(int i = q->front;i<q->size;i++) {
                    if(q->nodeArray[i].status == RUNNING) continue;
                    q->nodeArray[i].readyTime += abs(timeStamp - q->nodeArray[i].referencePointReady);
                    q->nodeArray[i].referencePointReady = timeStamp;
                }
                struct Node *outNode = calloc(1, sizeof(struct Node));
                outNode->process_id = n.process_id;
                outNode->runningTime = n.runningTime;
                outNode->readyTime = n.readyTime;
                outNode->blockTime = n.blockTime;
                outNode->referencePointRunning = n.referencePointRunning;
                outNode->referencePointReady = n.referencePointReady;
                outNode->referencePointBlock = n.referencePointBlock; 
                outNode->status = n.status;
                outNode->next = n.next;
                output[outIndex] = outNode;
                output = realloc(output, sizeof(struct Node *) * (outIndex + 1000));
                outIndex++;
                break;
            default:
                printf("Invalid Command!\n");
                return 0;
        }        
        // printQueue(q);
        free_tokens(input_line_tokens, tokenSize);
    }

    struct Node *nNode = calloc(1, sizeof(struct Node));
    nNode->process_id = no_node.process_id;
    nNode->runningTime = no_node.runningTime;
    nNode->readyTime = no_node.readyTime;
    nNode->blockTime = no_node.blockTime;
    nNode->referencePointRunning = no_node.referencePointRunning;
    nNode->referencePointReady = no_node.referencePointReady;
    nNode->referencePointBlock = no_node.referencePointBlock; 
    nNode->status = no_node.status;
    nNode->next = no_node.next;

    output[outIndex] = nNode;
    outIndex++;


    qsort(output, outIndex, sizeof(output), cmpFunc);

    for(int i = 0;i<outIndex;i++) {
        printf("%d %d %d %d\n",output[i]->process_id,output[i]->runningTime,output[i]->readyTime,output[i]->blockTime);
    }

    free(q);
    free_list(head_ref1);
    free_list(head_ref2);
    free_list(head_ref3);
    free_list(head_ref4);
    free_list(head_ref5);
    for(int i = 0;i<outIndex-1;i++) {
        free(output[i]);
    }
    free(output);
    return 0;
}