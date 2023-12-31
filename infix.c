#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "list.h"
#include "infix.h"
#include <stdio.h>
#include <ctype.h>

enum state
{
    SPACE,
    NUM,
    OP,
    DECIMAL,
    FINISH,
    ERROR
};

token getToken(char *expr, int *reset)
{
    static int i = 0;
    static enum state currstate = SPACE;
    enum state nextstate;
    char currentChar;
    if (*reset == 1)
    {
        currstate = SPACE;
        *reset = 0;
        i = 0;
    }
    token t;
    Number *new_number = (Number *)malloc(sizeof(Number));
    initNumber(new_number);
    while (1)
    {
        currentChar = expr[i];
        switch (currstate)
        {
        case SPACE:
            switch (currentChar)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                t.type = OPERAND;
                t.num = new_number;
                nextstate = NUM;
                currstate = nextstate;
                // i++;

                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
            case '(':
            case ')':

                nextstate = OP;
                currstate = nextstate;
                i++;

                break;
            case ' ':
                nextstate = SPACE;
                currstate = nextstate;
                i++;

                break;
            case '\0':

                nextstate = FINISH;
                currstate = nextstate;
                break;
            case '.':

                nextstate = DECIMAL;
                currstate = nextstate;
                i++;

                break;
            default:
                nextstate = ERROR;
                currstate = nextstate;
                break;
            }
            break;
        case NUM:
            switch (currentChar)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                // t.type = OPERAND;
                // t.num = new_number;
                appendDigit(new_number, currentChar);
                nextstate = NUM;
                currstate = nextstate;
                i++;

                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
            case '(':
            case ')':
                t.type = OPERAND;
                t.num = new_number;
                nextstate = OP;
                currstate = nextstate;
                i++;
                return t;
                break;
            case ' ':
                t.type = OPERAND;
                t.num = new_number;
                nextstate = SPACE;
                currstate = nextstate;
                i++;
                return t;
                break;
            case '\0':
                t.type = OPERAND;
                t.num = new_number;
                nextstate = FINISH;
                currstate = nextstate;
                return t;

                break;
            case '.':

                nextstate = DECIMAL;
                currstate = nextstate;
                i++;

                break;
            default:
                t.type = OPERAND;
                t.num = new_number;
                nextstate = ERROR;
                currstate = nextstate;

                return t;
                break;
            }
            break;
        case OP:
            switch (currentChar)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                t.type = OPERATOR;
                t.op = expr[i - 1];
                nextstate = NUM;
                currstate = nextstate;
                // i++;
                return t;
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
            case '(':
            case ')':
                t.type = OPERATOR;
                t.op = expr[i - 1];
                nextstate = OP;
                currstate = nextstate;
                i++;
                return t;
                break;
            case ' ':
                t.type = OPERATOR;
                t.op = expr[i - 1];
                nextstate = SPACE;
                currstate = nextstate;
                i++;
                return t;
                break;
            case '\0':
                t.type = OPERATOR;
                t.op = expr[i - 1];
                nextstate = FINISH;
                currstate = nextstate;

                return t;
                break;
            case '.':
                t.type = OPERATOR;
                t.op = expr[i - 1];
                nextstate = DECIMAL;
                currstate = nextstate;
                i++;
                return t;

                break;
            default:
                t.type = OPERATOR;
                t.op = expr[i - 1];
                nextstate = ERROR;
                currstate = nextstate;
                return t;
                break;
            }
            break;
        case DECIMAL:
            switch (currentChar)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                appendDigit(new_number, currentChar);
                new_number->dec++;
                nextstate = DECIMAL;
                currstate = nextstate;
                i++;

                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
            case '(':
            case ')':
                t.type = OPERAND;
                t.num = new_number;
                nextstate = OP;
                currstate = nextstate;
                i++;
                return t;
                break;
            case ' ':
                t.type = OPERAND;
                t.num = new_number;
                nextstate = SPACE;
                currstate = nextstate;
                i++;
                return t;
                break;
            case '\0':
                t.type = OPERAND;
                t.num = new_number;
                nextstate = FINISH;
                currstate = nextstate;

                return t;
                break;
            case '.':
                // this is my choice

                nextstate = ERROR;
                currstate = nextstate;
                i++;
                return t;
                break;
            default:
                t.type = OPERAND;
                t.num = new_number;
                nextstate = ERROR;
                currstate = nextstate;
                i++;
                return t;
                break;
            }
            break;

        case FINISH:
            t.type = END;
            return t;
            break;
        case ERROR:
            t.type = ERR;
            return t;
            break;
        default:
            break;
        }
    }
}

int precedence(char op)
{
    switch (op)
    {
    case ')':
        return 3;
        break;
    case '+':
    case '-':
        return 4;
        break;
    case '*':
    case '/':
        return 5;
        break;
    case '%':
        return 6;
        break;
    case '^':
        return 7;
        break;
    case '(':
        return 8;
        break;

    default:
        // this case will never occur since our getnext function will handle any error in the string
        return 100;
        break;
    }
}

