#include <iostream>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <fstream>
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::remove_if;
using std::string;
using std::stringstream;
using std::vector;

string to_lower(string &str) // To make string lowercase
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    return str;
}

vector<string> &read_data(string strFileName, vector<string> &strData) // To read the data into array (lowercase)
{
    string line;

    ifstream datafile("Dictionary.txt");

    if (!datafile)
        return strData;

    while (getline(datafile, line))
        strData.push_back(line);

    datafile.close();

    return strData;
}

int check_matching(string &strToken, string &strData) // To check the strings are matching at least 70% or not
{
    int count = 0;

    if (strToken.size() == strData.size())
    {
        for (int i = 0; i < strToken.size(); i++)
        {
            if (strToken[i] == strData[i])
                count++;
        }

        if (count <= 0.8 * strToken.size() - 1) // Checks if string matches at least 70% or not
            return -1;
        else
            return 0;
    }

    else
        return -1;
}

void spelling_checker(string strSentence)
{
    int i, j;
    int match_result;
    int match_count = 0;
    int print_counter;
    string token;
    vector<int> spellerrorinfo;
    vector<string> tokens;
    vector<string> data;
    stringstream ss(strSentence);

    while (ss >> token)
    {
        token.erase(remove_if(token.begin(), token.end(), ispunct), token.end());
        tokens.push_back(token);
    }

    for (i = 0; i < tokens.size(); i++)
        to_lower(tokens[i]);

    read_data("Dictionary.txt", data);

    for (i = 0; i < tokens.size(); i++)
    {
        for (j = 0; j < data.size(); j++)
        {
            if (tokens[i].compare(data[j]) == 0)
            {
                spellerrorinfo.push_back(0);
                break;
            }
        }
        if (j == data.size())
            spellerrorinfo.push_back(1);
    }

    for (i = 0; i < spellerrorinfo.size(); i++)
    {
        if (spellerrorinfo[i] != 0)
            match_count++;
    }

    if (match_count == 0)
    {
        cout << "\nYour spellings are all correct.\n";
        return;
    }

    for (i = 0; i < spellerrorinfo.size(); i++)
    {
        print_counter = match_count = 0;

        for (j = 0; j < data.size(); j++)
        {
            if (spellerrorinfo[i] == 1)
            {
                match_result = check_matching(tokens[i], data[j]);

                if (match_result == 0)
                {
                    if (print_counter == 0)
                    {
                        cout << "\nInstead of \'" << tokens[i] << "\' did you mean?\n";
                        print_counter++;
                    }
                    match_count++;
                    cout << " - " << data[j] << endl;
                }
            }
        }

        if (match_count  == 0 && spellerrorinfo[i] == 1)
            cout << "\nIt seems you have entered the word " << i + 1 << " with incorrect spelling.\n";
    }
}

int main(void)
{
    string str;

    while (1)
    {
        cout << "\nEnter the sentence to check spelling : \n";
        getline(cin, str);

        spelling_checker(str);
    }

    return 0;
}