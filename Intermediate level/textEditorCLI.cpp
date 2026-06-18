#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <conio.h>
#include <windows.h>
#include <memory>
#include <algorithm>
#include <sstream>
#include <stack>

using namespace std;

struct EditorState {
    vector<string> lines;
    int cursorRow;
    int cursorCol;
    int topLine;
    string filename;
    bool modified;
};

struct UndoAction {
    enum Type { INSERT_CHAR, DELETE_CHAR, NEW_LINE, MERGE_LINE, PASTE, CUT };
    Type type;
    int row;
    int col;
    char ch;
    string text;
    int count;
};

class TextEditor {
private:
    vector<string> lines;
    int cursorRow, cursorCol;
    int topLine;
    string filename;
    bool modified;
    int screenRows, screenCols;

    stack<UndoAction> undoStack;
    stack<UndoAction> redoStack;

    string clipboard;

    const char* STATUS_BG = " \x1b[44m";
    const char* RESET = "\x1b[0m";
    const char* CURSOR_BG = "\x1b[47m\x1b[30m";

    void getConsoleSize() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        screenCols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        screenRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }

    void clearScreen() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        DWORD written;
        DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
        FillConsoleOutputCharacter(hConsole, ' ', cells, {0, 0}, &written);
        FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cells, {0, 0}, &written);
        SetConsoleCursorPosition(hConsole, {0, 0});
    }

    void pushUndo(UndoAction action) {
        undoStack.push(action);
        while (!redoStack.empty()) redoStack.pop();
    }

    void setCursorVisible(bool visible) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = visible;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

    int lineNumberWidth() const {
        int total = lines.size();
        if (total == 0) return 2;
        return to_string(total).length() + 1;
    }

