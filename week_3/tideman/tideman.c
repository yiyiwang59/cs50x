#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
    int margin;
} pair;

typedef struct
{
    string candidate;
    int num_won;
    int total_margin;
} result;

result results[MAX];
// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void initialize_results(void);
void merge_sort(pair pairs_to_sort[], int num_pairs);
bool detect_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
        // printf("%s\n", candidates[i]);
    }

    initialize_results();

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
            preferences[i][j] = 0;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int index_i = 0;
    int index_j = 0;
    for (int i = 0; i < candidate_count - 1; i++)
    {
        index_i = ranks[i];
        for (int j = i + 1; j < candidate_count; j++)
        {
            index_j = ranks[j];
            preferences[index_i][index_j]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pairs[pair_count].margin = preferences[j][i] - preferences[i][j];
                // printf("Pair:\nWinner: %s\nLoser: %s\n Margin:%i\n", candidates[j], candidates[i],  pairs[pair_count].margin);
                pair_count++;
            }
            else if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pairs[pair_count].margin = preferences[i][j] - preferences[j][i];
                // printf("Pair:\nWinner: %s\nLoser: %s\n Margin:%i\n", candidates[i], candidates[j],  pairs[pair_count].margin);
                pair_count++;
            }

        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    merge_sort(pairs, pair_count);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i=0; i < pair_count; i++)
    {
        // printf("Sorted Pairs:\nWinner: %s\nLoser: %s\n Margin:%i\n", candidates[pairs[i].winner], candidates[pairs[i].loser],  pairs[i].margin);
        bool lock_pair = true;
        for (int j=0; j < candidate_count; j++)
        {
            if (locked[j][pairs[i].winner] == true && locked[pairs[i].loser][j] == true)
            {
                lock_pair = false;
            }
        }

        if (detect_cycle(pairs[i].winner, pairs[i].loser))
        {
            lock_pair = false;
        }

        if (lock_pair == true)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            results[pairs[i].winner].num_won++;
            results[pairs[i].winner].total_margin = pairs[i].margin;
            // printf("Pair Locked:\n%s\n%s\nMargin: %i\n", candidates[pairs[i].winner], candidates[pairs[i].loser], results[pairs[i].winner].total_margin);

        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int winners[MAX];
    int winners_index = 0;
    for (int i=0; i < candidate_count; i++)
    {
        bool has_lost = false;
        for (int j=0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                has_lost = true;
                break;
            }
        }
        if (!has_lost)
        {
            winners[winners_index] = i;
            winners_index++;
        }
    }
    if (winners_index == 1)
    {
        printf("%s\n", candidates[winners[0]]);
    }
    else if (winners_index > 1)
    {
        int max_num_won = 0;
        int num_won_winners[winners_index+1];
        int num_won_winners_inx = 0;
        for (int k=0; k < winners_index; k++)
        {
            if (results[winners[k]].num_won >= max_num_won)
            {
                max_num_won = results[winners[k]].num_won;
                num_won_winners[num_won_winners_inx] = winners[k];
                num_won_winners_inx++;
            }

        }
        if (num_won_winners_inx == 1)
        {
            printf("%s\n", candidates[num_won_winners[0]]);
        }
        else if (num_won_winners_inx > 1)
        {
            int max_margin = 0;
            int margin_winner[num_won_winners_inx+1];
            int margin_winner_inx = 0;
            for (int n=0; n < num_won_winners_inx; n++)
            {
                if (results[num_won_winners[n]].total_margin >= max_margin)
                {
                    max_margin = results[num_won_winners[n]].total_margin;
                    margin_winner[margin_winner_inx] = num_won_winners[margin_winner_inx];
                    margin_winner_inx++;
                }

            }
            if (margin_winner_inx == 1)
            {
                printf("%s\n", candidates[margin_winner[0]]);
        }

        }

    }
    return;
}

void initialize_results(void)
{
    for (int i=0; i < candidate_count; i++)
    {
        results[i].candidate = candidates[i];
        results[i].num_won = 0;
        results[i].total_margin = 0;
    }
}

void merge_sort(pair pairs_to_sort[], int num_pairs)
{
    if (num_pairs > 1)
    {
        int left_arr_len = round(num_pairs / 2);
        int right_arr_len = num_pairs - left_arr_len;
        pair left_pairs[left_arr_len];
        pair right_pairs[right_arr_len];

        for (int i=0; i<left_arr_len; i++)
        {
            left_pairs[i] = pairs_to_sort[i];
        }
        for (int i=0, j=left_arr_len; j < num_pairs; i++, j++)
        {
            right_pairs[i] = pairs_to_sort[j];
        }

        merge_sort(left_pairs, left_arr_len);
        merge_sort(right_pairs, right_arr_len);

        int left_index = 0;
        int right_index = 0;
        int merged_index = 0;

        while (left_index < left_arr_len && right_index < right_arr_len)
        {
            if (left_pairs[left_index].margin > right_pairs[right_index].margin)
            {
                pairs_to_sort[merged_index] = left_pairs[left_index];
                left_index++;
            }
            else
            {
                pairs_to_sort[merged_index] = right_pairs[right_index];
                right_index++;
            }
            merged_index++;
        }

        while (left_index < left_arr_len)
        {
            pairs_to_sort[merged_index] = left_pairs[left_index];
            left_index++;
            merged_index++;
        }

        while (right_index < right_arr_len)
        {
            pairs_to_sort[merged_index] = right_pairs[right_index];
            right_index++;
            merged_index++;
        }
    }
    return;
}

bool detect_cycle(int winner, int loser)
{
    bool continue_cycle = false;
    if (winner == loser)
    {
        return true;
    }
    for (int i=0; i<pair_count; i++)
    {
        if (locked[pairs[i].winner][pairs[i].loser] && pairs[i].winner == loser)
        {
            continue_cycle = detect_cycle(loser, i);
            if (continue_cycle)
            {
                return true;
            }
        }
    }
    return false;
}
