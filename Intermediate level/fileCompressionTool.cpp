#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <queue>

using namespace std;
namespace fs = filesystem;

const string COMPRESSED_EXT = ".cmp";
const string DECOMPRESSED_EXT = ".dec";

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

class CompressionStats
{
public:
    uintmax_t originalSize;
    uintmax_t compressedSize;
    double ratio;
    double elapsedSeconds;

    CompressionStats()
        : originalSize(0), compressedSize(0), ratio(0), elapsedSeconds(0) {}

    void calculate()
    {
        if (originalSize > 0)
            ratio = 100.0 - (compressedSize * 100.0 / originalSize);
    }

    void display() const
    {
        cout << "\n--- Compression Statistics ---\n";
        cout << left << setw(25) << "Original Size:" << getFileSizeString(originalSize) << endl;
        cout << left << setw(25) << "Compressed Size:" << getFileSizeString(compressedSize) << endl;
        cout << left << setw(25) << "Space Saved:" << fixed << setprecision(2) << ratio << "%" << endl;
        cout << left << setw(25) << "Time Taken:" << fixed << setprecision(3) << elapsedSeconds << "s" << endl;
    }
};

class RunLengthEncoder
{
public:
    static bool compress(const string &inputFile, const string &outputFile)
    {
        ifstream in(inputFile, ios::binary);
        if (!in.is_open())
        {
            cout << "Error: Cannot open input file.\n";
            return false;
        }

        ofstream out(outputFile, ios::binary);
        if (!out.is_open())
        {
            cout << "Error: Cannot create output file.\n";
            return false;
        }

        out.put('R');
        out.put('L');
        out.put('E');

        char prevChar;
        char currentChar;
        int count = 0;
        bool first = true;

        while (in.get(currentChar))
        {
            if (first)
            {
                prevChar = currentChar;
                count = 1;
                first = false;
                continue;
            }

            if (currentChar == prevChar && count < 255)
            {
                count++;
            }
            else
            {
                out.put(static_cast<char>(count));
                out.put(prevChar);
                prevChar = currentChar;
                count = 1;
            }
        }

        if (!first)
        {
            out.put(static_cast<char>(count));
            out.put(prevChar);
        }

        in.close();
        out.close();
        return true;
    }

    static bool decompress(const string &inputFile, const string &outputFile)
    {
        ifstream in(inputFile, ios::binary);
        if (!in.is_open())
        {
            cout << "Error: Cannot open compressed file.\n";
            return false;
        }

        char h1, h2, h3;
        in.get(h1);
        in.get(h2);
        in.get(h3);
        if (h1 != 'R' || h2 != 'L' || h3 != 'E')
        {
            cout << "Error: Invalid RLE compressed file format.\n";
            return false;
        }

        ofstream out(outputFile, ios::binary);
        if (!out.is_open())
        {
            cout << "Error: Cannot create output file.\n";
            return false;
        }

        char countChar, dataChar;
        while (in.get(countChar))
        {
            if (!in.get(dataChar))
                break;

            unsigned char count = static_cast<unsigned char>(countChar);
            for (int i = 0; i < count; i++)
                out.put(dataChar);
        }

        in.close();
        out.close();
        return true;
    }
};

struct HuffmanNode
{
    char data;
    unsigned freq;
    HuffmanNode *left;
    HuffmanNode *right;

    HuffmanNode(char d, unsigned f) : data(d), freq(f), left(nullptr), right(nullptr) {}
};

struct CompareNode
{
    bool operator()(HuffmanNode *a, HuffmanNode *b)
    {
        return a->freq > b->freq;
    }
};

class HuffmanEncoder
{
private:
    map<char, string> huffmanCodes;
    map<char, unsigned> freqTable;
    HuffmanNode *root;

    void buildFreqTable(const string &inputFile)
    {
        ifstream in(inputFile, ios::binary);
        char c;
        while (in.get(c))
            freqTable[c]++;
        in.close();
    }

    void buildTree()
    {
        priority_queue<HuffmanNode *, vector<HuffmanNode *>, CompareNode> pq;

        for (auto &pair : freqTable)
            pq.push(new HuffmanNode(pair.first, pair.second));

        if (pq.size() == 1)
        {
            HuffmanNode *temp = pq.top();
            pq.pop();
            HuffmanNode *internal = new HuffmanNode('\0', temp->freq);
            internal->left = temp;
            pq.push(internal);
        }

        while (pq.size() > 1)
        {
            HuffmanNode *left = pq.top();
            pq.pop();
            HuffmanNode *right = pq.top();
            pq.pop();

            HuffmanNode *internal = new HuffmanNode('\0', left->freq + right->freq);
            internal->left = left;
            internal->right = right;
            pq.push(internal);
        }

        if (!pq.empty())
            root = pq.top();
        else
            root = nullptr;
    }

