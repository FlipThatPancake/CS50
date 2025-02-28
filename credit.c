// Example test card numbers:
// American Express	378282246310005
// American Express	371449635398431
// American Express Corporate	378734493671000
// Diners Club	30569309025904
// Discover	6011111111111117
// Discover	6011000990139424
// JCB	3530111333300000
// JCB	3566002020360505
// Mastercard	2221000000000009
// Mastercard	2223000048400011
// Mastercard	2223016768739313
// Mastercard	5555555555554444
// Mastercard	5105105105105100
// Visa	4111111111111111
// Visa	4012888888881881
// Visa	4222222222222

// 1 Prompt input
// 2 Calculat checksum
// 3 Check credit card length and starting digits
// 4 Print result

// ----------------------PROGRAM STARTS HERE------------------------------- //

#include <cs50.h>
#include <math.h>
#include <stdio.h>

long get_card_n(void);
bool checksum(long card_n);
string what_card(long card_n);
int check_len(long card_n);

int main(void)
{
    long card_n = get_card_n();
    if (checksum(card_n) == true)
    {
        string card_provider = what_card(card_n);
        printf("%s\n", card_provider);
    }
    else
    {
        printf("INVALID\n");
    }
}

// Get input from the user
long get_card_n(void)
{
    long card_n;
    do
    {
        card_n = get_long("Card number: ");
    }
    while (card_n < 0 || card_n > 6000000000000000);
    return card_n;
}

// Check input len
int check_len(long card_n)
{
    int count = 0;
    while (card_n > 0)
    {
        count += 1;
        card_n = card_n / 10;
    }
    return count;
}

// Perform checksum check of the card number
bool checksum(long card_n)
{
    int sum = 0;
    bool isAlternate = false;

    while (card_n > 0)
    {
        if (isAlternate == true)
        {
            int last_dig = card_n % 10;
            int doubled = last_dig * 2;
            sum += doubled / 10 + doubled % 10;
        }
        else
        {
            int last_dig = card_n % 10;
            sum += last_dig;
        }
        isAlternate = !isAlternate;
        card_n = card_n / 10;
    }

    return sum % 10 == 0;
    // If the remainder is 0, the expression evaluates to true
    // If the remainder is anything else, the expression evaluates to false
}

// Identify card company
string what_card(long card_n)
{
    int input_len = check_len(card_n);

    // // That's one way to do this but too man lines
    // long first_digits = card_n;
    // while (first_digits >= 100)
    // {
    //     first_digits /= 10;
    // }

    // int first_digit = first_digits / 10;
    // int first_two_digits = first_digits;

    int first_two_digits = card_n / pow(10, (input_len - 2));

    if ((first_two_digits == 34 || first_two_digits == 37) &&
        input_len == 15) // American Express - 15 digits - starts with 34, 37
    {
        return "AMEX";
    }
    else if ((first_two_digits >= 51 && first_two_digits <= 55) &&
             input_len == 16) // Master Card - 16 digits - starts with 51, 52, 53, 54, 55
    {
        return "MASTERCARD";
    }
    else if ((first_two_digits / 10 == 4) &&
             (input_len == 13 || input_len == 16)) // Visa - 13 or 16 digits - starts with 4
    {
        return "VISA";
    }
    else
    {
        return "INVALID";
    }
}
