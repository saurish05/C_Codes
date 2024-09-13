#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>

//this add colors to output i.e snake, tail, fruit, and board as well as to intro to 
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

// Height and width are the size of the box.
// x, y are the coordinates of the head of the snake.
// fruitx, fruity are the coordinates of the fruit.
// flag is used to set the directions.
int height = 20, width = 20, x, y, fruitx, fruity, flag;
int gameend, score;

int tailx[100], taily[100];
int piece = 0 , questionCount = 0;

//Questions type and maximum question should be asked
int mathQuestion =0, relationalQuestion = 0, maxQuestion= 5;

//this function handles the answer and length as well.
// user enter correct answer then length double but for incorrect answer length -10
void handleanswer(int useranswer, int num1, int num2, int operation)
{
    int correctanswer;

    switch (operation)
    {
        case 0:
        {
            correctanswer = num1 + num2;
            break;
        }
        
        case 1:
        {
            correctanswer = num1 - num2;
            break;
        }

        case 2:
        {
            correctanswer = num1 * num2;
            break;
        }

        case 3:
        {
            correctanswer = num1 / num2;
            break;
        }
    }

    // checks that values that has been entered by the user is correct 
    //if correct then score increases by 10 and length of snake doubles
    if (useranswer == correctanswer)
    {
        score += 10;
    }

    else
    {
        piece -= 10; // Decrease the snake's length by 10
        
        if (piece < 0)
        {
            piece = 0;
            score -= 5;
        }
    }
}

//this function generates the question through which user can have bonus points the score
void generaterelationalquestion()
{
    int num1 = rand() % 2;
    int num2 = rand() % 2;

    int operation = rand() % 6; // 6 represents the total number of operations (4 math + 2 relational)

    if (operation == 0)
    {
        printf("What is the result of %d + %d = ", num1, num2);
    }

    else if (operation == 1)
    {
        printf("What is the result of %d - %d = ", num1, num2);
    }

    else if (operation == 2)
    {
        printf("What is the result of %d * %d = ", num1, num2);
    }

    else if (operation == 3)
    {
        if (num2 != 0) // Avoid division by zero
            printf("What is the result of %d / %d = ", num1, num2);
        else
            generaterelationalquestion(); // Retry if division by zero
    }

    else if (operation == 4)
    {
        printf("Is %d equal to %d? (1 for true, 0 for false) = ", num1, num2);
    }

    else if (operation == 5)
    {
        printf("Is %d not equal to %d? (1 for true, 0 for false) = ", num1, num2);
    }

    int answer;
    scanf("%d", &answer);

    if (operation >= 4)
    {
        // Relational operator questions
        if ((operation == 4 && answer == (num1 == num2)) || (operation == 5 && answer == (num1 != num2)))
        {
            piece *= 2;
            score += 10;
        }

        else
        {
            piece -= 10;
            if (piece < 0)
            {
                piece = 0;
                score -= 5;
            }
        }
    }

    else
    {
        // Mathematical questions
        handleanswer(answer, num1, num2, operation);
    }

    questionCount++;
}

void generatequestion()
{
    if (questionCount < 5)
    {
        int choice = rand() % 2; // 0 for mathematical, 1 for relational
        if (choice == 0)
        {
            generatequestion();
        }
        else
        {
            generaterelationalquestion();
        }
    }
}