public:
    TextEditor(const string& file = "") : cursorRow(0), cursorCol(0), topLine(0), modified(false) {
        if (!file.empty()) {
            filename = file;
            loadFile();
        } else {
            filename = "Untitled";
            lines.push_back("");
        }
    }

    void loadFile() {
        ifstream fin(filename);
        if (!fin) {
            lines.push_back("");
            return;
        }
        string line;
        while (getline(fin, line)) {
            lines.push_back(line);
        }
        if (lines.empty()) lines.push_back("");
        fin.close();
    }

    void saveFile() {
        if (filename == "Untitled") {
            cout << "\n\x1b[33mFilename to save as: \x1b[0m";
            getline(cin, filename);
        }
        ofstream fout(filename);
        for (size_t i = 0; i < lines.size(); i++) {
            fout << lines[i];
            if (i != lines.size() - 1) fout << '\n';
        }
        fout.close();
        modified = false;
    }

    void saveAs(const string& name) {
        filename = name;
        saveFile();
    }

    void insertChar(char ch) {
        if (ch == '\t') {
            for (int i = 0; i < 4; i++) insertChar(' ');
            return;
        }
        pushUndo({UndoAction::INSERT_CHAR, cursorRow, cursorCol, ch, "", 0});
        lines[cursorRow].insert(cursorCol, 1, ch);
        cursorCol++;
        modified = true;
    }

    void newLine() {
        pushUndo({UndoAction::NEW_LINE, cursorRow, cursorCol, 0, lines[cursorRow].substr(cursorCol), 0});
        string rest = lines[cursorRow].substr(cursorCol);
        lines[cursorRow] = lines[cursorRow].substr(0, cursorCol);
        lines.insert(lines.begin() + cursorRow + 1, rest);
        cursorRow++;
        cursorCol = 0;
        modified = true;
    }

    void deleteChar() {
        if (cursorCol < (int)lines[cursorRow].length()) {
            pushUndo({UndoAction::DELETE_CHAR, cursorRow, cursorCol, lines[cursorRow][cursorCol], "", 0});
            lines[cursorRow].erase(cursorCol, 1);
            modified = true;
        } else if (cursorRow + 1 < (int)lines.size()) {
            pushUndo({UndoAction::MERGE_LINE, cursorRow, cursorCol, 0, lines[cursorRow + 1], (int)lines[cursorRow].length()});
            lines[cursorRow] += lines[cursorRow + 1];
            lines.erase(lines.begin() + cursorRow + 1);
            modified = true;
        }
    }

    void backspace() {
        if (cursorCol > 0) {
            cursorCol--;
            pushUndo({UndoAction::DELETE_CHAR, cursorRow, cursorCol, lines[cursorRow][cursorCol], "", 0});
            lines[cursorRow].erase(cursorCol, 1);
            modified = true;
        } else if (cursorRow > 0) {
            cursorRow--;
            cursorCol = lines[cursorRow].length();
            pushUndo({UndoAction::MERGE_LINE, cursorRow, cursorCol, 0, lines[cursorRow + 1], cursorCol});
            lines[cursorRow] += lines[cursorRow + 1];
            lines.erase(lines.begin() + cursorRow + 1);
            modified = true;
        }
    }

    void moveCursorUp() {
        if (cursorRow > 0) {
            cursorRow--;
            if (cursorRow < topLine) topLine = cursorRow;
            if (cursorCol > (int)lines[cursorRow].length())
                cursorCol = lines[cursorRow].length();
        }
    }

    void moveCursorDown() {
        if (cursorRow + 1 < (int)lines.size()) {
            cursorRow++;
            if (cursorRow >= topLine + screenRows - 2)
                topLine = cursorRow - screenRows + 3;
            if (cursorCol > (int)lines[cursorRow].length())
                cursorCol = lines[cursorRow].length();
        }
    }

    void moveCursorLeft() {
        if (cursorCol > 0) {
            cursorCol--;
        } else if (cursorRow > 0) {
            cursorRow--;
            cursorCol = lines[cursorRow].length();
        }
    }

    void moveCursorRight() {
        if (cursorCol < (int)lines[cursorRow].length()) {
            cursorCol++;
        } else if (cursorRow + 1 < (int)lines.size()) {
            cursorRow++;
            cursorCol = 0;
        }
    }

    void moveHome() { cursorCol = 0; }
    void moveEnd() { cursorCol = lines[cursorRow].length(); }

    void copySelection(int startRow, int startCol, int endRow, int endCol) {
        if (startRow == endRow) {
            clipboard = lines[startRow].substr(startCol, endCol - startCol);
        } else {
            clipboard = lines[startRow].substr(startCol) + "\n";
            for (int r = startRow + 1; r < endRow; r++)
                clipboard += lines[r] + "\n";
            clipboard += lines[endRow].substr(0, endCol);
        }
    }

    void pasteClipboard() {
        if (clipboard.empty()) return;
        pushUndo({UndoAction::PASTE, cursorRow, cursorCol, 0, clipboard, 0});
        stringstream ss(clipboard);
        string line;
        getline(ss, line);
        string firstPart = lines[cursorRow].substr(0, cursorCol);
        string lastPart = lines[cursorRow].substr(cursorCol);
        lines[cursorRow] = firstPart + line;
        int originalRow = cursorRow;
        while (getline(ss, line)) {
            cursorRow++;
            lines.insert(lines.begin() + cursorRow, line);
        }
        lines[cursorRow] += lastPart;
        cursorCol = lines[cursorRow].length() - lastPart.length();
        modified = true;
    }

    void undo() {
        if (undoStack.empty()) return;
        UndoAction act = undoStack.top(); undoStack.pop();
        redoStack.push(act);

        switch (act.type) {
            case UndoAction::INSERT_CHAR:
                cursorRow = act.row; cursorCol = act.col;
                lines[cursorRow].erase(cursorCol, 1);
                break;
            case UndoAction::DELETE_CHAR:
                cursorRow = act.row; cursorCol = act.col;
                lines[cursorRow].insert(cursorCol, 1, act.ch);
                break;
            case UndoAction::NEW_LINE:
                cursorRow = act.row; cursorCol = act.col;
                lines[cursorRow] += act.text;
                lines.erase(lines.begin() + cursorRow + 1);
                break;
            case UndoAction::MERGE_LINE:
                cursorRow = act.row; cursorCol = act.col;
                lines[cursorRow] = lines[cursorRow].substr(0, act.col);
                lines.insert(lines.begin() + cursorRow + 1, act.text);
                break;
            case UndoAction::PASTE:
                break;
            default: break;
        }
        modified = true;
    }

    void redo() {
        if (redoStack.empty()) return;
        UndoAction act = redoStack.top(); redoStack.pop();
        undoStack.push(act);

        switch (act.type) {
            case UndoAction::INSERT_CHAR:
                cursorRow = act.row; cursorCol = act.col;
                lines[cursorRow].insert(cursorCol, 1, act.ch);
                cursorCol++;
                break;
            case UndoAction::DELETE_CHAR:
                cursorRow = act.row; cursorCol = act.col;
                lines[cursorRow].erase(cursorCol, 1);
                break;
            case UndoAction::NEW_LINE:
                cursorRow = act.row; cursorCol = act.col;
                newLine();
                break;
            case UndoAction::MERGE_LINE:
                cursorRow = act.row; cursorCol = act.col;
                deleteChar();
                break;
            default: break;
        }
        modified = true;
    }

    bool findText(const string& search, bool forward = true) {
        for (size_t r = 0; r < lines.size(); r++) {
            size_t pos = lines[r].find(search);
            if (pos != string::npos) {
                cursorRow = r;
                cursorCol = pos;
                return true;
            }
        }
        return false;
    }

    int findTextAll(const string& search) {
        int count = 0;
        for (const auto& line : lines) {
            size_t pos = 0;
            while ((pos = line.find(search, pos)) != string::npos) {
                count++;
                pos += search.length();
            }
        }
        return count;
    }

    void goToLine(int line) {
        if (line < 1) line = 1;
        if (line > (int)lines.size()) line = lines.size();
        cursorRow = line - 1;
        cursorCol = 0;
        if (cursorRow < topLine) topLine = cursorRow;
        if (cursorRow >= topLine + screenRows - 2)
            topLine = cursorRow - screenRows + 3;
    }

    void render() {
        clearScreen();
        getConsoleSize();
        int lw = lineNumberWidth();
        int editRows = screenRows - 1;

        for (int r = 0; r < editRows; r++) {
            int lineIdx = topLine + r;
            cout << "\x1b[90m";
            if (lineIdx < (int)lines.size()) {
                cout.width(lw);
                cout << right << (lineIdx + 1);
            } else {
                cout.width(lw);
                cout << "~";
            }
            cout << RESET;

            if (lineIdx < (int)lines.size()) {
                string line = lines[lineIdx];
                if (line.length() > screenCols - lw - 1)
                    line = line.substr(0, screenCols - lw - 1);
                cout << " " << line;
            }
            cout << "\r\n";
        }

        cout << STATUS_BG;
        cout << " " << filename;
        if (modified) cout << " [modified]";
        cout << "  |  Row: " << (cursorRow + 1) << " Col: " << (cursorCol + 1) << "  |  Lines: " << lines.size();
        cout << "  |  Ctrl+S:Save  Ctrl+O:Open  Ctrl+F:Find  Ctrl+G:GoTo  Ctrl+Z:Undo  Ctrl+Y:Redo  Ctrl+Q:Quit";
        cout << RESET;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        int displayRow = cursorRow - topLine;
        if (displayRow >= 0 && displayRow < editRows) {
            COORD pos = {(SHORT)(lw + 1 + cursorCol), (SHORT)displayRow};
            SetConsoleCursorPosition(hConsole, pos);
        }
    }

    void openFile(const string& file) {
        filename = file.empty() ? "Untitled" : file;
        lines.clear();
        cursorRow = cursorCol = topLine = 0;
        modified = false;
        while (!undoStack.empty()) undoStack.pop();
        while (!redoStack.empty()) redoStack.pop();
        if (!file.empty()) loadFile();
    }

    void run() {
        setCursorVisible(false);
        bool running = true;

        while (running) {
            render();

            if (!_kbhit()) continue;
            int ch = _getch();

            if (ch == 0xE0 || ch == 0) {
                ch = _getch();
                switch (ch) {
                    case 72: moveCursorUp(); break;
                    case 80: moveCursorDown(); break;
                    case 75: moveCursorLeft(); break;
                    case 77: moveCursorRight(); break;
                    case 71: moveHome(); break;
                    case 79: moveEnd(); break;
                    case 83: deleteChar(); break;
                }
            } else if (ch == 13) {
                newLine();
            } else if (ch == 8) {
                backspace();
            } else if (ch == 27) {
                running = false;
            } else if (ch == 9) {
                insertChar('\t');
            } else if (ch >= 32 && ch <= 126) {
                insertChar((char)ch);
            }

            if (ch == 1 || ch == 19) { // Ctrl+A? No — Ctrl+S = 19
                saveFile();
            } else if (ch == 15) { // Ctrl+O
                cout << "\n\x1b[33mOpen file: \x1b[0m";
                string f; getline(cin, f);
                if (!f.empty()) openFile(f);
            } else if (ch == 6) { // Ctrl+F
                cout << "\n\x1b[33mFind: \x1b[0m";
                string s; getline(cin, s);
                if (!s.empty()) {
                    int count = findTextAll(s);
                    if (count > 0) {
                        findText(s);
                        cout << "\x1b[32mFound " << count << " occurrence(s)\x1b[0m";
                    } else {
                        cout << "\x1b[31mNot found\x1b[0m";
                    }
                    Sleep(1000);
                }
            } else if (ch == 7) { // Ctrl+G
                cout << "\n\x1b[33mGo to line: \x1b[0m";
                string s; getline(cin, s);
                goToLine(atoi(s.c_str()));
            } else if (ch == 26) { // Ctrl+Z
                undo();
            } else if (ch == 25) { // Ctrl+Y
                redo();
            } else if (ch == 17) { // Ctrl+Q
                if (modified) {
                    cout << "\n\x1b[33mSave changes? (y/n/c): \x1b[0m";
                    ch = _getch();
                    if (ch == 'y' || ch == 'Y') saveFile();
                    else if (ch == 'c' || ch == 'C') continue;
                }
                running = false;
            } else if (ch == 3) { // Ctrl+C copy — simplified
                clipboard = lines[cursorRow];
            } else if (ch == 22) { // Ctrl+V paste
                pasteClipboard();
            }
        }

        setCursorVisible(true);
        clearScreen();
        cout << "\x1b[32mEditor closed.\x1b[0m\n";
    }
};

int main(int argc, char* argv[]) {
    string initialFile;
    if (argc > 1) initialFile = argv[1];

    TextEditor editor(initialFile);
    editor.run();

    return 0;
}
