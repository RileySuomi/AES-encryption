#include <iostream>
#include "aes.h"
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// menu display for user input
void DisplayMenu() {  
    printf("Choose a command\n");
    printf("Encrypt = e\n");
    printf("Descrypt = d\n");
    printf("Quit = q\n");
    printf("Your choice: ");
}

// ask user for file path 
string GetFilePath() {

    string file;
    cout << "Enter file path: ";
    cin >> file;
    //cout << "Debug: File path entered: " << file << endl;
    return file;
}

// need file contents as a vector in order to encrypt or decrypt
vector<unsigned char>GetContents(string filePath) {

    ifstream file(filePath, ios::binary); // open in binary

    if(!file.is_open()) {
        std::cerr << "error opening file: " << filePath << endl;
        vector<unsigned char> empty;
        return empty;
    }

    vector<unsigned char> file_data(
        (istreambuf_iterator<char>(file)), 
        istreambuf_iterator<char>()
    );
    file.close();

    return file_data; 
}

void WriteToFile(string file_path, const vector<unsigned char>processed) {
    
    ofstream outFile(file_path, ios::out | ios::binary);

    if (!outFile.is_open()) {
        cerr << "Failed to open file for writing" << endl;
        return;
    }

    // Write to output file and close
    outFile.write(reinterpret_cast<const char*>(processed.data()), processed.size());
    outFile.close();

    cout << "Process success! File written to " << file_path << "\n";
}

// Generate a new file name by appending a prefix
// filesystem is broken on my version of c++
string GenerateNewFileName(const string& originalFilePath, const string& prefix) {
    size_t lastSlashPos = originalFilePath.find_last_of("/\\");
    string directory;
    string filename;

    if (lastSlashPos != string::npos) {
        directory = originalFilePath.substr(0, lastSlashPos + 1);
        filename = originalFilePath.substr(lastSlashPos + 1);
    } else {
        directory = "";
        filename = originalFilePath;
    }

    size_t lastDotPos = filename.find_last_of(".");
    string stem = filename.substr(0, lastDotPos);
    string extension = (lastDotPos != string::npos) ? filename.substr(lastDotPos) : "";

    return directory + prefix + stem + extension;
}

void Encryption(AES& aes, string file_path, const vector<unsigned char> data) {
    try {
        // Encrypt the desired txt file
        vector<unsigned char> encrypted = aes.Encrypt(data);

        // Create a new file name for the encrypted file
        string newFile = GenerateNewFileName(file_path, "encrypted_");
        WriteToFile(newFile, encrypted);
    } 
    catch (const exception& e) {
        cerr << "Encryption failed\n";
        return;
    }
}

void Decryption(AES& aes, string file_path, const vector<unsigned char> data) {
    try {
        // Decrypt the desired txt file
        vector<unsigned char> decrypted = aes.Decrypt(data);

        // Create a new file name for the decrypted file
        string newFile = GenerateNewFileName(file_path, "decrypted_");
        WriteToFile(newFile, decrypted);
    } 
    catch (const exception& e) {
        cerr << "Decryption failed\n";
        return;
    }
}

int main() {

    vector<unsigned char> key(16, 'r');
    AES aes(key);
    char user_input = '\0';
    
    // while user hasnt quit
    while (user_input != 'q') {
        DisplayMenu();
        user_input = getchar();

        switch(user_input) {
            case 'e': {
            string file = GetFilePath();
            vector<unsigned char>data = GetContents(file);
            if (data.empty()) {
                cout << "we empy?" << endl;
                return 0;
            }
            Encryption(aes, file, data);
            cout << "\n";
            break;
            }

            case 'd': {
            string file = GetFilePath();
            vector<unsigned char>data = GetContents(file);
            Decryption(aes, file, data);
            cout << "\n";
            break;
            }
        }
    }
    return 0;
}