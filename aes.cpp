#include "aes.h"
#include <iostream>
#include <cstring>

using namespace std;

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
void InverseShiftRows(std::array<uint8_t, 16>& block) {
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

vector<uint8_t> AES::Encrypt(const std::vector<uint8_t>& defaulttext) {
    
    
    std::array<uint8_t, 16> state; // state of the default data
    std::vector<uint8_t> ciphertext; // storage for the encryption result


    // Copy the text to state array
    for (int i = 0; i < 16; ++i) {
        state[i] = defaulttext[i];
    }

    // Initial round key addition
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

    // Copy state array to ciphertext
    for (int i = 0; i < 16; ++i) {
        ciphertext.push_back(state[i]); // adding the encryption to the vector
    }

    // ciphertext should now hold the result of the encrypted message 
    return ciphertext;
}

vector<uint8_t> AES::Decrypt(const std::vector<uint8_t>& cipher) {
    
    std::array<uint8_t, 16> state; // state of the cipher data
    std::vector<uint8_t> text; // storage for turning the cipher into normal text

    // Copy ciphertext to state array
    for (int i = 0; i < 16; ++i) {
        state[i] = cipher[i];
    }

    // Initial round key addition
    AddRoundKey(state, Nr);

    // Main rounds (Nr - 1 rounds)
    for (int round = Nr - 1; round > 0; --round) {
        InverseShiftRows(state);
        InverseSubstituteBytes(state);
        AddRoundKey(state, round);
    }

    // Final round
    InverseShiftRows(state);
    InverseSubstituteBytes(state);
    AddRoundKey(state, 0);

    // Copy state array to normal text
    for (int i = 0; i < 16; ++i) {
        text.push_back(state[i]);
    }

    return text;
}
