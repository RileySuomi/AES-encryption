#include <iostream>
#include <string>
#include <random>
#include <array>
#include <vector>
#include <stdlib.h>
#include <stdint.h>
#include <queue>
#include <cstdint>

using namespace std;

// SBox used for subbytes step, fixed values so no need to regenerate
const uint8_t sbox[256] = { 0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA8, 0x51, 0xA3,
    0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2, 0xCD, 0x0C,
    0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73, 0x60, 0x81,
    0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB, 0xE0, 0x32,
    0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0x0F, 0xF7, 0x68, 0x0D,
    0x74, 0x17, 0xF6, 0xC9, 0x0A, 0xF2, 0xE7, 0x94, 0x5D, 0x9D, 0x7B, 0x8B, 0x3C, 0xF5, 0x3D, 0x9A};

const uint8_t inverseSBox[256] = {0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0x7B, 0xF6, 0xE1, 0x9A, 0x8D, 0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE0, 0x43, 0x4A, 0xC4, 0x52,
    0xE9, 0xB7, 0x27, 0xB4, 0x4C, 0x3B, 0x35, 0x99, 0xAF, 0xA4, 0xC1, 0x76, 0xC7, 0xA9, 0x65, 0xA2,
    0xF0, 0xC1, 0x49, 0x3B, 0xA5, 0xD1, 0x66, 0x72, 0x40, 0xF7, 0xC2, 0xBC, 0xB4, 0x2C, 0x98, 0x51,
    0xA3, 0x40, 0xA3, 0x9B, 0x7E, 0x29, 0x19, 0x3C, 0x3E, 0x20, 0x4D, 0x72, 0x36, 0x64, 0x5C, 0x3A,
    0xA8, 0x72, 0x82, 0x2C, 0x62, 0x9B, 0x63, 0x7C, 0xC4, 0x6B, 0x79, 0x93, 0x39, 0x59, 0x7F, 0x37,
    0x8E, 0x79, 0x64, 0x34, 0x44, 0xE2, 0xD4, 0x28, 0x55, 0x25, 0x0E, 0x60, 0x58, 0x2A, 0x88, 0x2B,
    0xD5, 0xF3, 0xD2, 0x46, 0x89, 0x12, 0x0F, 0xE9, 0x67, 0xD3, 0x5B, 0x69, 0x58, 0xB3, 0x0A, 0x0F,
    0x5F, 0x93, 0x2F, 0xB2, 0x97, 0x8B, 0x5D, 0x49, 0x6E, 0x34, 0x8A, 0x77, 0x93, 0x5A, 0xD1, 0x7E,
    0x21, 0x4F, 0x56, 0x63, 0x5C, 0x5E, 0x4E, 0x5D, 0x6D, 0x44, 0x67, 0x91, 0x6C, 0x3A, 0xA4, 0xB2
};

// This function derives a new 128 bit key each round
uint8_t generateKey(uint8_t input) {
    
}


/*
    This function does byte substitution for one 
    16 byte block.
*/

void subBytesHelper(vector<uint8_t> &input, bool encrypt) {
    for (int i = 0; i < 16; i++) {
        
        /*
            Substitute each input byte using a corresponding
            byte from the sub box
        */
        if (encrypt) {

            input[i] = sbox[input[i]];
        }
        else {
            input[i] = inverseSBox[input[i]];
        }
    }
}

/*
    This function takes a parameter, encrypt, which determines if the
    substitution is done using the sbox or the inverse sbox. Encryption
    uses the sbox while decryption uses the inverse sbox.
*/
void subBytes(vector<vector<uint8_t> > &messageBlock, bool encrypt) {
    
    
    for (int i = 0; i < messageBlock.size(); i++) {
        
        subBytesHelper(messageBlock[i]);
        
    }
}

/*
        Shifting rules for state array
        First row no shifting
        Second row 1 byte circular left shift
        Third row 2 byte circular left shift
        Fourth row 3 byte circlar left shift
    */
