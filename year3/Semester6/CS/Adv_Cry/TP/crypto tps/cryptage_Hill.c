#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Function to calculate determinant for 2x2 matrix
int determinant2x2(int a, int b, int c, int d) {
    return a * d - b * c;
}

// Function to calculate determinant for NxN matrix
int determinantNxN(int **matrix, int size) {
    if (size == 1) return matrix[0][0];
    if (size == 2) return determinant2x2(matrix[0][0], matrix[0][1], matrix[1][0], matrix[1][1]);

    int det = 0;
    for (int col = 0; col < size; col++) {
        // Allocate minor matrix
        int **minor = (int **)malloc((size-1) * sizeof(int *));
        for (int i = 0; i < size-1; i++) {
            minor[i] = (int *)malloc((size-1) * sizeof(int));
        }

        // Fill minor matrix
        for (int i = 1; i < size; i++) {
            int minor_col = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                minor[i-1][minor_col++] = matrix[i][j];
            }
        }

        // Calculate cofactor and add to determinant
        int sign = (col % 2 == 0) ? 1 : -1;
        det += sign * matrix[0][col] * determinantNxN(minor, size-1);

        // Free minor matrix
        for (int i = 0; i < size-1; i++) {
            free(minor[i]);
        }
        free(minor);
    }

    return det;
}

// Function to calculate modular inverse
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

// Function to get cofactor matrix
void getCofactor(int **matrix, int **temp, int p, int q, int size) {
    int i = 0, j = 0;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (row != p && col != q) {
                temp[i][j++] = matrix[row][col];
                if (j == size - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

// Function to calculate adjugate matrix
void adjugateMatrix(int **matrix, int **adj, int size) {
    if (size == 1) {
        adj[0][0] = 1;
        return;
    }

    int sign = 1;
    int **temp = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        temp[i] = (int *)malloc(size * sizeof(int));
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            getCofactor(matrix, temp, i, j, size);
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            adj[j][i] = sign * determinantNxN(temp, size-1);
        }
    }

    for (int i = 0; i < size; i++) {
        free(temp[i]);
    }
    free(temp);
}

// Function to calculate inverse matrix
int inverseMatrix(int **matrix, float **inverse, int size) {
    int det = determinantNxN(matrix, size);
    if (det == 0) {
        return -1; // Matrix is not invertible
    }

    int detInv = modInverse(det, 26);
    if (detInv == -1) {
        return -1; // Modular inverse doesn't exist
    }

    int **adj = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        adj[i] = (int *)malloc(size * sizeof(int));
    }

    adjugateMatrix(matrix, adj, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            inverse[i][j] = (adj[i][j] * detInv) % 26;
            if (inverse[i][j] < 0) {
                inverse[i][j] += 26;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        free(adj[i]);
    }
    free(adj);

    return 0;
}

// Function to encrypt message using Hill cipher
void hillEncrypt(int **key, char *message, int len, char *encryptedMessage, int size) {
    for (int i = 0; i < len; i += size) {
        int *plaintext = (int *)malloc(size * sizeof(int));
        int *ciphertext = (int *)malloc(size * sizeof(int));

        // Convert characters to numbers
        for (int j = 0; j < size; j++) {
            if (i + j < len) {
                if (isupper(message[i + j])) {
                    plaintext[j] = message[i + j] - 'A';
                } else if (islower(message[i + j])) {
                    plaintext[j] = message[i + j] - 'a';
                }
            } else {
                plaintext[j] = 0; // Padding with 'a'
            }
        }

        // Matrix multiplication
        for (int row = 0; row < size; row++) {
            ciphertext[row] = 0;
            for (int col = 0; col < size; col++) {
                ciphertext[row] += key[row][col] * plaintext[col];
            }
            ciphertext[row] %= 26;

            // Convert back to character
            if (i + row < len) {
                if (isupper(message[i + row])) {
                    encryptedMessage[i + row] = ciphertext[row] + 'A';
                } else {
                    encryptedMessage[i + row] = ciphertext[row] + 'a';
                }
            } else {
                encryptedMessage[i + row] = ciphertext[row] + 'a';
            }
        }

        free(plaintext);
        free(ciphertext);
    }
    encryptedMessage[len] = '\0';
}

// Function to decrypt message using Hill cipher
void hillDecrypt(int **key, char *encryptedMessage, int len, char *decryptedMessage, int size) {
    // Calculate inverse matrix
    float **invKey = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; i++) {
        invKey[i] = (float *)malloc(size * sizeof(float));
    }

    if (inverseMatrix(key, invKey, size) == -1) {
        printf("Decryption not possible. The key is not invertible.\n");
        for (int i = 0; i < size; i++) {
            free(invKey[i]);
        }
        free(invKey);
        return;
    }

    for (int i = 0; i < len; i += size) {
        int *ciphertext = (int *)malloc(size * sizeof(int));
        int *plaintext = (int *)malloc(size * sizeof(int));

        // Convert characters to numbers
        for (int j = 0; j < size; j++) {
            if (i + j < len) {
                if (isupper(encryptedMessage[i + j])) {
                    ciphertext[j] = encryptedMessage[i + j] - 'A';
                } else if (islower(encryptedMessage[i + j])) {
                    ciphertext[j] = encryptedMessage[i + j] - 'a';
                }
            } else {
                ciphertext[j] = 0;
            }
        }

        // Matrix multiplication with inverse key
        for (int row = 0; row < size; row++) {
            plaintext[row] = 0;
            for (int col = 0; col < size; col++) {
                plaintext[row] += (int)(invKey[row][col] * ciphertext[col]);
            }
            plaintext[row] %= 26;
            if (plaintext[row] < 0) {
                plaintext[row] += 26;
            }

            // Convert back to character
            if (i + row < len) {
                if (isupper(encryptedMessage[i + row])) {
                    decryptedMessage[i + row] = plaintext[row] + 'A';
                } else {
                    decryptedMessage[i + row] = plaintext[row] + 'a';
                }
            } else {
                decryptedMessage[i + row] = plaintext[row] + 'a';
            }
        }

        free(ciphertext);
        free(plaintext);
    }
    decryptedMessage[len] = '\0';

    for (int i = 0; i < size; i++) {
        free(invKey[i]);
    }
    free(invKey);
}

