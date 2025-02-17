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

// This function derives a new 128 bit key each round
uint8_t generateKey(uint8_t input) {
    
}


/*
    This function does byte substitution for one 
    16 byte block.
*/

void subBytesHelper(vector<uint8_t> &input) {
    for (int i = 0; i < 16; i++) {
        
        /*
            Substitute each input byte using a corresponding
            byte from the sub box
        */
        
        input[i] = sbox[i];
    }
}

void subBytes(vector<vector<uint8_t> > &messageBlock) {
    
    
    for (int i = 0; i < messageBlock.size(); i++) {
        
        subBytesHelper(messageBlock[i]);
        
    }
}


uint8_t shiftRows(uint8_t input) {
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

    subBytes(messageBlocks);
    

    // vector<uint8_t> test = messageBlocks.pop();
    // cout << test << std::endl;
    cout << "test" << std::endl;
    // AESMaster();
}