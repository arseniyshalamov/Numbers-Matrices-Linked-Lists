/*
    Assigned by:
    Arsenii Shalamov #342822319
*/

/* I wrote this code on a MacBook, but tried to make it suitable for Visual Studio on Windows. */

#define _CRT_SECURE_NO_WARNINGS
 
/* Libraries */

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
//on the macBook used: #include <stdlib.h> insted: #include <malloc.h>

/* Constant definitions */

#define N 3
#define ROWS 4
#define COLS 5

/* Type definitions */

typedef struct number
{
    unsigned long long num;
    int sum;
} Number;

typedef struct triad
{
    int i, j, value;
} Triad;

typedef struct item
{
    Triad data;
    struct item* next;
} Item;

/* Function declarations */

void Ex1();
void Ex2();
void Ex3();

Number* primeSums(unsigned long long n1, unsigned long long n2, int* p_size);
int ** matrixMaxNeighbor( int A[][COLS], int rows, int cols);
void createThreeLists(int** A, int rows, int cols, Item** pL1, Item** pL2);

/* Declarations of auxiliary functions */

int isPrime(int num);
int digitSum(unsigned long long num);

int neighborMax(int A[][COLS], int rows, int cols, int i, int j);
int** allocMatrix(int rows, int cols);

Triad createThree(int i, int j, int value);
void insert(Item** lst, Triad item);
void reverseList(Item** list);

void printArray(Number* arr, int size);
void inputMatrix(int A[][COLS], int rows, int cols);
void printMatrix(int A[][COLS], int rows, int cols);
void printDynamicMatrix(int** A, int rows, int cols);
void freeMatrix(void** A, int rows);
void inputDynamicMatrix(int** A, int rows, int cols);
void printList(Item* lst);
void freeList(Item* lst);

/* ------------------------------- */

int main()
{
    int select = 0, i, all_Ex_in_loop = 0;
    printf("Run menu once or cyclically?\n(Once - enter 0, cyclically - enter other number) ");
    if (scanf("%d", &all_Ex_in_loop) == 1)
        do
        {
            for (i = 1; i <= N; i++)
                printf("Ex%d--->%d\n", i, i);
            printf("EXIT-->0\n");
            do {
                select = 0;
                printf("please select 0-%d : ", N);
                scanf("%d", &select);
            } while ((select < 0) || (select > N));
            switch (select)
            {
            case 1: Ex1(); break;
            case 2: Ex2(); break;
            case 3: Ex3(); break;
            }
        } while (all_Ex_in_loop && select);
        return 0;
}


/* Function  */

/* The function "Ex1" takes two numbers from the user: "num1" as the minimum and "num2" as the maximum. It also checks to ensure that "num2" is greater than "num1". Then, it sends these values to the "primeSums" function, which returns a list of numbers within the range from "num1" to "num2" along with the sum of the digits in each number. Finally, it prints this list using the "printArray" function. */
void Ex1()
{
    unsigned long long num1, num2;
    int p_size, i;
    Number* primeSumsPrint;
    
    printf("Enter first number: ");
    scanf("%llu", &num1);
    
    printf("Enter second number: ");
    scanf("%llu", &num2);
    while (num2 < num1) {
        printf("Second number must be greater then the first: ");
        scanf("%llu", &num2);
    }
    
    primeSumsPrint = primeSums(num1, num2, &p_size);
    
    if (primeSumsPrint != NULL) {
        printf("Found %d numbers:\n", p_size);
        printArray(primeSumsPrint, p_size);
        free(primeSumsPrint);
    } else printf("No prime numbers found in the given range.\n");
}


/* The function "primeSums" creates a dynamic list where it records the sum of the digits in a number if this sum is a prime number, along with the number itself. */
Number* primeSums(unsigned long long n1, unsigned long long n2, int* p_size)
{
    Number* list = NULL;
    int count = 0, sum;
    
    while (n1 <= n2) {
        if (n1 > 9) sum = digitSum(n1);
        else sum = n1;
        
        if (sum % 2 != 0) {
            if (isPrime(sum)) {
                list = (Number*)realloc(list, (count + 1) * sizeof(Number));
                assert(list);
                list[count].num = n1;
                list[count].sum = sum;
                count++;
            }
        }
        
        n1 += 1;
    }
    
    *p_size = count;
    
    return list;
}

