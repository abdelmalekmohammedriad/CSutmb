#include <stdio.h>
#include <string.h>

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

char encryptChar(char ch, int a, int b) {
    if (ch >= 'A' && ch <= 'Z') {
        return (char)((a * (ch - 'A') + b) % 26 + 'A');
    } else if (ch >= 'a' && ch <= 'z') {
        return (char)((a * (ch - 'a') + b) % 26 + 'a');
    }
    return ch;
}

char decryptChar(char ch, int a, int b) {
    int a_inv = modInverse(a, 26);
    if (a_inv == -1) {
        printf("Error: 'a' has no modular inverse modulo 26.\n");
        return ch;
    }

    b = b % 26;

    if (ch >= 'A' && ch <= 'Z') {
        int y = ch - 'A';
        int x = (a_inv * (y - b + 26)) % 26;
        return (char)(x + 'A');
    } else if (ch >= 'a' && ch <= 'z') {
        int y = ch - 'a';
        int x = (a_inv * (y - b + 26)) % 26;
        return (char)(x + 'a');
    }

    return ch;
}

void encryptString(char *str, int a, int b) {
    for (int i = 0; i < strlen(str); i++) {
        str[i] = encryptChar(str[i], a, b);
    }
}

void decryptString(char *str, int a, int b) {
    for (int i = 0; i < strlen(str); i++) {
        str[i] = decryptChar(str[i], a, b);
    }
}

int main() {
    char str[100];
    int a, b;

    printf("Enter the message: ");
    scanf(" %[^\n]", str);

    printf("Enter the value of key a: ");
    scanf("%d", &a);

    printf("Enter the value of key b: ");
    scanf("%d", &b);

    if (a==1 && b!=13){
        printf("this is ceasar encription and decryption.\n");
    }
    if(a==1 && b==13){
        printf("this is RO13 encription and decryption.\n");
    }
    if (modInverse(a, 26) == -1) {
        printf("Error: 'a' must be coprime with 26 for the Affine cipher to work.\n");
        return 1;
    }


    printf("Original: %s\n", str);

    encryptString(str, a, b);
    printf("Encrypted: %s\n", str);

    decryptString(str, a, b);
    printf("Decrypted: %s\n", str);

    return 0;
}
