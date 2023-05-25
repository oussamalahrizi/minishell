#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char type;
	char *value;
}	Token;

typedef struct {
    char* command;
    char** arguments;
    char** input_files;
    char** output_files;
    char** file_types;
    int num_arguments;
    int num_input_files;
    int num_output_files;
} ParsedCommand;

ParsedCommand parse_tokens(Token* tokens) {
    ParsedCommand command;
    command.command = NULL;
    command.arguments = NULL;
    command.input_files = NULL;
    command.output_files = NULL;
    command.file_types = NULL;
    command.num_arguments = 0;
    command.num_input_files = 0;
    command.num_output_files = 0;

    int arg_index = 0;
    int input_index = 0;
    int output_index = 0;
    char* current_file_type = NULL;

    for (int i = 0; tokens[i].type != '\0'; i++) {
        Token token = tokens[i];

        if (token.type == 's') {
            if (command.command == NULL) {
                command.command = strdup(token.value);
            } else {
                command.arguments = realloc(command.arguments, sizeof(char*) * (arg_index + 1));
                command.arguments[arg_index++] = strdup(token.value);
                command.num_arguments++;
            }
        } else if (token.type == '<') {
            current_file_type = "redirection";
            command.input_files = realloc(command.input_files, sizeof(char*) * (input_index + 1));
            command.file_types = realloc(command.file_types, sizeof(char*) * (input_index + 1));
            command.input_files[input_index] = strdup(tokens[i + 1].value);
            command.file_types[input_index++] = strdup(current_file_type);
            command.num_input_files++;
            i++;  // Skip the next token as it contains the file name
        } else if (token.type == '>') {
            current_file_type = "redirection";
            if (tokens[i + 1].type == '>') {
                current_file_type = "append";
                i++;  // Skip the next token as it is part of the append operator
            }
            command.output_files = realloc(command.output_files, sizeof(char*) * (output_index + 1));
            command.file_types = realloc(command.file_types, sizeof(char*) * (output_index + 1));
            command.output_files[output_index] = strdup(tokens[i + 1].value);
            command.file_types[output_index++] = strdup(current_file_type);
            command.num_output_files++;
            i++;  // Skip the next token as it contains the file name
        }
    }

    return command;
}

void free_parsed_command(ParsedCommand command) {
    free(command.command);

    for (int i = 0; i < command.num_arguments; i++) {
        free(command.arguments[i]);
    }
    free(command.arguments);

    for (int i = 0; i < command.num_input_files; i++) {
        free(command.input_files[i]);
        free(command.file_types[i]);
    }
    free(command.input_files);
    free(command.file_types);

    for (int i = 0; i < command.num_output_files; i++) {
        free(command.output_files[i]);
        free(command.file_types[i]);
    }
    free(command.output_files);
    free(command.file_types);
}

int main() {
    Token tokens[] = {
        {'s', "cat"},
        {'<', "<"},
        {'s', "Makefile"},
        {'<', "<"},
        {'s', "test"},
        {'<', "<"},
        {'s', "haha"},
        {'|', "|"},
        {'s', "wc"},
        {'s', "-l"},
        {'>', ">"},
        {'s', "out"},
        {'>', ">"},
        {'s', "output.txt"},
        {'\0', NULL}  // End of tokens
    };

    ParsedCommand command = parse_tokens(tokens);

    printf("Command: %s\n", command.command);

    printf("Arguments:\n");
    for (int i = 0; i < command.num_arguments; i++) {
        printf("%s\n", command.arguments[i]);
    }

    printf("Input Files:\n");
    for (int i = 0; i < command.num_input_files; i++) {
        printf("%s (%s)\n", command.input_files[i], command.file_types[i]);
    }

    printf("Output Files:\n");
    for (int i = 0; i < command.num_output_files; i++) {
        printf("%s (%s)\n", command.output_files[i], command.file_types[i]);
    }

    free_parsed_command(command);

    return 0;
}
