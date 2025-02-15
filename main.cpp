#include <iostream>

using namespace std;

int main() {

    generateSBox();
    AESMaster();

}

int galoisField() {

}

void generateSBox() {
    int galoisField();

}


// AESMaster handles the steps in encryption 
uint8_t  AESMaster(uint8_t input) {
    uint8_t output = subBytes(output);
    output = applyRoundKey(input);
    output = shiftRows(output);
    output = mixColumns(output);
    
}

uint8_t applyRoundKey(uint8_t input) {

}

uint8_t subBytes(uint8_t input) {
    input = applyRoundKey(input);
}

uint8_t shiftRows(uint8_t input) {
    input = applyRoundKey(input);
}

uint8_t mixColumns(uint8_t input) {
    input = applyRoundKey(input);

}