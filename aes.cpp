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

// Sbox subsitution method
void AES::SubstituteBytes(std::array<uint8_t, 16>& state) {
    for (int i = 0; i < 16; ++i) {
        // sub the byte with the value from the Sbox
        // sbox is indexed by the byte's curr val
        state[i] = sbox[state[i]];
    }
}


void AES::ShiftRows(std::array<uint8_t, 16>& state) {
}