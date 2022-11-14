#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

typedef struct Tuple
{
    char c;
    int freq;
} Tuple;

typedef struct TreeNode
{
    Tuple data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct PriorityQueue
{
    TreeNode heap[128]; // Min heap
    int size;
} PriorityQueue;

void enqueue(PriorityQueue *p, Tuple e);
void enqueueTreeNode(PriorityQueue *p, TreeNode e);
TreeNode dequeue(PriorityQueue *p);
bool isEmpty(PriorityQueue *p);
TreeNode *encodeText(PriorityQueue *pq);
void inOrder(TreeNode *root);
void encodeTree(TreeNode *root, char codes[256], int i);
int length(char *c);

char final_encoded_map[256][256];

int main()
{
    // TreeNode *root = NULL;
    int freq[] = {5, 9, 12, 13, 16, 45};
    char x = 'a';
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    for (int i = 0; i < 6; i++)
    {
        Tuple t;
        t.c = (char)(x++);
        t.freq = freq[i];
        enqueue(pq, t);
    }
    // while(! isEmpty(pq))
    // {
    //     TreeNode t = dequeue(pq);
    //     printf("%c -- %d\n", t.data.c, t.data.freq);
    // }

    printf("Queue over\n");

    printf("EncodeText \n");
    TreeNode *root = encodeText(pq);
    printf("tree made successfully");

    char codes[256];
    encodeTree(root, codes, 0);
    char y = 'a';
    printf("\n\nCodes are - ");
    int resultStrsize = 0;
    int originalStrsize = 0;
    for (int i = 0; i < 6; i++)
    {
        char *code = final_encoded_map[(char)y];
        int l = length(code);
        resultStrsize += l * freq[i];
        originalStrsize += 8 * freq[i];
        printf("\n%c - %s - %d", (char)y, code, l);
        y++;
    }
    double percentage = (double)(originalStrsize - resultStrsize) / (double)originalStrsize * 100;
    printf("\nOriginal size = %d\nCompressed Size = %d\nPercentage = %f", originalStrsize, resultStrsize, percentage);

    return 0;
}

int length(char *c)
{
    int l = 0;
    while (c[l])
    {
        l++;
    }
    return l;
}

void enqueue(PriorityQueue *p, Tuple e)
{
    // Tuple* heap = p->heap;
    p->heap[p->size].data = e;
    p->size += 1;
    int child = p->size - 1;
    int parent = 0;
    while (child != 0)
    {
        parent = (child - 1) / 2;
        if (p->heap[parent].data.freq >= p->heap[child].data.freq)
        {
            TreeNode temp = p->heap[parent];
            p->heap[parent] = p->heap[child];
            p->heap[child] = temp;
            child = parent;
        }
        else
        {
            break;
        }
    }
}

void enqueueTreeNode(PriorityQueue *p, TreeNode e)
{
    // Tuple* heap = p->heap;
    p->heap[p->size] = e;
    p->size += 1;
    int child = p->size - 1;
    int parent = 0;
    while (child != 0)
    {
        parent = (child - 1) / 2;
        if (p->heap[parent].data.freq >= p->heap[child].data.freq)
        {
            TreeNode temp = p->heap[parent];
            p->heap[parent] = p->heap[child];
            p->heap[child] = temp;
            child = parent;
        }
        else
        {
            break;
        }
    }
}

TreeNode dequeue(PriorityQueue *p)
{

    TreeNode maxItem = p->heap[0];
    p->heap[0] = p->heap[p->size - 1];
    p->heap[p->size - 1].data.c = '\0';
    p->heap[p->size - 1].data.freq = 0;
    p->size -= 1;

    int parent = 0;

    while (parent >= 0)
    {
        int swapped = -1;
        int leftChild = 2 * parent + 1;
        if (leftChild < p->size)
        {
            if (p->heap[parent].data.freq > p->heap[leftChild].data.freq)
            {
                TreeNode temp = p->heap[parent];
                p->heap[parent] = p->heap[leftChild];
                p->heap[leftChild] = temp;
                swapped = leftChild;
            }
        }
        int rightChild = 2 * parent + 2;
        if (rightChild < p->size)
        {
            if (p->heap[parent].data.freq > p->heap[rightChild].data.freq)
            {
                TreeNode temp = p->heap[parent];
                p->heap[parent] = p->heap[rightChild];
                p->heap[rightChild] = temp;
                swapped = rightChild;
            }
        }
        if (swapped == -1)
        {
            break;
        }
        parent = swapped;
    }

    return maxItem;
}

bool isEmpty(PriorityQueue *p)
{
    return p->size == 0;
}

TreeNode *encodeText(PriorityQueue *pq)
{
    TreeNode *root = NULL;
    while (!isEmpty(pq))
    {
        TreeNode min = dequeue(pq);
        if (isEmpty(pq))
            return root;

        TreeNode min2 = dequeue(pq);
        int tempSum = min.data.freq + min2.data.freq;

        TreeNode *tempNode = (TreeNode *)malloc(sizeof(TreeNode));
        Tuple tempData;
        tempData.c = '\0';
        tempData.freq = tempSum;
        tempNode->data = tempData;

        TreeNode *rightTempNode = (TreeNode *)malloc(sizeof(TreeNode));
        rightTempNode->data = min2.data;
        rightTempNode->left = min2.left;
        rightTempNode->right = min2.right;

        TreeNode *leftTempNode = (TreeNode *)malloc(sizeof(TreeNode));
        leftTempNode->data = min.data;
        leftTempNode->left = min.left;
        leftTempNode->right = min.right;

        // if (rightTempNode->data.c == '\0' && leftTempNode->data.c != '\0')
        // if (1)
        // {
        //     tempNode->left = rightTempNode;
        //     tempNode->right = leftTempNode;
        // }
        tempNode->right = rightTempNode;
        tempNode->left = leftTempNode;
        TreeNode temp;
        temp.data = tempNode->data;
        temp.left = tempNode->left;
        temp.right = tempNode->right;

        root = tempNode;
        enqueueTreeNode(pq, temp);
    }

    return root;
}

void encodeTree(TreeNode *root, char codes[], int codeptr)
{
    if (!root)
        return;
    if (!root->left && !root->right)
    {
        int i = 0, j = 0;
        // final_encoded_map[(int)root->data.c] = codes;
        while (codes[i])
        {
            final_encoded_map[(int)root->data.c][j++] = codes[i++];
        }

        printf("\n%c - %s", root->data.c, codes);
        return;
    }
    if (root->left)
    {
        codes[codeptr] = '0';
        encodeTree(root->left, codes, codeptr + 1);
    }
    if (root->right)
    {
        codes[codeptr] = '1';
        encodeTree(root->right, codes, codeptr + 1);
    }
}

void inOrder(TreeNode *root)
{
    if (!root)
        return;
    inOrder(root->left);
    printf("\n[%c, %d]", root->data.c, root->data.freq);
    inOrder(root->right);
}