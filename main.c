#include <stdio.h>

#define MAX_EXP_KEYS 20
#define MAX_MESSAGE 1000
typedef struct KeyPair{
    unsigned privateKey;
    unsigned publicKey;
}KeyPair;

void generateKeys(unsigned prime1, unsigned prime2, KeyPair *keyPairs_p);

unsigned cryptCharacter(unsigned key, unsigned primeProduct, unsigned character);
unsigned nextPublicKey(unsigned publicKey, unsigned coPrimeProduct);
unsigned privateKey(unsigned publicKey, unsigned coPrimeProduct);

void clearBuffer();

int main(void)
{
    unsigned choice = 0;
    unsigned encryptMessage[MAX_MESSAGE], decryptMessage[MAX_MESSAGE];
    unsigned char message[MAX_MESSAGE];
    KeyPair keyPairs[MAX_EXP_KEYS];
    generateKeys(991,997, keyPairs);
    printf("choice:\tpublic Key:\tprivate Key:\n");
    for (unsigned index = 0; index < MAX_EXP_KEYS; index++){
        printf("%d.\t%d\t\t%d\n", index +1,
               keyPairs[index].publicKey,
               keyPairs[index].privateKey);
    }

    puts("Which key pair do you want?");
    while(choice < 1 || choice > MAX_EXP_KEYS) {
        scanf("%d", &choice);
        clearBuffer();
        if(choice < 1 || choice > MAX_EXP_KEYS)
            printf("You entered a wrong input, enter a number between 1 and %d\n",MAX_EXP_KEYS);
    }

    printf("Your public key %d\n", keyPairs[choice-1].publicKey);
    printf("Your private key %d\n", keyPairs[choice-1].privateKey);

    puts("Enter your message to encrypt");
    fgets((char*)message,MAX_MESSAGE,stdin);
    unsigned long primeProduct = 991*997;

    for(unsigned index = 0; index < MAX_MESSAGE && message[index] != '\0';index++) {
        if(message[index] == '\n')
            message[index] = '\0';
        encryptMessage[index] = cryptCharacter(keyPairs[choice-1].publicKey,
                primeProduct,message[index]);
        message[index] = encryptMessage[index];
    }
    printf("%s\n",message);

    for(unsigned index = 0; index < MAX_MESSAGE && message[index] != '\0';index++){
        decryptMessage[index] = cryptCharacter(keyPairs[choice-1].privateKey,
                primeProduct, encryptMessage[index]);
        message[index] = decryptMessage[index];
    }
    printf("%s\n",message);

    return 0;
}

/**
 * @brief writes generated private/public pairs into keyPairArray_p
 * @param prime1 First Prime used to generate keypairs
 * @param prime2 Second Prime used to generate keypairs
 * @param keyPairArray_p Pointer to array where keypairs are written into
 */
void generateKeys(unsigned prime1, unsigned prime2, KeyPair *keyPairs_p)
{
    unsigned publicKey = 1;
    unsigned coPrimeProduct = (prime1 - 1) * (prime2 -1);
    //find key pairs till Array is full
    for (unsigned index = 0; index < MAX_EXP_KEYS; index++) {
        publicKey = nextPublicKey(publicKey, coPrimeProduct);
        keyPairs_p->publicKey = publicKey;
        keyPairs_p->privateKey = privateKey(publicKey, coPrimeProduct);
        keyPairs_p++;
    }
}
/**
 * @brief find next publicKey, which share no factor with coPrimeProduct
 * @param publicKey
 * @param coPrimeProduct
 */
unsigned nextPublicKey(unsigned publicKey, unsigned coPrimeProduct)
{
    unsigned nextKey = publicKey;
    int gcd=0;
    do {
        //only check publicKey >= 3 and uneven values, because coPrimeProduct is allways even
        nextKey += 2;
        unsigned check = coPrimeProduct;

        //find gcd from possible public key and coPrime
        gcd = nextKey;
        while (check != 0) {
            unsigned interimResult = gcd % check;
            gcd = check;
            check = interimResult;
        }
    } while(gcd != 1);
    return  nextKey;
}

/**
 * @brief calculate privateKey from publicKey with euclidean algorithm
 * @param publicKey
 * @param coPrimeProduct
 */
unsigned privateKey(unsigned publicKey, unsigned coPrimeProduct)
{
    int calcPrivateKey[2][2]= {{coPrimeProduct, coPrimeProduct},{publicKey, 1}};

    //find private Key with the extended euclidean algorithm
    while (calcPrivateKey[1][0] != 1) {
        unsigned tempResult1 = calcPrivateKey[0][0] % calcPrivateKey[1][0];
        int tempResult2 = calcPrivateKey[0][1] - calcPrivateKey[1][1]*(calcPrivateKey[0][0]/calcPrivateKey[1][0]);
        while (tempResult2 < 0)
            tempResult2 += coPrimeProduct;
        calcPrivateKey[0][0] = calcPrivateKey[1][0];
        calcPrivateKey[0][1] = calcPrivateKey[1][1];
        calcPrivateKey[1][0] = tempResult1;
        calcPrivateKey[1][1] = tempResult2;
    }
    return calcPrivateKey[1][1];
}
/**
 * @brief encrypts or decrypts a character with a key
 * @param key Public key that is used to encrypt or decrypt the character
 * @param primeProduct
 * @param character
 */
unsigned cryptCharacter(unsigned key, unsigned primeProduct, unsigned character)
{
    unsigned long cryptResult = 1;

    //calculate character ^ key mod primeproduct = cryptCharacter
    for(;key > 0; key--) {
        cryptResult *= character;
        cryptResult %= primeProduct;
    }
    return cryptResult;
}

void clearBuffer()
{
    char c;
    while((c = getchar()) != '\n' && c != EOF);
}
