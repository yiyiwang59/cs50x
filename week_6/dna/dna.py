import csv
import sys
import pandas


def main():

    # TODO: Check for command-line usage
    database_file = sys.argv[1]
    sequence_file = sys.argv[2]

    if database_file is None or sequence_file is None or len(sys.argv) != 3:
        print("Error: Missing Arguments")
    else:

        # TODO: Read database file into a variable
        database_pd = pandas.read_csv(database_file)

        # TODO: Read DNA sequence file into a variable
        with open(sequence_file, 'r') as file:
            sequence = file.read().strip("\n")

        # TODO: Find longest match of each STR in DNA sequence
        STRs = database_pd.columns.tolist()[1:]
        final_count = {}
        for STR in STRs:
            final_count[STR] = longest_match(sequence, STR)

        # TODO: Check database for matching profiles
        for index, row in database_pd.iterrows():
            match = False
            for STR in STRs:
                if row[STR] == final_count[STR]:
                    match = True
                else:
                    match = False
                    break
            if match == True:
                print(row['name'])
                break
        else:
            print("No match")

def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
