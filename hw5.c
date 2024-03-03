#include "hw5.h"

extern char *optarg; // pointer to what getopt() just processed
extern int optind;   // what pointer will do next, argc will do next
extern int optopt;   // error holding??

int compare(const char *word, const char *prefix)
{
    if (strlen(prefix) > strlen(word))
    {
        return 0;
    }
    for (size_t i = 0; i < strlen(prefix); i++)
    {
        if (word[i] != prefix[i])
        {
            return 0;
        }
    }
    return 1;
}
const char *startsWithStrStr(const char *ogLine, const char *prefix)
{
    const char *position = ogLine;

    while ((position = strstr(position, prefix)) != NULL)
    {
        if (position == ogLine || isspace(position[-1]) || ispunct(position[-1]))
        {
            return position;
        }
        position++;
    }

    return NULL;
}

const char *endsWithStrStr(const char *ogLine, const char *suffix)
{
    const char *position = ogLine;

    while ((position = strstr(position, suffix)) != NULL)
    {

        const char *suffixEnd = position + strlen(suffix);

        if (suffixEnd[0] == '\0' || isspace(suffixEnd[0]) || ispunct(suffixEnd[0]))
        {
            return position;
        }

        position = suffixEnd;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc < 7)
    {
       // fprintf(stderr, "MISSING_ARGUMENT");
        return MISSING_ARGUMENT;
    }

    char *search_text = NULL;
    char *replacement_text = NULL;
    int wildcard = 0;
    int start_line = 1;
    int end_line = MAX_LINE;
    char *infile = NULL;
    char *outfile = NULL;

    int opt;
    int l_option_parsed = 0;

    while ((opt = getopt(argc, argv, "s:r:wl:")) != -1)
    {
        switch (opt)
        {
        case 's':
            if (search_text)
            {
               // fprintf(stderr, "DUPLICATE_ARGUMENT\n");
                return DUPLICATE_ARGUMENT;
            }
            search_text = optarg;
            if (optarg == NULL || optarg[0] == '-')
            {
                //fprintf(stderr, "S_ARGUMENT_MISSING\n");
                return S_ARGUMENT_MISSING;
            }
            break;
        case 'r':
            if (replacement_text)
            {
               // fprintf(stderr, "DUPLICATE_ARGUMENT\n");
                return DUPLICATE_ARGUMENT;
            }
            replacement_text = optarg;

            if (optarg == NULL || optarg[0] == '-')
            {
                //fprintf(stderr, "R_ARGUMENT_MISSING\n");
                return R_ARGUMENT_MISSING;
            }
            break;

        case 'w':
            if (wildcard)
            {
               // fprintf(stderr, "DUPLICATE_ARGUMENT\n");
                return DUPLICATE_ARGUMENT;
            }
            wildcard = 1;
            break;

        case 'l':
            if (l_option_parsed)
            {
               // fprintf(stderr, "DUPLICATE_ARGUMENT\n");
                return DUPLICATE_ARGUMENT;
            }
            l_option_parsed = 1;
            char *token = strtok(optarg, ",");
            if (token == NULL)
            {
               // fprintf(stderr, "L_ARGUMENT_INVALID \n");
                return L_ARGUMENT_INVALID;
            }
            start_line = strtol(token, NULL, 10);
            token = strtok(NULL, ",");
            if (token == NULL)
            {
                //fprintf(stderr, "L_ARGUMENT_INVALID \n");
                return L_ARGUMENT_INVALID;
            }
            end_line = strtol(token, NULL, 10);
            if (start_line <= 0 || end_line <= 0 || start_line > end_line)
            {
               // fprintf(stderr, "L_ARGUMENT_INVALID \n");
                return L_ARGUMENT_INVALID;
            }
            break;
        default:
            break;
        }
    }

    if (!search_text)
    {
        //fprintf(stderr, "S_ARGUMENT_MISSING\n");
        return S_ARGUMENT_MISSING;
    }

    if (!replacement_text)
    {
       // fprintf(stderr, "R_ARGUMENT_MISSING\n");
        return R_ARGUMENT_MISSING;
    }

    if (optind + 2 != argc)
    {
       // fprintf(stderr, "MISSING_ARGUMENT\n");
        return MISSING_ARGUMENT;
    }

    int asterisk_count = 0;

    if (wildcard)
    {
        for (const char *c = search_text; *c; c++)
        {
            if (*c == '*')
            {
                asterisk_count++;
            }
        }
        if (asterisk_count != 1)
        {
            //printf("%s\n", search_text);
            //printf("%d", asterisk_count);
         //   fprintf(stderr, "WILDCARD_INVALID\n");
            return WILDCARD_INVALID;
        }
    }

    infile = argv[optind];
    outfile = argv[optind + 1];

    FILE *input = fopen(infile, "r");
    if (input == NULL)
    {
        //fprintf(stderr, "INPUT_FILE_MISSING\n");
        return INPUT_FILE_MISSING;
    }

    FILE *output = fopen(outfile, "w");
    if (output == NULL)
    {
        //fprintf(stderr, "OUTPUT_FILE_UNWRITABLE\n");
        fclose(input);
        return OUTPUT_FILE_UNWRITABLE;
    }

    int counter = 0;
    char line[MAX_LINE];
    char remaining[MAX_LINE];

    int search_start = 0;
    int search_end = 0;

    if (search_text[0] == '*')
    {
        search_start = 1;
        search_text++;
    }
    if (search_text[strlen(search_text) - 1] == '*')
    {
        search_end = 1;
        search_text[strlen(search_text) - 1] = '\0';
    }

    while (fgets(line, sizeof(line), input) != NULL)
    {
        counter++;
        if (counter >= start_line && counter <= end_line)
        {
            const char *position;

            while ((search_start == 1) && (position = endsWithStrStr(line, search_text)) != NULL)
            {
                while (position > line && !isspace(position[-1]) && !ispunct(position[-1]))
                {
                    position--;
                }

                size_t length_before = position - line;
                fwrite(line, 1, length_before, output);
                fwrite(replacement_text, 1, strlen(replacement_text), output);
                while (*position && !isspace(*position) && !ispunct(*position))
                {
                    position++;
                }
                strcpy(remaining, position);
                strcpy(line, remaining);
            }
            while ((search_end == 1) && (position = startsWithStrStr(line, search_text)) != NULL)
            {
                size_t length_before = position - line;
                fwrite(line, 1, length_before, output);
                fwrite(replacement_text, 1, strlen(replacement_text), output);
                while (*position && !isspace(*position) && !ispunct(*position))
                {
                    position++;
                }
                strcpy(remaining, position);
                strcpy(line, remaining);
            }
            while ((search_start == 0) && (search_end == 0) && (position = strstr(line, search_text)) != NULL)
            {
                size_t length_before = position - line;
                fwrite(line, 1, length_before, output);
                fwrite(replacement_text, 1, strlen(replacement_text), output);
                strcpy(remaining, position + strlen(search_text));
                strcpy(line, remaining);
            }
        }
        fputs(line, output);
    }

    fclose(input);
    fclose(output);
    return 0;
}
