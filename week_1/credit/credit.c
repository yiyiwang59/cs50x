#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <stdlib.h>

bool validate(long num);
int digits(long number);
string which_card(long number);

int main(void)
{
    long credit_card_num = get_long("What's the credit card number?\n");
    bool result = validate(credit_card_num);
    if (result == true)
    {
        string card = which_card(credit_card_num);
        printf("%s\n", card);
    }
    else
    {
        printf("INVALID\n");
    }
}

bool validate(long num)
{
    int sum_mult_dig = 0;
    int sum_other_dig = 0;
    int n = digits(num);
    for (int i=1; i<n+1; i++)
    {
        int last_digit = num%10;
        num = num/10;
        if (i%2 == 0)
        {
            int product = last_digit * 2;
            do
            {
                sum_mult_dig += product%10;
                product = product/10;
            }
            while (product != 0);
        }
        else
        {
            sum_other_dig += last_digit;
        }
    }
    int final_sum = sum_mult_dig + sum_other_dig;
    if (final_sum%10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int digits(long number)
{
    int count = 0;
    while (number > 0)
    {
        number = number/10;
        count++;
    }
    return count;
}

string which_card(long number)
{
    int digit = digits(number);
    long start = number;
    do
    {
        start = start / 10;
    }
    while (start > 100);
    if (digit == 15 && (start == 34 || start == 37))
    {
        return "AMEX";
    }
    else if (digit == 16 && 51 <= start && start <= 55)
    {
        return "MASTERCARD";
    }
    else if ((digit == 13 || digit == 16) && start/10 == 4)
    {
        return "VISA";
    }
    else
    {
        return "INVALID";
    }
}
