#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <filesystem>
#include <functional>

using namespace std;
namespace fs = filesystem;

const string ENCRYPTED_EXT = ".enc";
const string DECRYPTED_EXT = ".dec";

string toUpper(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

string toLower(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

bool isHexString(const string &s)
{
    for (char c : s)
        if (!isxdigit(c))
            return false;
    return !s.empty();
}

string bytesToHex(const vector<uint8_t> &data)
{
    stringstream ss;
    for (uint8_t b : data)
        ss << hex << setw(2) << setfill('0') << (int)b;
    return ss.str();
}

vector<uint8_t> hexToBytes(const string &hex)
{
    vector<uint8_t> bytes;
    for (size_t i = 0; i + 1 < hex.length(); i += 2)
    {
        uint8_t b = stoul(hex.substr(i, 2), nullptr, 16);
        bytes.push_back(b);
    }
    return bytes;
}

string getFileSizeString(uintmax_t bytes)
{
    stringstream ss;
    if (bytes < 1024)
        ss << bytes << " B";
    else if (bytes < 1024 * 1024)
        ss << fixed << setprecision(2) << (bytes / 1024.0) << " KB";
    else
        ss << fixed << setprecision(2) << (bytes / (1024.0 * 1024.0)) << " MB";
    return ss.str();
}

class CaesarCipher
{
public:
    string encrypt(const string &text, int shift)
    {
        string result = text;
        for (char &c : result)
        {
            if (isupper(c))
                c = char((c - 'A' + shift) % 26 + 'A');
            else if (islower(c))
                c = char((c - 'a' + shift) % 26 + 'a');
        }
        return result;
    }

    string decrypt(const string &text, int shift)
    {
        return encrypt(text, 26 - (shift % 26));
    }

    void bruteForce(const string &text)
    {
        cout << "\nAll 25 possible shifts:\n";
        cout << string(50, '-') << "\n";
        for (int shift = 1; shift < 26; shift++)
        {
            string decrypted = decrypt(text, shift);
            cout << "Shift " << setw(2) << shift << ": " << decrypted << "\n";
        }
    }
};

class VigenereCipher
{
public:
    string encrypt(const string &text, const string &key)
    {
        string result = text;
        string upperKey = toUpper(key);
        size_t keyIndex = 0;

        for (char &c : result)
        {
            if (isalpha(c))
            {
                char base = isupper(c) ? 'A' : 'a';
                int shift = upperKey[keyIndex % upperKey.size()] - 'A';
                c = char((c - base + shift) % 26 + base);
                keyIndex++;
            }
        }
        return result;
    }

    string decrypt(const string &text, const string &key)
    {
        string result = text;
        string upperKey = toUpper(key);
        size_t keyIndex = 0;

        for (char &c : result)
        {
            if (isalpha(c))
            {
                char base = isupper(c) ? 'A' : 'a';
                int shift = upperKey[keyIndex % upperKey.size()] - 'A';
                c = char((c - base - shift + 26) % 26 + base);
                keyIndex++;
            }
        }
        return result;
    }
};

class XorCipher
{
public:
    string encryptDecrypt(const string &text, const string &key)
    {
        string result = text;
        for (size_t i = 0; i < result.size(); i++)
            result[i] ^= key[i % key.size()];
        return result;
    }
};

class Base64Coder
{
private:
    const string BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

public:
    string encode(const string &data)
    {
        string result;
        size_t i = 0;
        while (i < data.size())
        {
            uint32_t triple = (uint8_t)data[i++];
            if (i < data.size())
                triple = (triple << 8) | (uint8_t)data[i++];
            if (i < data.size())
                triple = (triple << 8) | (uint8_t)data[i++];

            result += BASE64_CHARS[(triple >> 18) & 0x3F];
            result += BASE64_CHARS[(triple >> 12) & 0x3F];
            result += (i - 2 < data.size()) ? BASE64_CHARS[(triple >> 6) & 0x3F] : '=';
            result += (i - 1 < data.size()) ? BASE64_CHARS[triple & 0x3F] : '=';
        }
        return result;
    }

    string decode(const string &encoded)
    {
        vector<int> lookup(256, -1);
        for (int i = 0; i < 64; i++)
            lookup[BASE64_CHARS[i]] = i;

        string result;
        size_t i = 0;
        while (i < encoded.size() && encoded[i] != '=')
        {
            uint32_t triple = 0;
            int valid = 0;
            for (int j = 0; j < 4 && i < encoded.size(); j++, i++)
            {
                if (encoded[i] == '=')
                    break;
                int val = lookup[(uint8_t)encoded[i]];
                if (val == -1)
                    continue;
                triple = (triple << 6) | val;
                valid++;
            }
            if (valid >= 2)
                result += char((triple >> 16) & 0xFF);
            if (valid >= 3)
                result += char((triple >> 8) & 0xFF);
            if (valid >= 4)
                result += char(triple & 0xFF);
        }
        return result;
    }
};

class SimpleCiphers
{
public:
    static string rot13(const string &text)
    {
        CaesarCipher c;
        return c.encrypt(text, 13);
    }

    static string atbash(const string &text)
    {
        string result = text;
        for (char &c : result)
        {
            if (isupper(c))
                c = char('Z' - (c - 'A'));
            else if (islower(c))
                c = char('z' - (c - 'a'));
        }
        return result;
    }
};

class PasswordGenerator
{
public:
    string generate(size_t length)
    {
        const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()";
        string password;
        for (size_t i = 0; i < length; i++)
            password += chars[rand() % chars.size()];
        return password;
    }
};

class FrequencyAnalyzer
{
public:
    void analyze(const string &text)
    {
        map<char, int> freq;
        int totalLetters = 0;

        for (char c : text)
        {
            if (isalpha(c))
            {
                freq[toupper(c)]++;
                totalLetters++;
            }
        }

        cout << "\nLetter Frequencies:\n";
        cout << string(50, '-') << "\n";
        for (auto &p : freq)
        {
            double percent = (p.second * 100.0) / totalLetters;
            cout << "  " << p.first << ": " << setw(4) << p.second
                 << " (" << fixed << setprecision(2) << percent << "%)\n";
        }
        cout << "Total letters: " << totalLetters << "\n";

        if (!freq.empty())
        {
            auto maxFreq = max_element(freq.begin(), freq.end(),
                [](const pair<char, int> &a, const pair<char, int> &b)
                { return a.second < b.second; });
            int likelyShift = (maxFreq->first - 'E' + 26) % 26;
            cout << "\nMost frequent letter: '" << maxFreq->first
                 << "' (likely E in English, suggests Caesar shift ~" << likelyShift << ")\n";
        }
    }
};

class FileCrypto
{
private:
    XorCipher xorCipher;
    Base64Coder base64;

public:
    vector<uint8_t> readFile(const string &path)
    {
        ifstream file(path, ios::binary);
        if (!file)
            return {};

        file.seekg(0, ios::end);
        size_t size = file.tellg();
        file.seekg(0, ios::beg);

        vector<uint8_t> buffer(size);
        file.read(reinterpret_cast<char *>(buffer.data()), size);
        return buffer;
    }

    bool writeFile(const string &path, const vector<uint8_t> &data)
    {
        ofstream file(path, ios::binary);
        if (!file)
            return false;
        file.write(reinterpret_cast<const char *>(data.data()), data.size());
        return true;
    }

    bool encrypt(const string &input, const string &output,
                 const string &key, bool useBase64)
    {
        vector<uint8_t> data = readFile(input);
        if (data.empty())
            return false;

        string content(data.begin(), data.end());
        string result = xorCipher.encryptDecrypt(content, key);

        if (useBase64)
            result = base64.encode(result);

        vector<uint8_t> outData(result.begin(), result.end());
        return writeFile(output, outData);
    }

    bool decrypt(const string &input, const string &output,
                 const string &key, bool useBase64)
    {
        vector<uint8_t> data = readFile(input);
        if (data.empty())
            return false;

        string content(data.begin(), data.end());
        string result = content;

        if (useBase64)
        {
            try { result = base64.decode(content); }
            catch (...) { return false; }
        }

        result = xorCipher.encryptDecrypt(result, key);

        vector<uint8_t> outData(result.begin(), result.end());
        return writeFile(output, outData);
    }
};

class HashDemo
{
public:
    void showChecksums(const string &text)
    {
        uint32_t sum = 0;
        for (size_t i = 0; i < text.size(); i++)
            sum += (uint8_t)text[i] * (i + 1);

        uint32_t xorSum = 0;
        for (char c : text)
            xorSum ^= (uint8_t)c;

        uint32_t addSum = 0;
        for (char c : text)
            addSum += (uint8_t)c;

        cout << "  Addition Checksum:  0x" << hex << addSum << " (" << dec << addSum << ")\n";
        cout << "  XOR Checksum:       0x" << hex << xorSum << " (" << dec << xorSum << ")\n";
        cout << "  Weighted Checksum:  0x" << hex << sum << " (" << dec << sum << ")\n";
    }
};

void printHeader(const string &title)
{
    cout << "\n" << string(44, '=') << "\n";
    cout << "  " << title << "\n";
    cout << string(44, '=') << "\n";
}

void waitForEnter()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void textCipherMenu()
{
    CaesarCipher caesar;
    VigenereCipher vigenere;
    XorCipher xorCipher;
    Base64Coder base64;
    SimpleCiphers simple;
    FrequencyAnalyzer freq;

    int choice;
    do
    {
        printHeader("TEXT CIPHER TOOL");
        cout << "  1. Caesar Cipher\n";
        cout << "  2. ROT13\n";
        cout << "  3. Atbash Cipher\n";
        cout << "  4. Vigenere Cipher\n";
        cout << "  5. XOR Cipher\n";
        cout << "  6. Base64 Encode/Decode\n";
        cout << "  7. Caesar Brute Force\n";
        cout << "  8. Frequency Analysis\n";
        cout << "  0. Back to Main Menu\n";
        cout << "Choose: ";

        cin >> choice;
        cin.ignore();

        if (choice == 0)
            break;

        string text, key, mode;
        int shift;

        cout << "Enter text: ";
        getline(cin, text);

        if (choice >= 1 && choice <= 6)
        {
            cout << "Encrypt or Decrypt? (e/d): ";
            getline(cin, mode);
        }

        switch (choice)
        {
        case 1:
            cout << "Enter shift (1-25): ";
            cin >> shift;
            cin.ignore();
            if (toLower(mode)[0] == 'e')
                cout << "\nResult: " << caesar.encrypt(text, shift) << "\n";
            else
                cout << "\nResult: " << caesar.decrypt(text, shift) << "\n";
            break;
        case 2:
            cout << "\nResult: " << simple.rot13(text) << "\n";
            break;
        case 3:
            cout << "\nResult: " << simple.atbash(text) << "\n";
            break;
        case 4:
            cout << "Enter key: ";
            getline(cin, key);
            if (key.empty()) { cout << "Error: Key cannot be empty!\n"; break; }
            if (toLower(mode)[0] == 'e')
                cout << "\nResult: " << vigenere.encrypt(text, key) << "\n";
            else
                cout << "\nResult: " << vigenere.decrypt(text, key) << "\n";
            break;
        case 5:
            cout << "Enter key: ";
            getline(cin, key);
            if (key.empty()) { cout << "Error: Key cannot be empty!\n"; break; }
            cout << "\nResult: " << xorCipher.encryptDecrypt(text, key) << "\n";
            cout << "(Note: XOR is symmetric - same key encrypts and decrypts)\n";
            break;
        case 6:
            if (toLower(mode)[0] == 'e')
                cout << "\nResult: " << base64.encode(text) << "\n";
            else
                cout << "\nResult: " << base64.decode(text) << "\n";
            break;
        case 7:
            caesar.bruteForce(text);
            break;
        case 8:
            freq.analyze(text);
            break;
        }
    } while (false);
}

void fileCipherMenu()
{
    FileCrypto fileCrypto;
    PasswordGenerator pwGen;

    printHeader("FILE ENCRYPTION / DECRYPTION");

    int method;
    cout << "  1. XOR Cipher (fast, binary safe)\n";
    cout << "  2. XOR + Base64 (text-safe output)\n";
    cout << "  0. Back\n";
    cout << "Choose: ";
    cin >> method;
    cin.ignore();

    if (method == 0)
        return;

    string inputPath, outputPath, key;

    cout << "Enter input file path: ";
    getline(cin, inputPath);

    if (!fs::exists(inputPath))
    {
        cout << "Error: File does not exist!\n";
        return;
    }

    uintmax_t inSize = fs::file_size(inputPath);
    cout << "File size: " << getFileSizeString(inSize) << "\n";

    string mode;
    cout << "Encrypt or Decrypt? (e/d): ";
    getline(cin, mode);

    cout << "Enter key (leave empty to generate random): ";
    getline(cin, key);

    if (key.empty())
    {
        key = pwGen.generate(16);
        cout << "Generated random key: " << key << "\n";
        cout << "Key (hex): " << bytesToHex(vector<uint8_t>(key.begin(), key.end())) << "\n";
    }

    bool useBase64 = (method == 2);

    if (toLower(mode)[0] == 'e')
    {
        outputPath = inputPath + ENCRYPTED_EXT;
        if (fileCrypto.encrypt(inputPath, outputPath, key, useBase64))
        {
            uintmax_t outSize = fs::file_size(outputPath);
            cout << "Encrypted: " << outputPath << "\n";
            cout << "Size: " << getFileSizeString(inSize) << " -> "
                 << getFileSizeString(outSize) << "\n";
            cout << "WARNING: Keep your key safe! Key: " << key << "\n";
        }
        else
            cout << "Error: Encryption failed!\n";
    }
    else
    {
        outputPath = inputPath + DECRYPTED_EXT;
        if (fileCrypto.decrypt(inputPath, outputPath, key, useBase64))
        {
            uintmax_t outSize = fs::file_size(outputPath);
            cout << "Decrypted: " << outputPath << "\n";
            cout << "Size: " << getFileSizeString(outSize) << "\n";
        }
        else
            cout << "Error: Decryption failed! Wrong key or corrupted file?\n";
    }
}

void passwordMenu()
{
    PasswordGenerator pwGen;

    printHeader("PASSWORD GENERATOR");

    int length;
    cout << "Enter password length (8-128): ";
    cin >> length;
    cin.ignore();

    length = max(8, min(128, length));

    string password = pwGen.generate(length);
    cout << "\nPassword: " << password << "\n";
    cout << "Length:   " << password.length() << " characters\n";
    cout << "Hex:      " << bytesToHex(vector<uint8_t>(password.begin(), password.end())) << "\n";
}

void hashMenu()
{
    HashDemo demo;

    printHeader("CHECKSUM DEMO");

    string text;
    cout << "Enter text: ";
    getline(cin, text);

    demo.showChecksums(text);
}

void keyGeneratorMenu()
{
    PasswordGenerator pwGen;

    printHeader("ENCRYPTION KEY GENERATOR");

    int length;
    cout << "Enter key length in bytes (16-64): ";
    cin >> length;
    cin.ignore();

    length = max(16, min(64, length));

    string key = pwGen.generate(length);
    cout << "\nGenerated Key: " << key << "\n";
    cout << "Key (hex):     " << bytesToHex(vector<uint8_t>(key.begin(), key.end())) << "\n";
    cout << "Key length:    " << key.length() << " characters\n";
}

int main()
{
    srand((unsigned int)time(nullptr));

    int choice;
    do
    {
        printHeader("ENCRYPTION / DECRYPTION TOOL");
        cout << "  1. Text Ciphers (Caesar, Vigenere, XOR, Base64)\n";
        cout << "  2. File Encryption / Decryption\n";
        cout << "  3. Password Generator\n";
        cout << "  4. Checksum Demo\n";
        cout << "  5. Encryption Key Generator\n";
        cout << "  0. Exit\n";
        cout << string(44, '=') << "\n";
        cout << "Choose: ";

        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1: textCipherMenu(); break;
        case 2: fileCipherMenu(); break;
        case 3: passwordMenu(); break;
        case 4: hashMenu(); break;
        case 5: keyGeneratorMenu(); break;
        case 0: cout << "Goodbye!\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}
