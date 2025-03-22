// Longer words mean higher grade reading level
// More words per sentence also correlates with higher grade reading level
// Coleman Liau Index is: .index = 0.0588 * L - 0.296 * S - 15.8
// L = average number of letters per 100 words
// S = average number of sentences per 100 words

// We need to calculate the number ofletters, words and sentences in the text
// and use the formula to figure out the grading level.

// We can create a variable 'letters' that keeps track of how many letters there are in the string. Without counting blank spaces.
// Use ctype.h to determine if a letter is in fact a letter.
// Word = any sequence of letters separated by a space.
// To calculate 'words', we can calculate the number of spaces + 1.
// To calculate 'sentences', we can assume !, . or ? indicate sentenced end.

// Round the score to the nearest whole number.
// Output should be "Grade X" - X is the grade level

// Text examples:
// One fish. Two fish. Red fish. Blue fish. (Before Grade 1)

// Would you like them here or there? I would not like them here or there. I would not like them anywhere. (Grade 2)

// Congratulations! Today is your day. You're off to Great Places! You're off and away! (Grade 3)

// Harry Potter was a highly unusual boy in many ways. For one thing, he hated the summer holidays more
// than any other time of year. For another, he really wanted to do his homework, but was forced to do
// it in secret, in the dead of the night. And he also happened to be a wizard. (Grade 5)

// In my younger and more vulnerable years my father gave me some advice that I've been turning over
// in my mind ever since. (Grade 7)

// Alice was beginning to get very tired of sitting by her sister on the bank, and of having nothing
// to do: once or twice she had peeped into the book her sister was reading, but it had no pictures
// or conversations in it, "and what is the use of a book," thought Alice "without pictures or conversation?" (Grade 8)

// When he was nearly thirteen, my brother Jem got his arm badly broken at the elbow. When it healed,
// and Jem's fears of never being able to play football were assuaged, he was seldom self-conscious about his injury.
// His left arm was somewhat shorter than his right; when he stood or walked, the back of his hand was at right
//  angles to his body, his thumb parallel to his thigh. (Grade 8)

// There are more things in Heaven and Earth, Horatio, than are dreamt of in your philosophy. (Grade 9)

// It was a bright cold day in April, and the clocks were striking thirteen. Winston Smith, his chin
//  nuzzled into his breast in an effort to escape the vile wind, slipped quickly through the glass
//  doors of Victory Mansions, though not quickly enough to prevent a swirl of gritty dust from entering along with him. (Grade 10)

// A large class of computational problems involve the determination of properties of graphs, digraphs,
//  integers, arrays of integers, finite families of finite sets, boolean formulas and elements of other countable domains. (Grade 16+)


#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Compute the Coleman-Liau index
        // Coleman Liau Index is: .index = 0.0588 * L - 0.296 * S - 15.8
        // L = average number of letters per 100 words
        // S = average number of sentences per 100 words

        // use (float) when calculating L and S to avoid trunction

    float l = (float) letters / words * 100;
    float s = (float) sentences / words * 100;

    // Coleman Liau Index Formula
    float index = 0.0588 * l - 0.296 * s - 15.8;

    // Print the grade level using round(lvl) - rounds to nearest whole num away from zero
    int grade_lvl = round(index);
    // if index num greater > 16, return 16+
    if (grade_lvl < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade_lvl > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade_lvl);
    }
}

int count_letters(string text)
{
    // Return the number of letters in text
    int letter_count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if ((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z'))
        {
            letter_count++;
        }
    }
    return letter_count;
}

int count_words(string text)
{
    // Return the number of words in text
    int word_count = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == ' ')
        {
            word_count++;
        }
    }
    return word_count;
}

int count_sentences(string text)
{
    // Return number of sentences in text
    int sentence_count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentence_count++;
        }
    }
    return sentence_count;
}