Number *infix(char *expression)
{
    char ch, prev_op, curr_op;
    int reset = 1, OBcount = 0, CBcount = 0, prev, curr;
    istack operandStack;
    cstack operatorStack;
    cinit(&operatorStack);
    init(&operandStack);
    token t;
    Number *a = (Number *)malloc(sizeof(Number));
    Number *b = (Number *)malloc(sizeof(Number));
    Number *answer = (Number *)malloc(sizeof(Number));
    initNumber(a);
    initNumber(b);
    initNumber(answer);
    prev = ERR;
    while (1)
    // for (int i = 0; i < 5; i++)
    {
        t = getToken(expression, &reset);
        curr = t.type;
        if (curr == OPERAND && prev == OPERAND)
        {
            // there is some error
            return NULL;
        }
        if (curr == OPERAND)
        {
            // printf("%d", t.num->head->num);
            push(&operandStack, t.num);
        }
        else if (curr == OPERATOR)
        {

            curr_op = t.op;
            if (curr_op == '(')
                OBcount++;
            if (curr_op == ')')
                CBcount++;
            if (CBcount > OBcount)
            {
                printf("Unblanced paranthesis");
                return NULL;
            }
            if (!cisEmpty(operatorStack))
            {
                prev_op = ctop(operatorStack);
                // for starting situation we dont hav prev_op,hence if stack is empty then dont do all this
                while (precedence(curr_op) <= precedence(prev_op))
                {
                    prev_op = cpop(&operatorStack);
                    if (!isEmpty(operandStack))
                    {
                        a = pop(&operandStack);
                    }
                    else
                    {
                        cpush(&operatorStack, prev_op);
                        break;
                    }
                    if (!isEmpty(operandStack))
                    {
                        b = pop(&operandStack);
                    }
                    else
                    {
                        cpush(&operatorStack, prev_op);
                        push(&operandStack, a);
                        break;
                    }
                    if ((prev_op == '/' || prev_op == '%') && a == 0)
                    {
                        printf("Mathematical error\n");
                        return NULL;
                    }
                    switch (prev_op)
                    {
                    case '+':
                        // answer = b + a;
                        answer = add(a, b);
                        push(&operandStack, answer);
                        break;
                    case '-':
                        // answer = b - a;
                        answer = sub(b, a);
                        push(&operandStack, answer);
                        break;
                    case '*':
                        // answer = b * a;
                        answer = multiply(b, a);
                        push(&operandStack, answer);
                        break;
                    case '/':
                        // answer = b / a;
                        answer = divide(b, a);
                        push(&operandStack, answer);
                        break;
                    case '%':
                        // answer = b % a;
                        answer = modulus(b, a);
                        push(&operandStack, answer);
                        break;
                    case '^':
                        // answer = b % a;
                        answer = power(b, a);
                        push(&operandStack, answer);
                        break;
                    case '(':
                        // (-7)+(-3),in this case ( comes as an operator
                        cpush(&operatorStack, prev_op);
                        push(&operandStack, b);
                        push(&operandStack, a);
                        break;
                    default:
                        printf("Invalid Operator\n");
                        return NULL;
                        break;
                    }
                    if (prev_op == '(')
                        break;
                    if (!cisEmpty(operatorStack))
                        prev_op = ctop(operatorStack);
                    else
                        break;
                }
            }
            cpush(&operatorStack, curr_op);

            if (curr_op == ')')
            {
                // pop opening and closing brackets from stack
                cpop(&operatorStack);
                CBcount--;
                cpop(&operatorStack);
                OBcount--;
            }
        }
        else if (t.type == END)
        {
            if (CBcount == OBcount)
            {
                while (!cisEmpty(operatorStack))
                {

                    if (!isEmpty(operandStack))
                    {
                        a = pop(&operandStack);
                    }
                    else
                    {
                        printf("Less operands\n");
                        return NULL;
                    }
                    if (!isEmpty(operandStack))
                    {
                        b = pop(&operandStack);
                    }
                    else
                    {
                        printf("Less operands\n");
                        return NULL;
                    }
                    ch = cpop(&operatorStack);

                    if ((ch == '/' || ch == '%') && a == 0)
                    {
                        printf("Mathematical error\n");
                        return NULL;
                    }
                    switch (ch)
                    {
                    case '+':
                        // answer = b + a;

                        answer = add(a, b);
                        push(&operandStack, answer);
                        break;
                    case '-':
                        // answer = b - a;
                        answer = sub(b, a);
                        push(&operandStack, answer);
                        break;
                    case '*':
                        // answer = b * a;
                        answer = multiply(b, a);
                        push(&operandStack, answer);
                        break;
                    case '/':
                        // answer = b / a;
                        answer = divide(b, a);
                        push(&operandStack, answer);
                        break;
                    case '%':
                        // answer = b % a;
                        answer = modulus(b, a);
                        push(&operandStack, answer);
                        break;
                    case '^':
                        // answer = b % a;
                        answer = power(b, a);
                        push(&operandStack, answer);
                        break;
                    default:
                        printf("Invalid Operator\n");
                        return NULL;
                        break;
                    }
                }
            }
            else
            {
                printf("Error in Expression\n");
                //return INT_MIN;
                return NULL;
            }
            if (!isEmpty(operandStack))
            {
                answer = pop(&operandStack);
                if (isEmpty(operandStack))
                {
                    return answer;
                    // printf("Result:%d\n", answer);
                }
                else
                {
                    printf("Less Operators\n");
                    return NULL;
                }
            }
            else
            {
                printf("Less operands\n");
                return NULL;
            }
        }
        else if (t.type == ERR)
        {
            return NULL;
        }
        prev = curr;
    }
}