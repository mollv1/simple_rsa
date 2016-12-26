#include "simple_rsa.h"

unsigned p, q;

void clearBuffer();

int main(void)
{
    unsigned choice = 0;
    unsigned encryptMessage[MAX_MESSAGE], decryptMessage[MAX_MESSAGE];
    unsigned char message[MAX_MESSAGE];
    KeyPair keyPairs[MAX_EXP_KEYS];


    do{
    printf("Please enter 2 primenumbers sepparated by a space.\nThey must be bigger than 10 and cant be equal: ");
    scanf("%d %d", &p, &q);
    }while(!((isPrime(p) && isPrime(q)) && p != q && (p>=11 && q>=11)));

    clearBuffer();
    printf("\n#################################################\n");
    generateKeys(p,q, keyPairs);
    printf("# choice:\t# public Key:\t# private Key: \t#\n");
    for (unsigned index = 0; index < MAX_EXP_KEYS; index++){
        printf("#  %d.\t\t#  %d\t\t#  %d \t\t#\n", index +1,
               keyPairs[index].publicKey,
               keyPairs[index].privateKey);
    }
    printf("#################################################\n");

    puts("Which key pair do you want?");
    while(choice < 1 || choice > MAX_EXP_KEYS) {
        scanf("%d", &choice);
        clearBuffer();
        if(choice < 1 || choice > MAX_EXP_KEYS)
            printf("You entered a wrong input, enter a number between 1 and %d\n",MAX_EXP_KEYS);
    }

    printf("\nYour public key is:\t%d\n", keyPairs[choice-1].publicKey);
    printf("Your private key is:\t%d\n", keyPairs[choice-1].privateKey);

    puts("\nEnter your message to encrypt");
    fgets((char*)message,MAX_MESSAGE,stdin);
    unsigned long primeProduct = p*q;

    for(unsigned index = 0; index < MAX_MESSAGE && message[index] != '\0';index++) {
        if(message[index] == '\n')
            message[index] = '\0';
        encryptMessage[index] = cryptCharacter(keyPairs[choice-1].publicKey,
                primeProduct,message[index]);
        message[index] = encryptMessage[index];
    }
    puts("\nYour encrypted messag is:");
    printf("%s\n",message);

    for(unsigned index = 0; index < MAX_MESSAGE && message[index] != '\0';index++){
        decryptMessage[index] = cryptCharacter(keyPairs[choice-1].privateKey,
                primeProduct, encryptMessage[index]);
        message[index] = decryptMessage[index];
    }
    puts("\nYour decrypted message is:");
    printf("%s\n\n",message);

    return 0;
}


void clearBuffer()
{
    char c;
    while((c = getchar()) != '\n' && c != EOF);
}
