#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 50
#define MAX_TOKENS 500
#define DICT_SIZE 100910
#define MAX_INPUT_SIZE 5000

void remove_character(char *str, char c) // To remove '\n' character
{
    if (str == NULL)
        return;

    char *read_ptr = str;
    char *write_ptr = str;

    while (*read_ptr)
    {
        if (*read_ptr != c)
        {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }

    *write_ptr = '\0'; // Add null terminator to the end of the modified string
}

int check_matching(char *token, char *dictdata) // To check the string are matching at least 70% or not
{
    int count = 0;
    int token_len = strlen(token);
    int dictdata_len = strlen(dictdata);

    if (token_len == dictdata_len)
    {
        for (int i = 0; i < token_len; i++)
        {
            if (token[i] == dictdata[i])
                count++;
        }

        if (count <= 0.8 * token_len - 1)
            return -1;
        else
            return 0;
    }
    else
        return -1;
}

void spelling_checker(char *sentence)
{
    FILE *fp = NULL;
    int cnt1, cnt2;
    int tokens_index = 0;
    int data_index = 0;
    int bytes_read = 0;
    int match_count = 0;
    int match_result = 0;
    char *temptok = NULL;
    char buffer[BUFFER_SIZE];
    char *tokens[MAX_TOKENS];
    int *spellerrorinfo = NULL;
    char *dictionary[DICT_SIZE];
    const char *delimiters = ",.?!'\" ";

    temptok = strtok(sentence, delimiters);

    while (temptok != NULL) // Tokenize
    {
        tokens[tokens_index++] = strlwr(temptok);
        temptok = strtok(NULL, delimiters);
    }

    spellerrorinfo = (int *)malloc(sizeof(int) * tokens_index);

    fp = fopen("Dictionary.txt", "r");

    while (fgets(buffer, BUFFER_SIZE, fp) != NULL)
    {
        remove_character(buffer, '\n');
        dictionary[data_index++] = strdup(buffer);
    }

    for (cnt1 = 0; cnt1 < tokens_index; cnt1++)
    {
        for (cnt2 = 0; cnt2 < data_index; cnt2++)
        {
            if (strcmp(tokens[cnt1], dictionary[cnt2]) == 0)
            {
                spellerrorinfo[cnt1] = 0;
                break;
            }
        }
        if (cnt2 == data_index)
            spellerrorinfo[cnt1] = 1;
    }

    for (cnt1 = 0; cnt1 < tokens_index; cnt1++)
    {
        if (spellerrorinfo[cnt1] != 0)
            match_count++;
    }

    if (match_count == 0)
    {
        puts("\nYour spellings are all correct.");
        return;
    }

    for (cnt1 = 0; cnt1 < tokens_index; cnt1++)
    {
        match_count = 0;

        for (cnt2 = 0; cnt2 < data_index; cnt2++)
        {
            if (spellerrorinfo[cnt1] == 1)
            {
                match_result = check_matching(tokens[cnt1], dictionary[cnt2]);

                if (match_result == 0)
                {
                    match_count++;
                    printf("\nAt token %d did you mean %s?\n", cnt1 + 1, dictionary[cnt2]);
                }
            }
        }
        if (match_count == 0 && spellerrorinfo[cnt1] == 1)
            printf("\nIt seems you have entered the word %d with incorrect spelling.\n", cnt1 + 1);
    }
}

int main(void)
{
    char cSentence[MAX_INPUT_SIZE];

    puts("Enter the sentence to check spelling : ");
    gets(cSentence);

    spelling_checker(cSentence);

    return 0;
}