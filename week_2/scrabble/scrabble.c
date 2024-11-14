#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct
    {
        char key;
        int value;

    } kv_pair;

kv_pair dictionary[26];

int calculate_score(string word, kv_pair scoring[]);
string calculate_winner(int score1, int score2);

int main(void)
{
    char keys[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    int values[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    for (int i = 0; i < 26; i++)
    {
        dictionary[i].key = keys[i];
        dictionary[i].value = values[i];

    }
    string player1_word = get_string("Player 1: ");
    string player2_word = get_string("Player 2: ");
    int player1_score = calculate_score(player1_word, dictionary);
    int player2_score = calculate_score(player2_word, dictionary);
    string get_winner = calculate_winner(player1_score, player2_score);
    //printf("Player 1 score: %i\nPlayer 2 score: %i\n", player1_score, player2_score);
    printf("%s\n", get_winner);

}

int calculate_score(string word, kv_pair scoring[])
{
    int score = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            char current_letter = word[i];
            if (islower(current_letter))
            {
                current_letter = toupper(current_letter);
            }
            if (current_letter == scoring[j].key)
            {
                score += scoring[j].value;
                break;
            }
        }
    }
    return score;
}

string calculate_winner(int score1, int score2)
{
    if (score1 == score2)
    {
        return "Tie!";
    }
    else if (score1 > score2)
    {
        return "Player 1 wins.";
    }
    else
    {
        return "Player 2 wins.";
    }
}
