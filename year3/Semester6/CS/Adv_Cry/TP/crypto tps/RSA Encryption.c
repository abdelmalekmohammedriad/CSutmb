#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to check if a number is prime
bool is_prime(int num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
    }
    return true;
}

// Function to calculate GCD
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// Function to find first 10 valid e values
void find_valid_e_values(int phi, int e_values[]) {
    int count = 0;
    int current = 2; // Start checking from 2

    while (count < 10) {
        if (is_prime(current) && gcd(phi, current) == 1 && current < phi) {
            e_values[count] = current;
            count++;
        }
        current++;
    }
}

// Function to calculate modular inverse
int mod_inverse(int e, int phi) {
    e = e % phi; // Ensure e is within bounds
    for (int d = 1; d < phi; d++) {
        if ((e * d) % phi == 1) {
            return d;
        }
    }
    return -1; // Should never happen if e and phi are coprime
}

// Function to get character position (case-sensitive) or number value
int get_char_position(char ch) {
    if (isupper(ch)) {
        return ch - 'A' + 1;
    } else if (islower(ch)) {
        return ch - 'a' + 1;
    } else if (isdigit(ch)) {
        return ch - '0' + 100;
    }
    return 0;  // For other characters
}

// Function for modular exponentiation
int mod_exp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}
/*
3^5 % 7

donner initiale

result = 1
base = 3 % 7 = 3
mod=7
exp = 5

loop1:

result = 3
exp = 2
base = 3 * 3 % 7 = 9 % 7= 2

loop2:

result = 3
exp = 1
base = 2*2  % 7 = 4%7=4

loop3:

result = 12%7=5
exp = 1/2=0
base = 4*4 % 7 = 2


*/

void decrypt_message(int encrypted[], int length, int d, int n) {
    printf("\nDecryption Process:\n");
    printf("Cp | D = Cp^%d mod %d | Char\n", d, n);
    printf("---+------------------+-----\n");

    for (int i = 0; i < length; i++) {
        if (encrypted[i] >= 0) {
            int Cp = encrypted[i];
            int D = mod_exp(Cp, d, n);
            char original_char;

            // Convert back to character
            if (D >= 1 && D <= 26) {
                original_char = 'A' + D - 1;  // Uppercase letters
            } else if (D >= 101 && D <= 109) {
                original_char = '0' + (D - 100);  // Numbers
            } else {
                original_char = '?';  // Unknown
            }

            printf("%4d|%8d        |  %c\n", Cp, D, original_char);
        }
    }
}




int main() {
    int p, q;
    int e_values[10];

    // Get prime numbers from user
    printf("Enter two prime numbers for p and q:\n");
    printf("Enter first prime number (p): ");
    scanf("%d", &p);
    while (!is_prime(p)) {
        printf("%d is not prime. Enter a prime number: ", p);
        scanf("%d", &p);
    }

    printf("Enter second prime number (q): ");
    scanf("%d", &q);
    while (!is_prime(q) || q == p) {
        if (q == p) {
            printf("%d is the same as p. ", q);
        } else {
            printf("%d is not prime. ", q);
        }
        printf("Enter a different prime number: ");
        scanf("%d", &q);
    }

    // Calculate n and phi
    int n = p * q;
    int phi = (p - 1) * (q - 1);

    printf("\nCalculated values:\n");
    printf("n = p * q = %d\n", n);
    printf("phi(n) = (p-1)*(q-1) = %d\n", phi);

    // Find valid e values
    find_valid_e_values(phi, e_values);

    // Let user choose e
    int e, choice;
    printf("\nChoose your e value:\n");
    for (int i = 0; i < 10; i++) {
        printf("%2d. e = %d\n", i+1, e_values[i]);
    }
    printf("11. Enter custom e value\n");
    printf("Your choice (1-11): ");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= 10) {
        e = e_values[choice-1];
    }
    else if (choice == 11) {
        while (1) {
            printf("Enter your custom e value (must be 1 < e < %d and coprime with phi(n)): ", phi);
            scanf("%d", &e);

            if (e > 1 && e < phi && gcd(phi, e) == 1) {
                break;
            }
            printf("Invalid e! Must satisfy:\n");
            printf("1 < e < phi(n) [phi(n) = %d]\n", phi);
            printf("gcd(e, phi(n)) = 1\n\n");
        }
    }
    else {
        printf("Invalid choice!\n");
        exit(1);
    }

    printf("\nValid public exponent selected: e = %d\n", e);

    // Calculate private key d
    int d = mod_inverse(e, phi);
    if (d == -1) {
        printf("Error: No inverse exists! e and phi(n) must be coprime.\n");
        return 1;
    }

    printf("\nPrivate key (d, n) = (%d, %d)\n", d, n);
    printf("Verification: (e*d) mod phi(n) = %d\n", (e * d) % phi);

    /* Message Encryption */
    char message[256];
    int encrypted[256];

    // Clear input buffer
    while (getchar() != '\n');

    printf("\nEnter message to encrypt (letters and numbers only): ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; // Remove newline

    // Encrypt each character
    printf("\nEncryption Process:\n");
    printf("Char | Position (p) | c = p^%d mod %d\n", e, n);
    printf("-----+--------------+----------------\n");

    int msg_length = strlen(message);
    for (int i = 0; i < msg_length; i++) {
        if (isalnum(message[i])) {
            int p = get_char_position(message[i]);
            int c = mod_exp(p, e, n);
            encrypted[i] = c;

            printf("  %c  |      %2d      |     %3d\n",
                  message[i], p, c);
        } else {
            encrypted[i] = -1; // Mark non-alphanumeric characters
            printf("  %c  |    (skip)    |    (not encrypted)\n", message[i]);
        }
    }
    encrypted[msg_length] = -2;

    // Display final encrypted message
    printf("\nFinal Encrypted Message:\n");
    for (int i = 0; i < strlen(message); i++) {
        if (encrypted[i] != -1) {
            printf("%d ", encrypted[i]);
        }
    }
    printf("\n");

printf("\nWould you like to decrypt the message? (1=Yes, 0=No): ");
    int choice1;
    scanf("%d", &choice1);

    if (choice1 == 1) {
        // Count the number of encrypted values
        int encrypted_length = 0;
         while (encrypted[encrypted_length] != -2 && encrypted_length < 256) {
            encrypted_length++;
        }

        decrypt_message(encrypted, encrypted_length, d, n);
    }

    return 0;
}
