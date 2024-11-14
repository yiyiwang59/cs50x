#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int calculate_formula(int num_words, int num_sentences, int num_letters);
int grade_level(string text);
void return_range(int result);

int main(void)
{
    string text = get_string("Text: ");
    int grade = grade_level(text);
    return_range(grade);
}

int grade_level(string text)
{
    int word_count = 1;
    int sentence_count = 0;
    int letter_count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == ' ')
        {
            word_count++;
        }
        else if (text[i] == '?' || text[i] == '.' || text[i] == '!')
        {
            sentence_count++;
        }
        else if (isalpha(text[i]))
        {
            letter_count++;
        }
    }
    int answer = calculate_formula(word_count, sentence_count, letter_count);
    return answer;
}

int calculate_formula(int num_words, int num_sentences, int num_letters)
{
    //average number of letters per 100 words in the text
    float L = (float) num_letters/(float) num_words * 100.0;
    //average number of sentences per 100 words in the text
    float S = (float) num_sentences/(float) num_words * 100.0;
    float result = 0.0588 * L - 0.296 * S - 15.8;
    return round(result);
}

void return_range(int result)
{
    if (result >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (result < 1)
    {
        printf ("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", result);
    }
}