// This function controls the main logic for the program includes the size and many more
void makelogic()
{
    // The variables prevx, prevy, prev2x, prev2y
    // are used for incrementing the length of the snake
    // after eating the fruit.
    int i;
    int prevx, prevy, prev2x, prev2y;

    prevx = tailx[0];
    prevy = taily[0];

    tailx[0] = x;
    taily[0] = y;

    // Snake touches its own tail, end the game
    for (i = 1; i <= piece; i++)
    {
        if (x == tailx[i] && y == taily[i])
        {
            gameend = 1; 
            return;
        }
    }

    for (i = 1; i <= piece; i++)
    {
        prev2x = tailx[i];
        prev2y = taily[i];

        tailx[i] = prevx;
        taily[i] = prevy;

        prevx = prev2x;
        prevy = prev2y;
    }

    //For the direction of the snake
    switch (flag)
    {

    case 1:
    {
        // If the user wanted to go upward in the direction
        x--;
        break;
    }

    case 2:
    {
        // If the user wanted to go downward in the direction
        x++;
        break;
    }

    case 3:
    {
        // If the user wanted to go in the left direction
        y--;
        break;
    }

    case 4:
    {
        // If the user wanted to go in the right direction
        y++;
        break;
    }
    
    }

    // If the snake touches the wall, then end the game
    if (x == 0 || x == height - 1 || y == 0 || y == width - 1)
    {
        gameend = 1;
    }

    // If the snake eats the food and the food gets
    // randomly on the screen
    if (x == fruitx && y == fruity)
    {
        if (x == fruitx && y == fruity)
        {
        label3:
            fruitx = rand() % (height - 2) + 1;
        if (fruitx == 0)
            goto label3;

        label4:
            fruity = rand() % (width - 2) + 1;
        if (fruity == 0)
            goto label4;
        
        generatequestion();
        //score increases by 10
        score = score + 10;
        piece++;
        }
    }
}

// this function has been created for moving the snake
void input()
{
   //system function to switch the terminal input mode to "raw," allowing immediate
    //capture of single keypresses without Enter. It  reads a character from the standard 
    //input and switches the terminal input mode back to "cooked," providing line buffering 
    //and editing features.

    char c;
    system("/bin/stty raw");
    c = getchar();
    system("/bin/stty cooked");

    switch (c)
    {
        case 'w':
        {
            flag = 1;
            break;
        }

        case 's':
        {
            flag = 2;
            break;
        }

        case 'a':
        {
            flag = 3;
            break;
        }

        case 'd':
        {
            flag = 4;
            break;
        }
    }
}

// This function sets the coordinates for the snake and fruit
void setup()
{
    // x, y puts the snake at the center position
    x = height / 2;
    y = width / 2;

    // Random position of fruits on the board
    label1:
    fruitx = rand() % (height - 2) + 1;
    if (fruitx == 0)
        goto label1;

    label2:
    fruity = rand() % (width - 2) + 1;
    if (fruity == 0)
        goto label2;

    // Game will be running until gameend is 1
    gameend = 0;

    score = 0;
    piece++;
}

// This function draws the board
void draw()
{
    system("clear");
    
    int i, j, k, ch;
    
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                printf("%s*%s", RED, RESET);
            } 
            
            else 
            {
                if (i == x && j == y)
                {
                    printf("%sO%s", GREEN, RESET);
                }
                
                else if (i == fruitx && j == fruity)
                {
                    printf("%s.%s", YELLOW, RESET);
                } 
                
                else
                {
                    ch = 0;
                    
                    for (k = 0; k < piece; k++)
                    {
                        if (i == tailx[k] && j == taily[k])
                        {
                            printf("%so%s", BLUE, RESET);
                            
                            ch = 1;
                        }
                    }
                    if (ch == 0)
                    {
                        printf(" ");
                    }
                }
            }
        }
        printf("\n");
    }
    
    printf("%sYour score is: %d%s\n", CYAN, score, RESET);
}

int main()
{
    printf("%sWelcome to the Snake Game!%s\n", CYAN, RESET);
    printf("%sThe objective is to eat the fruit and grow your snake as long as possible.%s\n", YELLOW, RESET);
    printf("%sYou will be asked math and relational questions during the game, and answering them correctly will increase your score and snake length.%s\n", GREEN, RESET);
    printf("%sUse the 'w', 's', 'a', and 'd' keys to control the snake's movement.%s\n", BLUE, RESET);
    printf("%sGood luck and have fun!%s\n\n", MAGENTA, RESET);
    setup();

    while (gameend != 1)
    {
        input();
        draw();
        makelogic();
        usleep(100000); // Adjust the delay for snake speed
    }

    return 0;
}