#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int determinant2x2(int a, int b, int c, int d) {
    return a * d - b * c;
}

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

void hillEncrypt(int key[2][2], char *message, int len, char *encryptedMessage) {
    for (int i = 0; i < len; i += 2) {
        int p1, p2;

        if (isupper(message[i])) {
            p1 = message[i] - 'A';
        } else if (islower(message[i])) {
            p1 = message[i] - 'a';
        }

        if (i + 1 < len) {
            if (isupper(message[i + 1])) {
                p2 = message[i + 1] - 'A';
            } else if (islower(message[i + 1])) {
                p2 = message[i + 1] - 'a';
            }
        } else {
            p2 = 0;
        }


        encryptedMessage[i] = (key[0][0] * p1 + key[0][1] * p2) % 26;
        encryptedMessage[i + 1] = (key[1][0] * p1 + key[1][1] * p2) % 26;

        if (isupper(message[i])) {
            encryptedMessage[i] += 'A';
        } else if (islower(message[i])) {
            encryptedMessage[i] += 'a';
        }

        if (i + 1 < len) {
            if (isupper(message[i + 1])) {
                encryptedMessage[i + 1] += 'A';
            } else if (islower(message[i + 1])) {
                encryptedMessage[i + 1] += 'a';
            }
        } else {
            encryptedMessage[i + 1] += 'a';
        }
    }
    encryptedMessage[len] = '\0';
}

void hillDecrypt(int key[2][2], char *encryptedMessage, int len, char *decryptedMessage) {
    int det = determinant2x2(key[0][0], key[0][1], key[1][0], key[1][1]);
    int detInv = modInverse(det, 26);

    if (detInv == -1) {
        printf("Decryption not possible. The key is not invertible.\n");
        return;
    }

    int invKey[2][2];
    invKey[0][0] = (key[1][1] * detInv) % 26;
    invKey[0][1] = (-key[0][1] * detInv) % 26;
    invKey[1][0] = (-key[1][0] * detInv) % 26;
    invKey[1][1] = (key[0][0] * detInv) % 26;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (invKey[i][j] < 0) {
                invKey[i][j] += 26;
            }
        }
    }

    for (int i = 0; i < len; i += 2) {
        int c1, c2;

        if (isupper(encryptedMessage[i])) {
            c1 = encryptedMessage[i] - 'A';
        } else if (islower(encryptedMessage[i])) {
            c1 = encryptedMessage[i] - 'a';
        }

        if (i + 1 < len) {
            if (isupper(encryptedMessage[i + 1])) {
                c2 = encryptedMessage[i + 1] - 'A';
            } else if (islower(encryptedMessage[i + 1])) {
                c2 = encryptedMessage[i + 1] - 'a';
            }
        } else {
            c2 = 0;
        }

        // Decrypt the pair
        decryptedMessage[i] = (invKey[0][0] * c1 + invKey[0][1] * c2) % 26;
        decryptedMessage[i + 1] = (invKey[1][0] * c1 + invKey[1][1] * c2) % 26;

        // Convert back to uppercase if the original character was uppercase
        if (isupper(encryptedMessage[i])) {
            decryptedMessage[i] += 'A';
        } else if (islower(encryptedMessage[i])) {
            decryptedMessage[i] += 'a';
        }

        if (i + 1 < len) {
            if (isupper(encryptedMessage[i + 1])) {
                decryptedMessage[i + 1] += 'A';
            } else if (islower(encryptedMessage[i + 1])) {
                decryptedMessage[i + 1] += 'a';
            }
        } else {
            decryptedMessage[i + 1] += 'a';
        }
    }
    decryptedMessage[len] = '\0';
}

int main() {
    int key[2][2];
    char message[100];
    char encryptedMessage[100];
    char decryptedMessage[100];
    int len;

    printf("Enter the 2x2 key matrix (4 integers):\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            scanf("%d", &key[i][j]);
        }
    }

    printf("Enter the message to encrypt: ");
    scanf("%s", message);

    len = strlen(message);
    if (len % 2 != 0) {
        message[len] = 'a';
        message[len + 1] = '\0';
        len++;
    }

    hillEncrypt(key, message, len, encryptedMessage);
    printf("Encrypted message: %s\n", encryptedMessage);

    hillDecrypt(key, encryptedMessage, len, decryptedMessage);
    printf("Decrypted message: %s\n", decryptedMessage);

    return 0;
}
