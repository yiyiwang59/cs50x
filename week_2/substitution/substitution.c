#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

string trad_alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string cipher(string key, string message);
char get_cipher_letter(string key, int index, bool is_upper);
char if_alpha(char letter, string key);
string strlwr (string s);
bool validate_key(string key);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else
    {
        if (validate_key(argv[1]) != false)
        {
            string key = argv[1];
            string plain_text = get_string("plaintext: ");
            string output_text = cipher(key, plain_text);
            printf("ciphertext: %s\n", plain_text);
        }
        else
        {
            return 1;
        }
    }
}

string cipher(string key, string message)
{
    for (int i=0; i < strlen(message); i++)
    {
        char letter = message[i];
        if (isalpha(letter))
        {
            letter = if_alpha(letter, key);
            message[i] = letter;
        }
    }
    return message;
}

char get_cipher_letter(string key, int index, bool is_upper)
{
    if (is_upper == true)
    {
        return toupper(key[index]);
    }
    else
    {
        return key[index];
    }
}

char if_alpha(char letter, string key)
{
    char new_letter = '\0';
    for (int j = 0; j < 26; j++)
    {
        if (islower(letter))
        {
            if (letter == tolower(trad_alpha[j]))
            {
                new_letter = get_cipher_letter(key, j, false);
                break;
            }
        }
        if (letter == trad_alpha[j])
        {
            new_letter = get_cipher_letter(key, j, true);
            break;
        }
    }
    return new_letter;
}

bool validate_key(string key)
{
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters\n");
        return false;
    }
    string temp_key = strlwr(key);
    for (int i = 0; i < 26; i++)
    {
        if (isalpha(temp_key[i]) == false)
        {
            printf("Key has invalid characters\n");
            return false;
            break;
        }
        else
        {
            char letter = temp_key[i];
            temp_key[i] = ' ';
            if (strchr(temp_key, letter) != NULL)
            {
                printf("Key has duplicates\n");
                return false;
                break;
            }
            temp_key[i] = letter;
        }
    }
    return true;
}

string strlwr (string s) {
    for (int i = 0; i < strlen(s); ++i)
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] += 'a' - 'A';
    return s;
}
