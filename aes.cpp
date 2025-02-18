#include "aes.h"
#include <iostream>
#include <cstring>
#include<stdexcept>

using namespace std;

const uint8_t AES::sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const uint8_t AES::rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

const uint8_t AES::inverse_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
    };

AES::AES(const vector<uint8_t>& key) {
    if (key.size() != 16) {
        cerr << "Invalid key for AES-128! Must be 128 bits." << endl;
        return;
    }

    ExpandKey(key); // key gets expanded to 176 bytes, from the 128.
}

void AES::ExpandKey(const std::vector<uint8_t>& key) {
    
    // first copy original key to the beginning of expanded key
    for (int i = 0; i < Nk; ++i) {
        expandedKey[i * 4] = key[i * 4];
        expandedKey[i * 4 + 1] = key[i * 4 + 1];
        expandedKey[i * 4 + 2] = key[i * 4 + 2];
        expandedKey[i * 4 + 3] = key[i * 4 + 3];
    }

    uint8_t tmp[4]; // temporary storage

    // iterate over expanded key to generate all round keys!
    for (int i = Nk; i < Nb * (Nr + 1); ++i) {
        tmp[0] = expandedKey[(i - 1) * 4];
        tmp[1] = expandedKey[(i - 1) * 4 + 1];
        tmp[2] = expandedKey[(i - 1) * 4 + 2];
        tmp[3] = expandedKey[(i - 1) * 4 + 3];

        // every Nk words, apply key schedule (rotate, sub, Rcon)
        if (i % Nk == 0) {
            // Rotate Word : (rotating tmp left by 1 byte)
            uint8_t t = tmp[0];
            tmp[0] = tmp[1];
            tmp[1] = tmp[2];
            tmp[2] = tmp[3];
            tmp[3] = t;

            // Sub Word : apply sbox to each byte of the word 
            tmp[0] = sbox[tmp[0]];
            tmp[1] = sbox[tmp[1]];
            tmp[2] = sbox[tmp[2]];
            tmp[3] = sbox[tmp[3]];

            // Rcon : XOR the first byte with Rcon
            tmp[0] ^= rcon[i / Nk - 1];
        }

        // XOR tmp with the word Nk positions earlier and store in expandedKey
        for (int j = 0; j < 4; ++j) {
            expandedKey[i * 4 + j] = expandedKey[(i - Nk) * 4 + j] ^ tmp[j];
        }
    }
}

// round key is generated at the start of each round, modifies the the expanded key based on curr round.
void AES::AddRoundKey(std::array<uint8_t, 16>& state, int round) {

    // calculation of start index
    size_t start = round * Nb * 4;

    // XOR the round key with the state block
    for (int i = 0; i < 16; ++i) {
        state[i] ^= expandedKey[start + 1];
    }
}

// Sbox substitution method
void AES::SubstituteBytes(std::array<uint8_t, 16>& state) {
    for (int i = 0; i < 16; ++i) {
        // sub the byte with the value from the Sbox
        // sbox is indexed by the byte's curr val
        state[i] = sbox[state[i]];
    }
}

    /*
        Shifting rules for state array
        First row no shifting
        Second row 1 byte circular left shift
        Third row 2 byte circular left shift
        Fourth row 3 byte circlar left shift
    */

void AES::ShiftRows(std::array<uint8_t, 16> &block) {

    // Temporary copy of the state for manipulation
    std::array<uint8_t, 16> tmp = block;

    // 1st row (row 0) remains unchanged. Dont SHIFT

    // 2nd row (row 1) left shift by 1 byte
    block[1] = tmp[5];
    block[5] = tmp[9];
    block[9] = tmp[13];
    block[13] = tmp[1];

    // 3rd row (row 2) left shift by 2 bytes
    block[2] = tmp[10];
    block[6] = tmp[14];
    block[10] = tmp[2];
    block[14] = tmp[6];

    // 4th row (row 3) left shift by 3 bytes
    block[3] = tmp[15];
    block[7] = tmp[3];
    block[11] = tmp[7];
    block[15] = tmp[11];
}

// Inverse Sbox method
void AES::InverseSubstituteBytes(std::array<uint8_t, 16>& state) {
    for (int i = 0; i < 16; ++i) {
        state[i] = inverse_sbox[state[i]];
    }
}

// shift the rows back to original / where they were before
void AES::InverseShiftRows(std::array<uint8_t, 16>& block) {
    array<uint8_t, 16> tmp = block;
    
	// Do not shift Row 0!

    // Row 1 shifts right by 1: 
    block[5] = tmp[1];
    block[9] = tmp[5];
    block[13] = tmp[9];
    block[1] = tmp[13];

    // Row 2 shifts right by 2: 
    block[10] = tmp[2];
    block[14] = tmp[6];
    block[2] = tmp[10];
    block[6] = tmp[14];

    // Row 3 shifts right by 3: 
    block[15] = tmp[3];
    block[3] = tmp[7];
    block[7] = tmp[11];
    block[11] = tmp[15];
}

/*
encrypt data using aes-128
*/

// vector<uint8_t> AES::Encrypt(const std::vector<uint8_t>& defaulttext) {
    
//     std::array<uint8_t, 16> state; // state of the default data 16 bytes
//     std::vector<uint8_t> ciphertext; // storage for the encryption result //?


//     // Copy the text to state array
//     for (int i = 0; i < 16; ++i) {
//         state[i] = defaulttext[i]; //one block
//     }

//     // Initial round key addition
//     AddRoundKey(state, 0);

//     // Main rounds (Nr - 1 rounds)
//     for (int round = 1; round < Nr; ++round) {
//         SubstituteBytes(state);
//         ShiftRows(state);
//         AddRoundKey(state, round);
//     }

