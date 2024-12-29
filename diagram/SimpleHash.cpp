#include <iostream>

using namespace std;

#define DATA_MIN_SIZE 1
#define DATA_MAX_SIZE 255
#define DATA_MAX_BUFFER_SIZE 260

#define KEY_MIN_SIZE 5
#define KEY_MAX_SIZE 100

#define HESLO_SIZE 10

size_t len(unsigned char arr[]) {
    size_t lenght = 0;
    while (arr[lenght] != '\0') {
        lenght++;
    }
    return lenght;
}


void copy(unsigned char from[], unsigned char to[], int len) {
    for (int i = 0; i < len; i++) {
        to[i] = from[i];
    }
}

bool dataLenghtInRange(streamsize lenght) {
    return lenght >= DATA_MIN_SIZE && lenght <= DATA_MAX_SIZE;
}

bool keyLenghtInRange(streamsize lenght) {
    return lenght >= KEY_MIN_SIZE; 
}

int usedDataRows(int lenght) {
    int usedRows = lenght / 10;
    if (lenght % 10 != 0) {
        usedRows++;
    }
    return usedRows;
}

void resetAndFill(unsigned char data[], int lenght) {
    for (int i = 0; i < lenght; i++) {
        data[i] = 'a';
    }
}

void splitDataToRows(unsigned char data[], unsigned char dataBuffer[DATA_MAX_BUFFER_SIZE/10][10], int lenght) {
    for (int i = 0; i < DATA_MAX_BUFFER_SIZE/10; i++) {
        for (int j = 0; j < 10; j++) {
            dataBuffer[i][j] = data[i * 10 + j];
        }
    }
    return;
}

void passwGen(unsigned char privKey[], unsigned char heslo[10], int lenght) {
    if (lenght == 10) {
        copy(privKey, heslo, lenght);
        return;
    }
    if (lenght < 10) {
        copy(privKey, heslo, lenght);
        return ;
    }
    unsigned char sum = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < lenght; j++) {
            sum += privKey[j];
            j+= 10;
        }
        heslo[i] = sum;
        sum = 0;
    }
    
    return;
}

unsigned char loadAndHandleKey(){
    unsigned char privKey[KEY_MAX_SIZE];
    unsigned char heslo[HESLO_SIZE];
    streamsize privKeyLenght;

    resetAndFill(privKey, KEY_MAX_SIZE); 

    printf("Zadejte privátní klíč o délce alespoň 5 znaků: ");
    cin.getline(reinterpret_cast<char*>(privKey), sizeof(privKey));
    privKeyLenght = cin.gcount() - 1;
    if (!keyLenghtInRange(privKeyLenght)) {
        cerr << "Klíč musí být alespoň 5 znaků dlouhý" << endl;
    }

    passwGen(privKey, heslo, privKeyLenght);
    return *heslo;
}

unsigned short generateHash(unsigned char dataBuffer[DATA_MAX_BUFFER_SIZE/10][10], unsigned char heslo[10], int lenght) {
    unsigned short hash; // stačí nám, protože max hodnota bude <= 65025 (255*255)
    int usedRows = usedDataRows(lenght);

    for (int i = 0; i < usedRows; i++) {
        for (int j = 0; j < 10; j++) {
            dataBuffer[i][j] = dataBuffer[i][j] ^ heslo[j];
            hash += dataBuffer[i][j];
        }
    }
    return hash;
}



int main() {
    unsigned char data[DATA_MAX_BUFFER_SIZE];
    unsigned char dataBuffer[DATA_MAX_BUFFER_SIZE/10][10];
    unsigned short hash;
    streamsize DataLenght;


    cout << "data: " << data << endl;
    printf("Zadejte vstupní data v rozsahu 1-255 znaků: ");
    cin.getline(reinterpret_cast<char*>(data), sizeof(data));
    DataLenght = cin.gcount() - 1;

    if (!dataLenghtInRange(DataLenght)) {
        cerr << "Data musí být v rozsahu 1-255 znaků" << endl;
        return 1;
    }



    cout << "data lenght: " << DataLenght << endl;
    cout << "privKey lenght: " << privKeyLenght << endl;


    passwGen(privKey, heslo, privKeyLenght);
    splitDataToRows(data, dataBuffer, DataLenght);
    hash = generateHash(dataBuffer, heslo, DataLenght);


    cout << "Hash: " << hash << endl;
    return 0;
}
