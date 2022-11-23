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
void encodeTree(TreeNode *root, char codes[128], int codeptr);
int length(char *c);
void decode_original_str(TreeNode *root, char *encoded_string, char *decoded_string, int i);

char final_encoded_map[128][128];

int main()
{
    int freq[128];
    char ip_string[100000] = "currently taiking hardcoded chars. Later we will take users's file from i/p, read it and then make corresponding freq char pair tuples.";

    for (size_t i = 0; i < 128; i++)
    {
        freq[i] = 0;
    }

    for (size_t i = 0; i < strlen(ip_string); i++)
    {
        if (ip_string[i] != '\0')
            freq[(int)ip_string[i]]++;
    }

    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    for (size_t i = 0; i < 128; i++)
    {
        Tuple t;
        if (freq[i] != 0)
        {
            t.c = (char)(i);
            t.freq = freq[i];
            enqueue(pq, t);
        }
    }
    //! currently taiking hardcoded chars. Later we will take users's file from i/p, read it and then make corresponding freq char pair tuples.

    // while(! isEmpty(pq))
    // {
    //     TreeNode t = dequeue(pq);
    //     printf("%c -- %d\n", t.data.c, t.data.freq);
    // }

    printf("Queue over\n");

    printf("EncodeText \n");
    TreeNode *root = encodeText(pq);
    printf("tree made successfully");

    char codes[128];
    encodeTree(root, codes, 0);
    printf("\n\nCodes are - ");
    printf("\nChar\tcode\tsize");
    int resultStrsize = 0;
    int originalStrsize = 0;
    for (size_t i = 0; i < 128; i++)
    {
        if (freq[i] != 0)
        {
            char *code = final_encoded_map[i];
            int l = length(code);
            resultStrsize += l * freq[i];
            originalStrsize += 8 * freq[i];
            printf("\n%c\t%s\t%d", (char)i, code, l);
        }
    }
    double percentage = (double)(originalStrsize - resultStrsize) / (double)originalStrsize * 100;
    printf("\nOriginal size = %d\nCompressed Size = %d\nPercentage = %f", originalStrsize, resultStrsize, percentage);
    char encoded_string[strlen(ip_string) * 64];
    for (size_t i = 0; i < strlen(ip_string); i++)
    {
        if (ip_string[i] != '\0')
        {
            strcat(encoded_string, final_encoded_map[(int)ip_string[i]]);
        }
    }

    printf("\n\nOriginal String - %s", ip_string);
    printf("\nEncoder encoding the original string...");
    printf("\nEncoded String - %s\n", encoded_string);

    printf("\nDecoder decoding the original string...");
    char decoded_string[1000000];
    decode_original_str(root, encoded_string, decoded_string, 0);
    printf("\nDecoded String - %s\n", decoded_string);
    
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

void encodeTree(TreeNode *root, char codes[128], int codeptr)
{
    if (!root)
        return;
    if (!root->left && !root->right)
    {
        int i = 0, j = 0;
        // final_encoded_map[(int)root->data.c] = codes;
        while (i < codeptr)
        {
            final_encoded_map[(int)root->data.c][j++] = codes[i++];
        }

        // printf("\n%c - %s", root->data.c, codes);
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

void decode_original_str(TreeNode *root, char *encoded_string, char *decoded_string, int i)
{
    TreeNode *currNode = root;
    int j = 0;
    while (i < strlen(encoded_string))
    {
        if (encoded_string[i] == '0')
        {
            currNode = currNode->left;
        }
        else
        {
            currNode = currNode->right;
        }
        if (!currNode->left && !currNode->right)
        {
            decoded_string[j++] = currNode->data.c;
            currNode = root;
        }
        i++;
    }
}
