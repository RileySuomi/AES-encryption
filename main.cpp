#include <iostream>
#include "aes.h"
#include <fstream>
#include <array>
#include <vector>
#include <filesystem>
#include <string>
#include <sstream>

using namespace std;
namespace fs = filesystem;

// menu display for user input
void DisplayMenu() {
    
    printf("Choose a command");
    printf("Encrypt = e");
    printf("enter your file");
    printf("Descrypt = d");
    printf("enter your file");
    printf("Quit = q");
}

// ask user for file path 
string GetFilePath() {
    string file;
    cout << "Enter file path: ";
    getline(cin, file);
    return file;
}

// need file contents as a vector in order to encrypt or decrypt
vector<unsigned char>GetContents(string filePath) {
    ifstream file(filePath, ios::binary); // open in binary

    if (!file) {
        std::cerr << "error opening file: " << filePath << endl;
        return;
    }

    vector<unsigned char> file_data(
        (istreambuf_iterator<char>(file)), 
        istreambuf_iterator<char>()
    );
    file.close();

    return file_data; 
}

void WriteToFile(string file_path, vector<unsigned char>processed) {
    ofstream outFile(file_path, ios::out | ios::binary);
    if (!outFile.is_open()) {
        cerr << "failed" << "/n";
        return;
    }

    // write to output file and close
    outFile.write((const char*)processed.data(), processed.size());
    outFile.close();

    cout << "Process success!" << file_path << "/n";
}

void Encryption(AES& aes, string file_path, vector<unsigned char>data) {
    try {
        // should encrypt the desired txt file
        vector<unsigned char>encrypted = aes.Encrypt(data);
        // need to write to a file to see the result
        fs::path path(file_path);
        string newFile = "encryptedfile_" + path.stem().string() + ".txt";
        WriteToFile(newFile, encrypted);

    } 
    catch (const exception& e) {
        cerr << "failed" << '/n';
        return;
    }
}

void Decryption(AES& aes, string file_path, vector<unsigned char> data) {
    try {
        // should decrypt the desired txt file
        vector<unsigned char>decrypted = aes.Decrypt(data);
        // need to write to a file to see the result
        fs::path path(file_path);
        string newFile = "decryptedfile_" + path.stem().string() + ".txt";
        WriteToFile(newFile, decrypted);

    } 
    catch (const exception& e) {
        cerr << "failed" << '/n';
        return;
    }
}

int main() {

    vector<unsigned char> key(16, 'r');
    AES aes(key);
    char user_input = NULL;
    

    while (user_input != 'q') {
        DisplayMenu();
        user_input = getchar();

        switch(user_input) {
            case 'e': {
            string file = GetFilePath();
            vector<unsigned char>data = GetContents(file);
            Encryption(aes, file, data);
            cout << "/n";
            break;
            }

            case 'd': {
            string file = GetFilePath();
            vector<unsigned char>data = GetContents(file);
            Decryption(aes, file, data);
            cout << "/n";
            break;
            }
        }
    }
}