##include "list.h"

Number *power(Number *n1, Number *n2)
{
    int reciprocate = 0, answerNegative = 0;
    if (n2->sign == MINUS)
    {
        reciprocate = 1;
    }
    Number *ans = (Number *)malloc(sizeof(Number));
    initNumber(ans);
    Number *one = (Number *)malloc(sizeof(Number));
    initNumber(one);
    insertAtBegining(one, 1);
    Number *two = (Number *)malloc(sizeof(Number));
    initNumber(two);
    insertAtBegining(two, 2);
    if (isNumber0(n2))
    {
        return one;
    }
    else if (isNumber0(sub(n2, one)))
    {
        return n1;
    }
    // copy n1 to answer
    node *lastDigit = n2->tail;

    if (n1->sign == PLUS && n2->sign == MINUS)
    {
        n2->sign = PLUS;
    }
    else if (n1->sign == MINUS)
    {
        if (lastDigit->num % 2)
            // power is odd,hence answer will be negative
            answerNegative = 1;
        n1->sign = PLUS;
        if (n2->sign == MINUS)
        {
            n2->sign = PLUS;
        }
    }
    duplicateNumber(ans, n1);
    n2 = sub(n2, one);
    // displayNumber(n2);
    // while (y > 0)
    // {
    //     if (y % 2)
    //     {
    //         prod *= term;
    //     }
    //     term = term * term;
    //     y = y / 2;
    // }
    // faster algorithm try 2054^3000->it contains 9938 digits,this algo computes this in 1 sec
    while (!isNumber0(n2))
    {
        lastDigit = n2->tail;
        if (lastDigit->num % 2)
        {
            ans = multiply(ans, n1);
        }
        n1 = multiply(n1, n1);
        n2 = floordivide(n2, two);
    }
    // slower approach,takes n2 number of multiplications
    // while (!isNumber0(n2))
    // {
    //     ans = multiply(n1, ans);
    //     // displayNumber(ans);
    //     n2 = sub(n2, one);
    // }
    if (reciprocate)
        ans = divide(one, ans);
    if (answerNegative)
        ans->sign = MINUS;
    return ans;
}