/* The function "isPrime" takes the sum of digits from the "digitSum" function and checks if it is a prime number. */
int isPrime(int num) {
    int i;
    
    if (num < 2) return 0;
    else if (num == 2) return 1;
    else if (num % 2 == 0) return 0;
    for (i = 3; i <= sqrt(num); i += 2)
        if (num % i == 0) return 0;
    
    return 1;
}

/* The function "digitSum" takes a number and sums each digit in it. */
int digitSum(unsigned long long num) {
    int sum = 0;
    
    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }
    
    return sum;
}

/* The function "printArray" takes a list and prints it. */
void printArray(Number* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("Number: %llu, Sum of digits: %d\n", arr[i].num, arr[i].sum);
    }
}

/* The function "Ex2" checks if "ROWS" and "COLS" are greater than zero (if not, it raises an error), gets a matrix from the "inputMatrix" function, and prints it using the "printMatrix" function. Then, it sends the obtained matrix to the "matrixMaxNeighbor" function, where a dynamic matrix "B" is created containing the maximum neighboring numbers. Finally, matrix "B" is printed using the "printMatrix" function. */
void Ex2() {
    if (ROWS <= 0) {
        printf("Error, because 'ROWS' is: %d.\n", ROWS);
        printf("Must be greater than 0.\n");
        return;
    } else if (COLS <= 0) {
        printf("Error, because 'COLS' is: %d.\n", COLS);
        printf("Must be greater than 0.\n");
        return;
    }
    
    int A[ROWS][COLS];
    
    inputMatrix(A, ROWS, COLS);
    printf("MATRIX A:\n");
    printMatrix(A, ROWS, COLS);

    if (ROWS > 1 && COLS > 1) {
        int** B = matrixMaxNeighbor(A, ROWS, COLS);
        printf("MATRIX B:\n");
        printDynamicMatrix(B, ROWS, COLS);
        freeMatrix(B, ROWS);
    } else printf("MATRIX B: %d\n", A[0][0]);
}

/* The function "inputMatrix" takes a matrix "A" and inserts numbers into each cell. */
void inputMatrix(int A[][COLS], int rows, int cols) {
    int i, j;
    printf("Enter numbers for matrix: %d * %d\n", rows, cols);
    
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            scanf("%d", &A[i][j]);
        }
    }
    printf("\n");
}

/* The function "printMatrix" takes a matrix and prints it. */
void printMatrix(int A[][COLS], int rows, int cols) {
    int i, j;
    
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* The function "matrixMaxNeighbor" takes a matrix, creates a dynamic matrix "B" using the "allocMatrix" function, and sends it to the "neighborMax" function to insert the maximum neighboring numbers from matrix "A" into matrix "B". */
int** matrixMaxNeighbor(int A[][COLS], int rows, int cols) {
    int i, j;
    int** B = allocMatrix(rows, cols);
    
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            B[i][j] = neighborMax(A, rows, cols, i, j);
        }
    }
    
    return B;
}

/* The function "allocMatrix" creates a dynamic matrix of a specified size. */
int** allocMatrix(int rows, int cols) {
    int i;
    int** B = (int**)malloc(rows * sizeof(int*));
    assert(B);
    
    for (i = 0; i < rows; i++) {
        B[i] = (int*)malloc(cols * sizeof(int));
        assert(B[i]);
    }
    
    return B;
}

/* The function "neighborMax" takes a number from matrix "A" and checks it with its neighboring numbers to determine if it is the maximum number. If a neighboring number is greater, it returns the maximum number; otherwise, it returns the number itself. */
int neighborMax(int A[][COLS], int rows, int cols, int i, int j) {
    int max = 0;
    
    // The top row.
    if (i > 0 && A[i-1][j] > max) max = A[i-1][j];
    // The bottom row.
    if (i < rows-1 && A[i+1][j] > max) max = A[i+1][j];
    // The left column.
    if (j > 0 && A[i][j-1] > max) max = A[i][j-1];
    // The right column.
    if (j < cols-1 && A[i][j+1] > max) max = A[i][j+1];
    
    return max;
}

