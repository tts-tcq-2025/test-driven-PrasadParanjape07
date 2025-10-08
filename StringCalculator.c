#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "string_calculator.h"

// Global exception variable
CalculatorException calculator_exception = {0};

// Helper function to check if a character is a delimiter
int is_delimiter(char c, const char* delimiters) {
    return strchr(delimiters, c) != NULL;
}

// Helper function to extract custom delimiter from input
char* extract_delimiter(const char* input, char* delimiter_buffer) {
    if (input[0] == '/' && input[1] == '/') {
        const char* start = input + 2;
        const char* newline = strchr(start, '\n');
        
        if (newline) {
            // Check if it's the format //[delimiter]\n
            if (start[0] == '[') {
                const char* closing_bracket = strchr(start + 1, ']');
                if (closing_bracket && closing_bracket < newline) {
                    int len = closing_bracket - (start + 1);
                    strncpy(delimiter_buffer, start + 1, len);
                    delimiter_buffer[len] = '\0';
                    return (char*)(newline + 1);
                }
            } else {
                // Simple format //;\n
                delimiter_buffer[0] = start[0];
                delimiter_buffer[1] = '\0';
                return (char*)(newline + 1);
            }
        }
    }
    
    // Default delimiters
    strcpy(delimiter_buffer, ",\n");
    return (char*)input;
}

// Helper function to parse numbers and calculate sum
int parse_and_sum(const char* numbers, const char* delimiters) {
    if (!numbers || strlen(numbers) == 0) {
        return 0;
    }
    
    int sum = 0;
    char* numbers_copy = malloc(strlen(numbers) + 1);
    strcpy(numbers_copy, numbers);
    
    char negatives[256] = {0};
    int has_negatives = 0;
    
    char* token = strtok(numbers_copy, delimiters);
    
    while (token != NULL) {
        // Skip empty tokens
        if (strlen(token) == 0) {
            token = strtok(NULL, delimiters);
            continue;
        }
        
        int num = atoi(token);
        
        // Check for negative numbers
        if (num < 0) {
            if (has_negatives) {
                strcat(negatives, ",");
            }
            char neg_str[20];
            sprintf(neg_str, "%d", num);
            strcat(negatives, neg_str);
            has_negatives = 1;
        } else if (num <= 1000) {
            // Numbers bigger than 1000 should be ignored
            sum += num;
        }
        
        token = strtok(NULL, delimiters);
    }
    
    free(numbers_copy);
    
    // Handle negative numbers exception
    if (has_negatives) {
        calculator_exception.has_error = 1;
        sprintf(calculator_exception.message, "negatives not allowed: %s", negatives);
        return -1;
    }
    
    return sum;
}

// Main Add function
int Add(const char* numbers) {
    // Reset exception
    calculator_exception.has_error = 0;
    memset(calculator_exception.message, 0, sizeof(calculator_exception.message));
    
    if (!numbers) {
        return 0;
    }
    
    char delimiter_buffer[256];
    const char* numbers_to_parse = extract_delimiter(numbers, delimiter_buffer);
    
    return parse_and_sum(numbers_to_parse, delimiter_buffer);
}