// Function to allocate a matrix
int **allocateMatrix(int size) {
    int **matrix = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        matrix[i] = (int *)malloc(size * sizeof(int));
    }
    return matrix;
}

// Function to free a matrix
void freeMatrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int size;
    printf("Select key matrix size:\n");
    printf("1. 2x2\n");
    printf("2. 3x3\n");
    printf("3. 4x4\n");
    printf("4. 5x5\n");
    printf("Enter your choice (1-4): ");
    scanf("%d", &size);

    if (size < 1 || size > 4) {
        printf("Invalid choice.\n");
        return 1;
    }

    // Map choice to actual size
    size += 1; // 1->2, 2->3, etc.

    // Allocate key matrix
    int **key = allocateMatrix(size);

    printf("Enter the %dx%d key matrix (%d integers):\n", size, size, size*size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%d", &key[i][j]);
        }
    }

    char message[1000];
    char encryptedMessage[1000];
    char decryptedMessage[1000];

    printf("Enter the message to encrypt: ");
    scanf(" %[^\n]s", message);

    int len = strlen(message);
    // Pad the message if needed
    if (len % size != 0) {
        int pad = size - (len % size);
        for (int i = 0; i < pad; i++) {
            message[len + i] = 'a';
        }
        message[len + pad] = '\0';
        len += pad;
    }

    hillEncrypt(key, message, len, encryptedMessage, size);
    printf("Encrypted message: %s\n", encryptedMessage);

    hillDecrypt(key, encryptedMessage, len, decryptedMessage, size);
    printf("Decrypted message: %s\n", decryptedMessage);

    freeMatrix(key, size);

    return 0;
}
