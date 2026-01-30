#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE_LENGTH 1024

int main() {
    double num1, num2, result;
    char operation[16];
    char line[MAX_LINE_LENGTH];

    printf("Enter calculation (e.g., 3.5 + 2.1 or sin 0.5): ");
    if (fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = 0;

        if (sscanf(line, "%15s %lf", operation, &num1) == 2) {
            if (strcmp(operation, "sin") == 0) result = sin(num1);
            else if (strcmp(operation, "cos") == 0) result = cos(num1);
            else if (strcmp(operation, "tan") == 0) result = tan(num1);
            else if (strcmp(operation, "log") == 0) {
                if (num1 <= 0) { printf("Error: Logarithm of non-positive number.\n"); return 1; }
                result = log10(num1);
            }
            else if (strcmp(operation, "ln") == 0) {
                if (num1 <= 0) { printf("Error: Natural logarithm of non-positive number.\n"); return 1; }
                result = log(num1);
            }
            else if (strcmp(operation, "exp") == 0) result = exp(num1);
            else if (strcmp(operation, "sqrt") == 0) {
                if (num1 < 0) { printf("Error: Square root of negative number.\n"); return 1; }
                result = sqrt(num1);
            }
            else {
                printf("Unsupported unary operation: %s\n", operation);
                return 1;
            }
            printf("Result: %.6f\n", result);
        }
        else if (sscanf(line, "%lf %15s %lf", &num1, operation, &num2) == 3) {
            if (strcmp(operation, "+") == 0) result = num1 + num2;
            else if (strcmp(operation, "-") == 0) result = num1 - num2;
            else if (strcmp(operation, "*") == 0) result = num1 * num2;
            else if (strcmp(operation, "/") == 0) {
                if (num2 == 0) { printf("Error: Division by zero.\n"); return 1; }
                result = num1 / num2;
            }
            else if (strcmp(operation, "^") == 0) result = pow(num1, num2);
            else { printf("Unsupported operation: %s\n", operation); return 1; }
            printf("Result: %.6f\n", result);
        } else {
            printf("Invalid input format.\n");
            return 1;
        }
    } else {
        printf("Error reading input.\n");
        return 1;
    }
    return 0;
}