    void generateCodes(HuffmanNode *node, string code)
    {
        if (!node)
            return;

        if (!node->left && !node->right)
        {
            huffmanCodes[node->data] = code;
            return;
        }

        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }

    void destroyTree(HuffmanNode *node)
    {
        if (!node)
            return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    string bitsToBytes(const string &bits)
    {
        string bytes;
        for (size_t i = 0; i < bits.size(); i += 8)
        {
            unsigned char byte = 0;
            for (size_t j = 0; j < 8 && i + j < bits.size(); j++)
            {
                if (bits[i + j] == '1')
                    byte |= (1 << (7 - j));
            }
            bytes += byte;
        }
        return bytes;
    }

    string bytesToBits(const string &bytes, size_t totalBits)
    {
        string bits;
        for (size_t i = 0; i < bytes.size(); i++)
        {
            unsigned char byte = static_cast<unsigned char>(bytes[i]);
            for (int j = 7; j >= 0; j--)
            {
                if (bits.size() >= totalBits)
                    break;
                bits += (byte & (1 << j)) ? '1' : '0';
            }
        }
        return bits;
    }

public:
    HuffmanEncoder() : root(nullptr) {}

    ~HuffmanEncoder()
    {
        destroyTree(root);
    }

    bool compress(const string &inputFile, const string &outputFile)
    {
        freqTable.clear();
        huffmanCodes.clear();
        destroyTree(root);
        root = nullptr;

        buildFreqTable(inputFile);

        if (freqTable.empty())
        {
            cout << "Error: Input file is empty.\n";
            return false;
        }

        buildTree();
        generateCodes(root, "");

        ifstream in(inputFile, ios::binary);
        if (!in.is_open())
        {
            cout << "Error: Cannot open input file.\n";
            return false;
        }

        string encodedBits;
        char c;
        while (in.get(c))
            encodedBits += huffmanCodes[c];
        in.close();

        ofstream out(outputFile, ios::binary);
        if (!out.is_open())
        {
            cout << "Error: Cannot create output file.\n";
            return false;
        }

        out.put('H');
        out.put('U');
        out.put('F');

        unsigned short tableSize = static_cast<unsigned short>(freqTable.size());
        out.write(reinterpret_cast<const char *>(&tableSize), sizeof(tableSize));

        for (auto &pair : freqTable)
        {
            out.put(pair.first);
            out.write(reinterpret_cast<const char *>(&pair.second), sizeof(pair.second));
        }

        unsigned int bitCount = static_cast<unsigned int>(encodedBits.size());
        out.write(reinterpret_cast<const char *>(&bitCount), sizeof(bitCount));

        string byteData = bitsToBytes(encodedBits);
        out.write(byteData.c_str(), byteData.size());

        out.close();
        return true;
    }

    bool decompress(const string &inputFile, const string &outputFile)
    {
        ifstream in(inputFile, ios::binary);
        if (!in.is_open())
        {
            cout << "Error: Cannot open compressed file.\n";
            return false;
        }

        char h1, h2, h3;
        in.get(h1);
        in.get(h2);
        in.get(h3);
        if (h1 != 'H' || h2 != 'U' || h3 != 'F')
        {
            cout << "Error: Invalid Huffman compressed file format.\n";
            return false;
        }

        freqTable.clear();
        huffmanCodes.clear();
        destroyTree(root);
        root = nullptr;

        unsigned short tableSize;
        in.read(reinterpret_cast<char *>(&tableSize), sizeof(tableSize));

        for (int i = 0; i < tableSize; i++)
        {
            char data;
            unsigned freq;
            in.get(data);
            in.read(reinterpret_cast<char *>(&freq), sizeof(freq));
            freqTable[data] = freq;
        }

        buildTree();
        generateCodes(root, "");

        unsigned int bitCount;
        in.read(reinterpret_cast<char *>(&bitCount), sizeof(bitCount));

        string byteData((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
        in.close();

        string encodedBits = bytesToBits(byteData, bitCount);

        ofstream out(outputFile, ios::binary);
        if (!out.is_open())
        {
            cout << "Error: Cannot create output file.\n";
            return false;
        }

        HuffmanNode *current = root;
        for (char bit : encodedBits)
        {
            if (bit == '0')
                current = current->left;
            else
                current = current->right;

            if (current && !current->left && !current->right)
            {
                out.put(current->data);
                current = root;
            }
        }

        out.close();
        return true;
    }
};

class DictionaryEncoder
{
public:
    static bool compress(const string &inputFile, const string &outputFile)
    {
        ifstream in(inputFile);
        if (!in.is_open())
        {
            cout << "Error: Cannot open input file.\n";
            return false;
        }

        string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
        in.close();

        vector<string> words;
        string current;
        for (char c : content)
        {
            if (isalnum(c) || c == '_')
            {
                current += c;
            }
            else
            {
                if (!current.empty())
                {
                    words.push_back(current);
                    current.clear();
                }
                words.push_back(string(1, c));
            }
        }
        if (!current.empty())
            words.push_back(current);

        map<string, unsigned short> dictionary;
        vector<pair<unsigned short, string>> encoded;

        for (const string &word : words)
        {
            if (word.length() <= 1 && !isalnum(word[0]))
            {
                encoded.push_back({0, word});
                continue;
            }

            auto it = dictionary.find(word);
            if (it != dictionary.end())
            {
                encoded.push_back({it->second, ""});
            }
            else
            {
                unsigned short idx = static_cast<unsigned short>(dictionary.size() + 1);
                dictionary[word] = idx;
                encoded.push_back({idx, word});
            }
        }

        ofstream out(outputFile, ios::binary);
        if (!out.is_open())
        {
            cout << "Error: Cannot create output file.\n";
            return false;
        }

        out.put('D');
        out.put('I');
        out.put('C');

        unsigned short dictSize = static_cast<unsigned short>(dictionary.size());
        out.write(reinterpret_cast<const char *>(&dictSize), sizeof(dictSize));

        for (auto &pair : dictionary)
        {
            unsigned short idx = pair.second;
            out.write(reinterpret_cast<const char *>(&idx), sizeof(idx));
            unsigned short wordLen = static_cast<unsigned short>(pair.first.length());
            out.write(reinterpret_cast<const char *>(&wordLen), sizeof(wordLen));
            out.write(pair.first.c_str(), wordLen);
        }

        unsigned int encodedCount = static_cast<unsigned int>(encoded.size());
        out.write(reinterpret_cast<const char *>(&encodedCount), sizeof(encodedCount));

        for (auto &entry : encoded)
        {
            out.write(reinterpret_cast<const char *>(&entry.first), sizeof(entry.first));
            if (entry.first == 0)
            {
                unsigned short charLen = static_cast<unsigned short>(entry.second.length());
                out.write(reinterpret_cast<const char *>(&charLen), sizeof(charLen));
                out.write(entry.second.c_str(), charLen);
            }
        }

        out.close();
        return true;
    }

    static bool decompress(const string &inputFile, const string &outputFile)
    {
        ifstream in(inputFile, ios::binary);
        if (!in.is_open())
        {
            cout << "Error: Cannot open compressed file.\n";
            return false;
        }

        char h1, h2, h3;
        in.get(h1);
        in.get(h2);
        in.get(h3);
        if (h1 != 'D' || h2 != 'I' || h3 != 'C')
        {
            cout << "Error: Invalid Dictionary compressed file format.\n";
            return false;
        }

        map<unsigned short, string> reverseDict;
        unsigned short dictSize;
        in.read(reinterpret_cast<char *>(&dictSize), sizeof(dictSize));

        for (int i = 0; i < dictSize; i++)
        {
            unsigned short idx, wordLen;
            in.read(reinterpret_cast<char *>(&idx), sizeof(idx));
            in.read(reinterpret_cast<char *>(&wordLen), sizeof(wordLen));
            string word(wordLen, '\0');
            in.read(&word[0], wordLen);
            reverseDict[idx] = word;
        }

        unsigned int encodedCount;
        in.read(reinterpret_cast<char *>(&encodedCount), sizeof(encodedCount));

        ofstream out(outputFile);
        if (!out.is_open())
        {
            cout << "Error: Cannot create output file.\n";
            return false;
        }

        for (unsigned int i = 0; i < encodedCount; i++)
        {
            unsigned short idx;
            in.read(reinterpret_cast<char *>(&idx), sizeof(idx));

            if (idx == 0)
            {
                unsigned short charLen;
                in.read(reinterpret_cast<char *>(&charLen), sizeof(charLen));
                string literal(charLen, '\0');
                in.read(&literal[0], charLen);
                out << literal;
            }
            else
            {
                out << reverseDict[idx];
            }
        }

        in.close();
        out.close();
        return true;
    }
};

class FileCompressionTool
{
private:
    void displayHeader() const
    {
        cout << "\n========================================\n";
        cout << "        FILE COMPRESSION TOOL          \n";
        cout << "========================================\n";
    }

    bool fileExists(const string &path) const
    {
        return fs::exists(path);
    }

    uintmax_t getFileSize(const string &path) const
    {
        if (!fileExists(path))
            return 0;
        return fs::file_size(path);
    }

    string stripExtension(const string &filename) const
    {
        size_t dot = filename.find_last_of('.');
        if (dot == string::npos)
            return filename;
        return filename.substr(0, dot);
    }

    CompressionStats performCompression(const string &inputFile, int method)
    {
        CompressionStats stats;
        stats.originalSize = getFileSize(inputFile);

        if (!fileExists(inputFile))
        {
            cout << "Error: Input file does not exist.\n";
            return stats;
        }

        string baseName = stripExtension(inputFile);
        string outputFile = baseName + COMPRESSED_EXT;

        clock_t start = clock();
        bool success = false;

        switch (method)
        {
        case 1:
            cout << "Using RLE compression...\n";
            success = RunLengthEncoder::compress(inputFile, outputFile);
            break;
        case 2:
            cout << "Using Huffman compression...\n";
            {
                HuffmanEncoder encoder;
                success = encoder.compress(inputFile, outputFile);
            }
            break;
        case 3:
            cout << "Using Dictionary compression...\n";
            success = DictionaryEncoder::compress(inputFile, outputFile);
            break;
        }

        clock_t end = clock();
        stats.elapsedSeconds = static_cast<double>(end - start) / CLOCKS_PER_SEC;

        if (success)
        {
            stats.compressedSize = getFileSize(outputFile);
            stats.calculate();

            cout << "Compressed to: " << outputFile << endl;
            stats.display();
        }
        else
        {
            cout << "Compression failed.\n";
        }

        return stats;
    }

    CompressionStats performDecompression(const string &inputFile)
    {
        CompressionStats stats;

        if (!fileExists(inputFile))
        {
            cout << "Error: Compressed file does not exist.\n";
            return stats;
        }

        ifstream detect(inputFile, ios::binary);
        char h1, h2, h3;
        detect.get(h1);
        detect.get(h2);
        detect.get(h3);
        detect.close();

        string baseName = stripExtension(inputFile);
        string outputFile = baseName + DECOMPRESSED_EXT;

        clock_t start = clock();
        bool success = false;

        if (h1 == 'R' && h2 == 'L' && h3 == 'E')
        {
            cout << "Detected RLE format. Decompressing...\n";
            success = RunLengthEncoder::decompress(inputFile, outputFile);
        }
        else if (h1 == 'H' && h2 == 'U' && h3 == 'F')
        {
            cout << "Detected Huffman format. Decompressing...\n";
            HuffmanEncoder encoder;
            success = encoder.decompress(inputFile, outputFile);
        }
        else if (h1 == 'D' && h2 == 'I' && h3 == 'C')
        {
            cout << "Detected Dictionary format. Decompressing...\n";
            success = DictionaryEncoder::decompress(inputFile, outputFile);
        }
        else
        {
            cout << "Error: Unknown or invalid compressed file format.\n";
            return stats;
        }

        clock_t end = clock();
        stats.elapsedSeconds = static_cast<double>(end - start) / CLOCKS_PER_SEC;

        if (success)
        {
            stats.compressedSize = getFileSize(inputFile);
            stats.originalSize = getFileSize(outputFile);
            stats.calculate();

            cout << "Decompressed to: " << outputFile << endl;
            stats.display();
        }
        else
        {
            cout << "Decompression failed.\n";
        }

        return stats;
    }

    void analyzeFile(const string &inputFile) const
    {
        if (!fileExists(inputFile))
        {
            cout << "Error: File does not exist.\n";
            return;
        }

        uintmax_t size = getFileSize(inputFile);

        ifstream in(inputFile, ios::binary);
        map<unsigned char, uintmax_t> freq;
        uintmax_t totalChars = 0;
        unsigned char c;

        while (in.read(reinterpret_cast<char *>(&c), sizeof(c)))
        {
            freq[c]++;
            totalChars++;
        }
        in.close();

        double entropy = 0.0;
        for (auto &pair : freq)
        {
            double p = static_cast<double>(pair.second) / totalChars;
            if (p > 0)
                entropy -= p * log2(p);
        }

        cout << "\n--- File Analysis ---\n";
        cout << left << setw(25) << "File:" << inputFile << endl;
        cout << left << setw(25) << "Size:" << getFileSizeString(size) << endl;
        cout << left << setw(25) << "Unique Characters:" << freq.size() << endl;
        cout << left << setw(25) << "Total Characters:" << totalChars << endl;
        cout << left << setw(25) << "Entropy:" << fixed << setprecision(4) << entropy << " bits/char" << endl;
        cout << left << setw(25) << "Estimated Min Size:"
             << getFileSizeString(static_cast<uintmax_t>(ceil(entropy * totalChars / 8.0))) << endl;

        if (freq.size() <= 20)
        {
            cout << "\nCharacter Frequency Table:\n";
            cout << left << setw(10) << "Char" << setw(15) << "Count" << "Frequency" << endl;
            cout << string(50, '-') << endl;
            for (auto &pair : freq)
            {
                char display = isprint(pair.first) ? pair.first : '.';
                double pct = static_cast<double>(pair.second) * 100.0 / totalChars;
                cout << left << setw(10) << display
                     << setw(15) << pair.second
                     << fixed << setprecision(2) << pct << "%" << endl;
            }
        }
    }

    void batchCompress(const vector<string> &files, int method)
    {
        int successCount = 0;
        for (const string &file : files)
        {
            cout << "\nProcessing: " << file << endl;
            cout << string(40, '-') << endl;

            CompressionStats stats = performCompression(file, method);
            if (stats.compressedSize > 0)
                successCount++;
        }

        cout << "\nBatch complete. " << successCount << "/" << files.size() << " files compressed.\n";
    }

    int selectMethod() const
    {
        int method;
        cout << "\nSelect compression method:\n";
        cout << "1. Run-Length Encoding (RLE) - Best for repetitive data\n";
        cout << "2. Huffman Coding - Best for text with skewed distributions\n";
        cout << "3. Dictionary Encoding - Best for text with repeated words\n";
        cout << "Enter choice (1-3): ";
        cin >> method;
        return method;
    }

public:
    void run()
    {
        int choice;
        do
        {
            displayHeader();
            cout << "1.  Compress Single File\n";
            cout << "2.  Decompress File\n";
            cout << "3.  Analyze File\n";
            cout << "4.  Batch Compress Files\n";
            cout << "5.  Compare All Methods\n";
            cout << "0.  Exit\n";
            cout << "========================================\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                string filename;
                cout << "Enter file to compress: ";
                cin >> filename;
                int method = selectMethod();
                if (method >= 1 && method <= 3)
                    performCompression(filename, method);
                else
                    cout << "Invalid method.\n";
                break;
            }
            case 2:
            {
                string filename;
                cout << "Enter compressed file to decompress: ";
                cin >> filename;
                performDecompression(filename);
                break;
            }
            case 3:
            {
                string filename;
                cout << "Enter file to analyze: ";
                cin >> filename;
                analyzeFile(filename);
                break;
            }
            case 4:
            {
                int fileCount;
                cout << "How many files to compress? ";
                cin >> fileCount;

                vector<string> files;
                for (int i = 0; i < fileCount; i++)
                {
                    string filename;
                    cout << "File " << (i + 1) << ": ";
                    cin >> filename;
                    files.push_back(filename);
                }

                int method = selectMethod();
                if (method >= 1 && method <= 3)
                    batchCompress(files, method);
                else
                    cout << "Invalid method.\n";
                break;
            }
            case 5:
            {
                string filename;
                cout << "Enter file to compress: ";
                cin >> filename;

                if (!fileExists(filename))
                {
                    cout << "Error: File does not exist.\n";
                    break;
                }

                cout << "\nComparing all compression methods...\n";
                cout << string(50, '=') << endl;

                CompressionStats rleStats = performCompression(filename, 1);
                CompressionStats huffStats = performCompression(filename, 2);
                CompressionStats dictStats = performCompression(filename, 3);

                cout << "\n=== Comparison Summary ===\n";
                cout << left << setw(30) << "Method" << setw(20) << "Compressed Size" << "Space Saved" << endl;
                cout << string(70, '-') << endl;
                cout << left << setw(30) << "Original" << setw(20) << getFileSizeString(rleStats.originalSize) << "-" << endl;
                cout << left << setw(30) << "RLE"
                     << setw(20) << getFileSizeString(rleStats.compressedSize)
                     << fixed << setprecision(2) << rleStats.ratio << "%" << endl;
                cout << left << setw(30) << "Huffman"
                     << setw(20) << getFileSizeString(huffStats.compressedSize)
                     << fixed << setprecision(2) << huffStats.ratio << "%" << endl;
                cout << left << setw(30) << "Dictionary"
                     << setw(20) << getFileSizeString(dictStats.compressedSize)
                     << fixed << setprecision(2) << dictStats.ratio << "%" << endl;
                break;
            }
            case 0:
                cout << "Exiting File Compression Tool. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 0);
    }
};

int main()
{
    FileCompressionTool tool;
    tool.run();
    return 0;
}
