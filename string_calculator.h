#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

// StringCalculator function declaration
int Add(const char* numbers);

// Exception structure for negative numbers
typedef struct {
    char message[256];
    int has_error;
} CalculatorException;

// Global exception variable
extern CalculatorException calculator_exception;

#endif // STRING_CALCULATOR_H
