/**
 * @file playFairCipher.c
 * @author Thiago Colares 
 * @author Felipe Torquato
 * @brief Esse arquivo apresenta uma implementação clássica do algorítmo Playfair cipher
 * Trabalho da disciplina de Sistemas Embarcados do Instituto Federal de Educação, Ciência e Tecnologia do Ceará
 * 
 * @version 0.1
 * @date 2024-08-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//constante que é utilizada para construir matriz 5x5 tabela chave
#define SIZE 5

//constante que é utilizada para criar o vetor de 8 mil caracteres que armazena o texto a ser criptografado
#define MAX_TEXT_LENGTH 8000

//matriz 5x5 tabela chave
char keyTable[SIZE][SIZE];

//matriz que é responsável por armazenar a posição de caractere do alfabeto na table chave "keyTable"
int charPos[26][2];

/**
 * @brief Essa função gera a tabela chave do algorítmo a partir de uma chave recebida como parâmetro - manipula as variáveis globais char keyTable[SIZE][SIZE] e int charPos[26][2]
 *  
 * @param key 
 * @param keyLength 
 */

void generateKeyTable(char key[], int keyLength) {
    int alphabet[26] = {0};
    int i, j, k = 0;

    for (i = 0; i < keyLength; i++) {
        if (key[i] == 'J') key[i] = 'I';
        if (!alphabet[key[i] - 'A']) {
            alphabet[key[i] - 'A'] = 1;
            keyTable[k / SIZE][k % SIZE] = key[i];
            charPos[key[i] - 'A'][0] = k / SIZE;
            charPos[key[i] - 'A'][1] = k % SIZE;
            k++;
        }
    }

    for (i = 0; i < 26; i++) {
        if (i + 'A' == 'J') continue;
        if (!alphabet[i]) {
            keyTable[k / SIZE][k % SIZE] = i + 'A';
            charPos[i][0] = k / SIZE;
            charPos[i][1] = k % SIZE;
            k++;
        }
    }
}

/**
 * @brief Função que prepara o texto para a criptografia removendo espaços, caractéres especiais, números e por fim torna os caractéres maiúsculos
 * 
 * Recebe o parâmetro text que armazena o texto a ser criptografado e o parâmetro length que é a referência da variável que representa o tamanho da string text
 * 
 * @param text 
 * @param length 
 */
void prepareText(char text[], int *length) {
    int i, j = 0;
    for (i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            text[j] = (toupper(text[i]) == 'J') ? 'I' : toupper(text[i]);
            j++;
        }
    }
    text[j] = '\0';
    *length = j;
    if (*length % 2 != 0) {
        text[(*length)++] = 'X';
        text[*length] = '\0';
    }
}

/**
 * @brief função responsável por criptografar o texto, recebe dois caractéres char por vez e os criptografa de acordo com as suas posições na tabela chave segundo as regras do playfair cipher
 * 
 * @param a 
 * @param b 
 */

void encryptPair(char *a, char *b) {
    int row1 = charPos[*a - 'A'][0];
    int col1 = charPos[*a - 'A'][1];
    int row2 = charPos[*b - 'A'][0];
    int col2 = charPos[*b - 'A'][1];

    if (row1 == row2) {
        *a = keyTable[row1][(col1 + 1) % SIZE];
        *b = keyTable[row2][(col2 + 1) % SIZE];
    } else if (col1 == col2) {
        *a = keyTable[(row1 + 1) % SIZE][col1];
        *b = keyTable[(row2 + 1) % SIZE][col2];
    } else {
        *a = keyTable[row1][col2];
        *b = keyTable[row2][col1];
    }
}

/**
 * @brief Função responsável por chamar a função de criptografia(encryptPair) e passar para ela pares de caractéres da string texto
 *  Recebe o parâmetro text que armazena o texto a ser criptografado e o parâmetro length que se refere ao tamanhão da string text
 * 
 * @param text 
 * @param length 
 */

void encrypt(char text[], int length) {
    for (int i = 0; i < length; i += 2) {
        encryptPair(&text[i], &text[i + 1]);
    }
}

/**
 * @brief função principal do programa que chama as demais funções
 * 
 * @return int 
 */
int main() {
    //vetor que armazena a palavra de segurança utilizada pra gerar a tabela chave
    char key[] = "SECURITY";
    //vetor que armazena o texto a ser criptografado
    char text[MAX_TEXT_LENGTH];

    printf("Enter the text to encrypt (max 8000 characters): ");
    fgets(text, MAX_TEXT_LENGTH, stdin);

    //variável que armazena o tamanho do texto
    int textLength;
    prepareText(text, &textLength);

    generateKeyTable(key, strlen(key));

    printf("Key Table:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", keyTable[i][j]);
        }
        printf("\n");
    }

    printf("\nChar Position in Key Table:\n");
    for (int i = 0; i < 26; i++) {
        if (i + 'A' != 'J') {
            printf("%c: (%d, %d)\n", i + 'A', charPos[i][0], charPos[i][1]);
        }
    }

    

    printf("Prepared text: %s\n", text);

    encrypt(text, textLength);
    printf("Encrypted text: %s\n", text);

    return 0;
}