//     // Final round 
//     SubstituteBytes(state);
//     ShiftRows(state);
//     AddRoundKey(state, Nr);

//     // Copy state array to ciphertext
//     for (int i = 0; i < 16; ++i) {
//         ciphertext.push_back(state[i]); // adding the encryption to the vector
//     }

//     // ciphertext should now hold the result of the encrypted message 
//     return ciphertext;
// }

// vector<uint8_t> AES::Decrypt(const std::vector<uint8_t>& cipher) {
    
//     std::array<uint8_t, 16> state; // state of the cipher data
//     std::vector<uint8_t> text; // storage for turning the cipher into normal text

//     // Copy ciphertext to state array
//     for (int i = 0; i < 16; ++i) {
//         state[i] = cipher[i];
//     }

//     // Initial round key addition
//     AddRoundKey(state, Nr);

//     // Main rounds (Nr - 1 rounds)
//     for (int round = Nr - 1; round > 0; --round) {
//         InverseShiftRows(state);
//         InverseSubstituteBytes(state);
//         AddRoundKey(state, round);
//     }

//     // Final round
//     InverseShiftRows(state);
//     InverseSubstituteBytes(state);
//     AddRoundKey(state, 0);

//     // Copy state array to normal text
//     for (int i = 0; i < 16; ++i) {
//         text.push_back(state[i]);
//     }

//     return text; // returns the decrypted text
// }

vector<uint8_t> AES::Decrypt(const std::vector<uint8_t>& cipher) {
    
    std::array<uint8_t, 16> state = {}; // state of the default data 16 bytes // needs to be one block of data. 
    std::vector<uint8_t> text; 
    std::vector<vector<uint8_t> > blockqueue;
    int size = 0; 

  
    createBlocks(blockqueue, cipher);
   cout << blockqueue.size() << endl;
    for(std::vector<uint8_t> block : blockqueue){
        for(int i = 0; i < block.size(); i++){
            state[i] = block.at(i);
            size++;
        }
        AddRoundKey(state, Nr);

        // Main rounds (Nr - 1 rounds)
        for (int round = Nr-1; round >0 ; --round) {
            InverseShiftRows(state);
            InverseSubstituteBytes(state);
            AddRoundKey(state, round);
        }
    
        // Final round 
        
        InverseShiftRows(state);
        InverseSubstituteBytes(state);
        AddRoundKey(state, 0);
        //if final block remove padding
       // RemovePadding(state);
       
       RemovePadding(state,size);

       
        // Copy state array to ciphertext
        for (int i = 0; i < size; ++i) {
            text.push_back(state[i]); // adding the encryption to the vector
        }

        size = 0;
        // ciphertext should now hold the result of the encrypted message or current block
        //now i should do what? return the whole ciphertext? 
        
        
    }
    return text; // returns the decrypted text
}
void AES::RemovePadding(std::array<uint8_t, 16>& block,int& size){
    if(size == 0) return;
    uint8_t padding = block[size-1];
    
    if(padding <= size){
        size = size -padding; 
        return;
    }

};

void AES::AddPadding(std::vector<uint8_t>& block){
    
    uint8_t paddingSize = 16-block.size();

    block.resize(16,paddingSize);
    return;

    
};

void AES::createBlocks(std::vector<vector<uint8_t> > &blockQueue, const std::vector<uint8_t>& input) {
    size_t i;
    vector<uint8_t> temp; 
    for( i = 0; i < input.size(); i++){
        if(i != 0 && i % 16 == 0){
        //push into queue
            blockQueue.push_back(temp);
            temp.clear();
        }
        temp.push_back(input.at(i));
    }

    if(!input.empty()) blockQueue.push_back(temp);



}
void AES::createBlocksEncrypt(std::vector<vector<uint8_t> > &blockQueue, const std::vector<uint8_t>& input) {
    size_t i;
    vector<uint8_t> temp; 
    for( i = 0; i < input.size(); i++){
        if(i != 0 && i % 16 == 0){
        //push into queue
            blockQueue.push_back(temp);
            temp.clear();
        }
        temp.push_back(input.at(i));
    }

///wont matter for decrypt because it will never be empty!
    
    AddPadding(temp);
    blockQueue.push_back(temp); //pushes whatever is there 
    
//adds padding no matter what. 

}

vector<uint8_t> AES::Encrypt(const std::vector<uint8_t>& defaulttext) {
    std::array<uint8_t, 16> state ={}; // state of the default data 16 bytes // needs to be one block of data. 
    std::vector<uint8_t> ciphertext; 
    std::vector<vector<uint8_t> > blockqueue;
    int size = 0;
    

    createBlocksEncrypt(blockqueue, defaulttext);
   
    for(std::vector<uint8_t> block : blockqueue){
        for(int i = 0; i < block.size(); i++){
            state[i] = block.at(i);
            size++;
        }
        AddRoundKey(state, 0);

        // Main rounds (Nr - 1 rounds)
        for (int round = 1; round < Nr; ++round) {
            SubstituteBytes(state);
            ShiftRows(state);
            AddRoundKey(state, round);
        }
    
        // Final round 
        SubstituteBytes(state);
        ShiftRows(state);
        AddRoundKey(state, Nr);
    
       
        
        for (int i = 0; i < 16; ++i) {
            ciphertext.push_back(state[i]); // adding the encryption to the vector
        }
        size = 0;
        // ciphertext should now hold the result of the encrypted message or current block
        //now i should do what? return the whole ciphertext? 
        
        
    }

    


    return ciphertext;
}