void shiftRows(vector<vector<uint8_t> > &messageBlocks) {

    /*
        Iterate through each state array (16 byte block)
    */
   for (int i = 0; i < messageBlocks.size(); i++) {
        
       vector<uint8_t> currentBlock = messageBlocks[i];

        /*
            Start 2nd row shift by collecting 2nd row values
        */
       uint8_t firstValue = currentBlock[7];
       currentBlock[7] = currentBlock[4];
       currentBlock[4] = currentBlock[5];
       currentBlock[5] = currentBlock[6];
       currentBlock[6] = firstValue;

       /*
            Next, do third row shifts.
       */
       firstValue = currentBlock[8];
       uint8_t secondValue = currentBlock[9];
       currentBlock[8] = currentBlock[10];
       currentBlock[9] = currentBlock[11];
       currentBlock[10] = firstValue;
       currentBlock[11] = secondValue;
        
        /*
            Last, do fourth row shifts.
       */
      firstValue = currentBlock[15];
      currentBlock[15] = currentBlock[14];
      currentBlock[14] = currentBlock[13];
      currentBlock[13] = currentBlock[12];
      currentBlock[12] = firstValue;
   }

}

/*
    This function does the inverse of procedures in the shift rows function
*/
void inverseShiftRows(vector<vector<uint8_t> > &messageBlocks) {

    /*
        Iterate through each state array (16 byte block)
    */
   for (int i = 0; i < messageBlocks.size(); i++) {
        
       vector<uint8_t> currentBlock = messageBlocks[i];

        /*
            shift once to right
        */
       uint8_t firstValue = currentBlock[4];
       currentBlock[4] = currentBlock[7];
       currentBlock[7] = currentBlock[6];
       currentBlock[6] = currentBlock[5];
       currentBlock[5] = firstValue;

       /*
            Next, do third row shifts.
       */
       firstValue = currentBlock[8];
       uint8_t secondValue = currentBlock[9];
       currentBlock[8] = currentBlock[10];
       currentBlock[9] = currentBlock[11];
       currentBlock[10] = firstValue;
       currentBlock[11] = secondValue;
        
        /*
            Last, do fourth row shifts.
       */
      firstValue = currentBlock[12];
      currentBlock[12] = currentBlock[13];
      currentBlock[13] = currentBlock[14];
      currentBlock[14] = currentBlock[15];
      currentBlock[15] = firstValue;
   }

}

uint8_t mixColumns(uint8_t input) {


}


// AESMaster handles the steps in encryption 
uint8_t  AESMaster(uint8_t input) {
    //uint8_t output = subBytes(output);
    // output = shiftRows(output);
    // output = mixColumns(output);
    // output = applyRoundKey(input);
    
}

void createBlocks(vector<vector<uint8_t> > &blockQueue, string input) {
    const int blockSize = 16;
    int blockStart = 0;
    cout << input.substr(16,31) << std::endl;
    
    for (int i = 0; i < input.length(); i++) {
        /*
            If we are at the end of a 16 byte sequence or at the end of 
            the message, create a block and push it.
        */
       int diff = i - blockStart;
        if (i - blockStart == blockSize - 1 || i == input.length() - 1) {
            string currentBlock = input.substr(blockStart, blockSize);
            blockQueue.push_back(vector<uint8_t>(currentBlock.begin(), currentBlock.end()));
            cout << i - blockStart << std::endl;
            blockStart = i + 1;
        }
    }    
}

int main() {
    // dummy string for testing change to file io later
    string message = "Born in Seattle, Washington, Hendrix began playing guitar at age 15. In 1961, he enlisted in the US Army, but was discharged the following year. Soon afterward, he moved to Clarksville, then Nashville, Tennessee, and began playing gigs on the chitlin' circuit, earning a place in the Isley Brothers' backing band and later with Little Richard, with whom he continued to work through mid-1965. He then played with Curtis Knight and the Squires.";
    
    vector<vector<uint8_t> > messageBlocks;
    
    createBlocks(messageBlocks, message);

    subBytes(messageBlocks, true);

    shiftRows(messageBlocks);
    // vector<uint8_t> test = messageBlocks.pop();
    // cout << test << std::endl;
    
    // AESMaster();
}