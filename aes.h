
#ifndef AES_H_
#define AES_H_

#include <array>
#include <vector>
#include <cstdint>

class AES {
    public:
    AES(const std::vector<uint8_t>& key); // constructor to initialize with a key
    std::vector<uint8_t> Encrypt(const std::vector<uint8_t>& defaulttext);
    std::vector<uint8_t> Decrypt(const std::vector<uint8_t>& cipher);

    private: 
    static const int Nb = 4; // number of columns, AES Nb is always 4
    static const int Nk = 4; // number of 32-but words for the cipher key
    static const int Nr = 10; // number of rounds.
    uint8_t expandedKey[176]; // expanded key holder (expanded key here is 176 bytes)

    // methods that AES uses in encryption.

    void ExpandKey(const std::vector<uint8_t>& key);
	void AddRoundKey(std::array<uint8_t, 16>& state, int round);
	void SubstituteBytes(std::array<uint8_t, 16>& state);
	void ShiftRows(std::array<uint8_t, 16> &messageBlocks);
	void InverseSubstituteBytes(std::array<uint8_t, 16>& state);
	void InverseShiftRows(std::array<uint8_t, 16>& state);

    /*
    The S-Box is a constant table of values that AES uses (Rijndael)
    */

    /*
    Inverse S-Box is used to "undo" substitution performed by 
    original AES encryption
    */

    static const uint8_t sbox[256];
    static const uint8_t rcon[10];
    static const uint8_t inverse_sbox[256];

};


#endif // AES_H_