/* The function "printDynamicMatrix" takes a matrix and prints it. */
void printDynamicMatrix(int** A, int rows, int cols) {
    int i, j;
    
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* The function "freeMatrix" takes a dynamic matrix and frees the memory, essentially deleting the matrix from memory. */
void freeMatrix(void** A, int rows) {
    int i;
    
    for (i = 0; i < rows; i++) {
        free(A[i]);
    }
    free(A);
}

/* The function "Ex3" checks if "ROWS" and "COLS" are greater than zero (otherwise, it raises an error), creates a dynamic matrix of the specified size using the "allocMatrix" function, then sends it to the "inputDynamicMatrix" function to fill it with numbers, and prints the matrix using the "printDynamicMatrix" function. Next, in the line "Item* pL1 = NULL, * pL2 = NULL;", two lists are created, and the "createThreeLists" function populates these lists. In list pL1, all numbers are included if "i + j == A[i][j]", while list pL2 contains all numbers if "i < j < A[i][j]" or vice versa, with the condition that there is the same difference between "i, j, A[i][j]". The "reverseList" function reverses these lists, and the "printList" function prints them. Finally, the "freeList" and "freeMatrix" functions free the memory. */
void Ex3()
{
    if (ROWS <= 0) {
        printf("Error, because 'ROWS' is: %d.\n", ROWS);
        printf("Must be greater than 0.\n");
        return;
    } else if (COLS <= 0) {
        printf("Error, because 'COLS' is: %d.\n", COLS);
        printf("Must be greater than 0.\n");
        return;
    }
    
    int** matrix = allocMatrix(ROWS, COLS);
    
    inputDynamicMatrix(matrix, ROWS, COLS);
    printf("Matrix: \n");
    printDynamicMatrix(matrix, ROWS, COLS);
    
    Item* pL1 = NULL, * pL2 = NULL;
    createThreeLists(matrix, ROWS, COLS, &pL1, &pL2);
    
    reverseList(&pL1);
    printf("List L1 (Condition 1):\n");
    printList(pL1);
    
    reverseList(&pL2);
    printf("List L2 (Condition 2):\n");
    printList(pL2);
    
    freeList(pL1);
    freeList(pL2);
    freeMatrix(matrix, ROWS);
}

/* The function "inputDynamicMatrix" takes a matrix and fills it with values. */
void inputDynamicMatrix(int** A, int rows, int cols) {
    int i, j;
    
    printf("Enter numbers for matrix: %d * %d\n", rows, cols);
    
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            scanf("%d", &A[i][j]);
        }
    }
    printf("\n");
}

/* The function "createThreeLists" takes a matrix and two lists "pL1" and "pL2", and using the function "createThree", it generates a template, then fills the lists through the "insert" function. */
void createThreeLists(int** A, int rows, int cols, Item** pL1, Item** pL2)// 3
{
    int i, j;
    
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (A[i][j] == i + j) {
                insert(pL1, createThree(i, j, A[i][j]));
            }
            if ((A[i][j] - j) == (j - i)) {
                insert(pL2, createThree(i, j, A[i][j]));
            }
        }
    }
}

/* The function "createThree" receives data "i, j, A[i][j]", creates a template, and inserts these data into the template. */
Triad createThree(int i, int j, int value) {
    Triad item;
    item.i = i;
    item.j = j;
    item.value = value;
    return item;
}

/* The function "insert" receives a list and a template, and inserts the template into the list. */
void insert(Item** lst, Triad item) {
    Item* newLst = (Item*)malloc(sizeof(Item));
    assert(newLst);
        
    newLst->data = item;
    newLst->next = *lst;
    *lst = newLst;
}

/* The function "reverseList" takes a sequence of cells and reverses the order of reading. */
void reverseList(Item** list) {
    Item* curr = *list;
    Item* prev = NULL;
    Item* next = NULL;

    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    *list = prev;
}

/* The function "printList" takes a list and prints it. */
void printList(Item* lst) {
    while (lst) {
        printf("(%d, %d, %d) -> ", lst->data.i, lst->data.j, lst->data.value);
        lst = lst->next;
    }
    printf("NULL\n");
}

/* The function "freeList" takes a list and frees the memory, essentially deleting the list. */
void freeList(Item* lst) {
    Item* next;
    
    while (lst) {
        next = lst->next;
        free(lst);
        lst = next;
    }
}

/* Added function: */
//void reverseList(Item** list);

/* ------------------- */
