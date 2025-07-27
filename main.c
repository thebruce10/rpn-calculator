#include <stdio.h>
#include <stdlib.h> //for atof
#include <ctype.h>


//stack
#define MAXSTACKDEPTH 64 //maximum depth of stack
int sp = 0; //next free stack position
double val[MAXSTACKDEPTH]; //stack
//stack functions
void push(double value);
double pop(void);


#define BUFSIZE 64
char buf[BUFSIZE];
int bufp = 0;
int getch(void);
void ungetch(int);


#define MAXOP 64 //the maximum length of an operand or operator
#define NUMBER '0'
int getop(char s[]);


int main(void)
{
    int type;
    double op2; //stores the second operand in subtraction and division as the operands are needed in the specific order
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
            case NUMBER:
                push(atof(s));
                break;

            case '+':
                push(pop() + pop()); //adds the first two items on the stack before pushing them back
                break;

            case '*':
                push(pop() * pop()); //multiplies the first two items on the stack before pushing them back
                break;

            case '-':
                op2 = pop();
                push(pop() - op2);
                break;

            case '/':
                op2 = pop();
                if (op2 != 0.0) {
                    push(pop() / op2);
                } else {
                    printf("error: zero divisor\n");
                }
                break;

            case '\n':
                if (sp == 1) {
                    printf("\t%.8g\n", pop());
                } else
                {
                    printf("error: syntax\n");
                }
                break;

            default:
                printf("error: unknown command \"%s\"\n", s);
                break;
        }
    }



    return 0;
}



//needed to put the character back into the input buffer if the program reads too much input

//get a (possibly pushed-back) character
int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

//push character back on input
void ungetch(int c) {
    if (bufp >= BUFSIZE) {
        printf("ungetch: too many characters\n");
    } else {
        buf[bufp++] = c;
    }
}



//getop: get next character or numeric operand
int getop(char s[])
{
    int i, c;

    while ((s[0] = c = getch()) == ' ' || c == '\t'); //removes leading whitespace until it reaches a character
    s[1] = '\0';
    if (!isdigit(c) && c != '.') {
        return c; //NaN
    }
    i = 0;
    if (isdigit(c)) { //collect integer part
        while (isdigit(s[++i] = c = getch()));
    }

    if (c == '.') { //collect fraction part
        while (isdigit(s[++i] = c = getch()));
    }

    s[i] = '\0';
    if (c != EOF) {
        ungetch(c);
    }
    return NUMBER;

}



//push: push f onto value stack
void push(double value)
{
    if (sp < MAXSTACKDEPTH) {
        val[sp++] = value;
    } else {
        printf("error: stack full, can't push %g\n", value);
    }
}

//pop: pop and return top value from stack
double pop(void)
{
    if (sp > 0) {
        return val[--sp];
    } else {
        printf("error: stack empty\n");
        return 0.0;
    }
}