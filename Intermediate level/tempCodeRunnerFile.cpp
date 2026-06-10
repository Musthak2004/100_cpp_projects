// ============================================================================
// Online Quiz System (OOP) — Intermediate level project
// Style template: inventoryManagementSystem.cpp
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <random>
#include <stdexcept>
#include <cctype>

// ============================================================================
// Enums/Types
// ============================================================================
enum class Difficulty
{
    EASY,
    MEDIUM,
    HARD
};

// Helper function to print Difficulty as a string
std::string difficultyToString(Difficulty d)
{
    switch (d)
    {
    case Difficulty::EASY:
        return "EASY";
    case Difficulty::MEDIUM:
        return "MEDIUM";
    case Difficulty::HARD:
        return "HARD";
    default:
        return "UNKNOWN";
    }
}

// ============================================================================
// Domain classes: Question & Player
// ============================================================================

/**
 * @brief Question class representing a single multiple-choice question.
 * Encapsulates the question text, four options, the correct option index,
 * the category it belongs to, and the marks awarded for a correct answer.
 */
class Question
{
private:
    std::string questionText;
    std::vector<std::string> options; // Always 4 options (A, B, C, D)
    int correctIndex;                 // 0..3
    std::string category;
    int marks;
    Difficulty difficulty;

public:
    // Default Constructor
    Question()
        : questionText(""), correctIndex(0), category(""), marks(0), difficulty(Difficulty::EASY) {}

    // Parametric Constructor
    Question(std::string text, std::vector<std::string> opts, int correctIdx,
             std::string cat, int m, Difficulty d)
        : questionText(text), options(std::move(opts)), correctIndex(correctIdx),
          category(cat), marks(m), difficulty(d) {}

    // Getters
    std::string getText() const { return questionText; }
    std::vector<std::string> getOptions() const { return options; }
    int getCorrectIndex() const { return correctIndex; }
    std::string getCategory() const { return category; }
    int getMarks() const { return marks; }
    Difficulty getDifficulty() const { return difficulty; }

    /**
     * @brief Display the question stem and its 4 options, prefixed with
     *        A./B./C./D. and the 1-based question number.
     */
    void displayQuestion(int number) const
    {
        std::cout << "\n--------------------------------\n";
        std::cout << "Q" << number << " [" << category << " - "
                  << difficultyToString(difficulty) << "] "
                  << "(" << marks << " mark" << (marks == 1 ? "" : "s") << ")\n";
        std::cout << questionText << "\n\n";

        const char letters[4] = {'A', 'B', 'C', 'D'};
        for (size_t i = 0; i < options.size() && i < 4; ++i)
        {
            std::cout << "  " << letters[i] << ". " << options[i] << "\n";
        }
        std::cout << "--------------------------------\n";
    }

    /**
     * @brief Check whether the chosen option index is the correct one.
     * @param chosenIndex Zero-based option index (0..3).
     * @return true if chosenIndex equals the stored correct index.
     */
    bool isCorrect(int chosenIndex) const
    {
        return chosenIndex == correctIndex;
    }

    /**
     * @brief Return the correct option as a single letter ('A'..'D').
     */
    char getCorrectLetter() const
    {
        const char letters[4] = {'A', 'B', 'C', 'D'};
        if (correctIndex >= 0 && correctIndex < 4)
        {
            return letters[correctIndex];
        }
        return '?';
    }
};

/**
 * @brief Player class representing a registered quiz-taker.
 * Tracks the player's name and only the most recent attempt's score
 * (per project requirements: "last score only").
 */
class Player
{
private:
    std::string playerName;
    int lastScore;
    int lastTotal;
    bool hasTakenQuiz;
    std::string lastCategory;

public:
    // Default Constructor
    Player()
        : playerName(""), lastScore(0), lastTotal(0),
          hasTakenQuiz(false), lastCategory("") {}

    // Parametric Constructor
    explicit Player(std::string name)
        : playerName(std::move(name)), lastScore(0), lastTotal(0),
          hasTakenQuiz(false), lastCategory("") {}

    // Getters
    std::string getName() const { return playerName; }
    int getLastScore() const { return lastScore; }
    int getLastTotal() const { return lastTotal; }
    bool getHasTakenQuiz() const { return hasTakenQuiz; }
    std::string getLastCategory() const { return lastCategory; }

    /**
     * @brief Record the player's most recent quiz attempt. Overwrites any
     *        previous attempt, matching the "last score only" requirement.
     */
    void recordScore(int score, int total, const std::string &category)
    {
        lastScore = score;
        lastTotal = total;
        lastCategory = category;
        hasTakenQuiz = true;
    }

    /**
     * @brief Percentage score in [0, 100]. Returns 0 if total is 0.
     */
    double getLastPercentage() const
    {
        if (lastTotal <= 0)
        {
            return 0.0;
        }
        return (static_cast<double>(lastScore) / static_cast<double>(lastTotal)) * 100.0;
    }
};

// ============================================================================
// QuizManager — owns the question bank and the registered players
// ============================================================================

/**
 * @brief QuizManager class — central data layer for the quiz system.
 * Owns the question bank (std::vector<Question>) and the registered
 * players (std::map<std::string, Player> keyed by name).
 */
class QuizManager
{
private:
    std::vector<Question> questionBank;
    std::map<std::string, Player> players;

    // Random engine for shuffling questions
    std::mt19937 rng;

    /**
     * @brief Populate the question bank with hardcoded sample questions.
     * Called from the constructor; the project does not allow runtime
     * question creation, per requirements.
     */
    void loadSampleQuestions()
    {
        // --- C++ Basics ---
        questionBank.emplace_back(
            "Which header is required to use std::cout and std::cin in C++?",
            std::vector<std::string>{"<stdio.h>", "<iostream>", "<stdlib.h>", "<string>"},
            1, "C++ Basics", 1, Difficulty::EASY);

        questionBank.emplace_back(
            "Which keyword declares a class-scoped strongly-typed enumeration in C++?",
            std::vector<std::string>{"enum", "enum struct", "enum class", "class enum"},
            2, "C++ Basics", 1, Difficulty::MEDIUM);

        questionBank.emplace_back(
            "What does the 'explicit' keyword on a single-argument constructor prevent?",
            std::vector<std::string>{
                "Copy construction",
                "Implicit conversion from the argument type",
                "Move construction",
                "Virtual dispatch"},
            1, "C++ Basics", 1, Difficulty::MEDIUM);

        questionBank.emplace_back(
            "Which container stores key-value pairs with O(log n) lookup and sorted keys by default?",
            std::vector<std::string>{
                "std::unordered_map", "std::vector", "std::map", "std::list"},
            2, "C++ Basics", 1, Difficulty::EASY);

        questionBank.emplace_back(
            "Inside a const member function, which of the following is NOT allowed?",
            std::vector<std::string>{
                "Reading a member variable",
                "Calling another const member function",
                "Modifying a member variable",
                "Returning by value"},
            2, "C++ Basics", 1, Difficulty::MEDIUM);

        // --- General Knowledge ---
        questionBank.emplace_back(
            "What is the capital of Japan?",
            std::vector<std::string>{"Seoul", "Tokyo", "Beijing", "Bangkok"},
            1, "General Knowledge", 1, Difficulty::EASY);

        questionBank.emplace_back(
            "Which planet is known as the Red Planet?",
            std::vector<std::string>{"Earth", "Jupiter", "Mars", "Venus"},
            2, "General Knowledge", 1, Difficulty::EASY);

        questionBank.emplace_back(
            "How many continents are there on Earth?",
            std::vector<std::string>{"5", "6", "7", "8"},
            2, "General Knowledge", 1, Difficulty::EASY);

        questionBank.emplace_back(
            "Which language is primarily used to style web pages?",
            std::vector<std::string>{"HTML", "Python", "JavaScript", "CSS"},
            3, "General Knowledge", 1, Difficulty::EASY);

        // --- Math ---
        questionBank.emplace_back(
            "What is 7 x 8?",
            std::vector<std::string>{"54", "56", "58", "64"},
            1, "Math", 1, Difficulty::EASY);

        questionBank.emplace_back(
            "What is the value of pi rounded to two decimal places?",
            std::vector<std::string>{"3.12", "3.14", "3.16", "3.18"},
            1, "Math", 1, Difficulty::EASY);

        questionBank.emplace_back(
            "What is the square root of 144?",
            std::vector<std::string>{"10", "11", "12", "14"},
            2, "Math", 1, Difficulty::EASY);
    }

public:
    QuizManager() : rng(std::random_device{}())
    {
        loadSampleQuestions();
    }

    /**
     * @brief Register a new player by name. Returns false if the name
     *        already exists (case-sensitive exact match).
     */
    bool registerPlayer(const std::string &name)
    {
        if (name.empty())
        {
            return false;
        }
        if (players.find(name) != players.end())
        {
            return false; // already registered
        }
        players.emplace(name, Player(name));
        return true;
    }

    /**
     * @brief Look up a player by exact name. Returns nullptr if not found.
     */
    Player *findPlayer(const std::string &name)
    {
        auto it = players.find(name);
        if (it == players.end())
        {
            return nullptr;
        }
        return &(it->second);
    }

    /**
     * @brief Return the unique list of categories present in the question bank.
     *        Order follows first appearance in the bank.
     */
    std::vector<std::string> getCategories() const
    {
        std::vector<std::string> categories;
        for (const auto &q : questionBank)
        {
            const std::string &cat = q.getCategory();
            if (std::find(categories.begin(), categories.end(), cat) == categories.end())
            {
                categories.push_back(cat);
            }
        }
        return categories;
    }

    /**
     * @brief Return all questions that belong to the given category.
     *        If no questions match, the returned vector is empty.
     */
    std::vector<Question> getQuestionsForCategory(const std::string &category) const
    {
        std::vector<Question> result;
        for (const auto &q : questionBank)
        {
            if (q.getCategory() == category)
            {
                result.push_back(q);
            }
        }
        return result;
    }

    /**
     * @brief Run a quiz for the named player in the given category.
     *        Throws std::runtime_error if the player is unknown or the
     *        category has no questions. Returns the score earned and
     *        writes the total possible marks into outTotal.
     */
    int takeQuiz(const std::string &playerName, const std::string &category, int &outTotal)
    {
        Player *player = findPlayer(playerName);
        if (player == nullptr)
        {
            throw std::runtime_error("Player not found: " + playerName);
        }

        std::vector<Question> qs = getQuestionsForCategory(category);
        if (qs.empty())
        {
            throw std::runtime_error("No questions available in category: " + category);
        }

        // Shuffle the working copy for replay value
        std::shuffle(qs.begin(), qs.end(), rng);

        int score = 0;
        int totalMarks = 0;
        for (const auto &q : qs)
        {
            totalMarks += q.getMarks();
        }

        std::cout << "\n========================================\n";
        std::cout << "       QUIZ: " << category << "\n";
        std::cout << "       Player: " << player->getName() << "\n";
        std::cout << "========================================\n";

        const int MAX_INVALID_RETRIES = 3;

        for (size_t i = 0; i < qs.size(); ++i)
        {
            qs[i].displayQuestion(static_cast<int>(i + 1));

            int chosenIndex = -1;
            int invalidAttempts = 0;
            while (true)
            {
                std::cout << "Your answer (A/B/C/D): ";
                std::string ans;
                std::cin >> ans;

                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Please answer A, B, C, or D.\n";
                    if (++invalidAttempts >= MAX_INVALID_RETRIES)
                    {
                        std::cout << "Maximum invalid attempts reached. Marking as wrong.\n";
                        break;
                    }
                    continue;
                }

                if (ans.empty())
                {
                    if (++invalidAttempts >= MAX_INVALID_RETRIES)
                    {
                        std::cout << "Maximum invalid attempts reached. Marking as wrong.\n";
                        break;
                    }
                    std::cout << "Please answer A, B, C, or D.\n";
                    continue;
                }

                char letter = static_cast<char>(std::toupper(static_cast<unsigned char>(ans[0])));
                if (letter == 'A')
                {
                    chosenIndex = 0;
                }
                else if (letter == 'B')
                {
                    chosenIndex = 1;
                }
                else if (letter == 'C')
                {
                    chosenIndex = 2;
                }
                else if (letter == 'D')
                {
                    chosenIndex = 3;
                }
                else
                {
                    if (++invalidAttempts >= MAX_INVALID_RETRIES)
                    {
                        std::cout << "Maximum invalid attempts reached. Marking as wrong.\n";
                        break;
                    }
                    std::cout << "Please answer A, B, C, or D.\n";
                    continue;
                }
                break; // valid letter
            }

            if (chosenIndex == -1)
            {
                // Treat as wrong; show correct answer and continue.
                std::cout << "Wrong! Correct answer was: "
                          << qs[i].getCorrectLetter() << "\n";
                continue;
            }

            if (qs[i].isCorrect(chosenIndex))
            {
                std::cout << "Correct!\n";
                score += qs[i].getMarks();
            }
            else
            {
                std::cout << "Wrong! Correct answer was: "
                          << qs[i].getCorrectLetter() << "\n";
            }
        }

        // Persist the result on the player
        player->recordScore(score, totalMarks, category);

        outTotal = totalMarks;
        return score;
    }

    /**
     * @brief Print a leaderboard table of all registered players,
     *        sorted by their most recent percentage (descending).
     *        Players who have not taken a quiz are shown with placeholders.
     */
    void printLeaderboard() const
    {
        if (players.empty())
        {
            std::cout << "No players registered yet.\n";
            return;
        }

        std::vector<Player> rows;
        rows.reserve(players.size());
        for (const auto &pair : players)
        {
            rows.push_back(pair.second);
        }

        std::sort(rows.begin(), rows.end(),
                  [](const Player &a, const Player &b)
                  {
                      return a.getLastPercentage() > b.getLastPercentage();
                  });

        std::cout << "\n========================================\n";
        std::cout << "              LEADERBOARD\n";
        std::cout << "========================================\n";

        std::cout << std::left
                  << std::setw(6) << "Rank"
                  << std::setw(20) << "Player"
                  << std::setw(20) << "Category"
                  << std::setw(10) << "Score"
                  << std::setw(10) << "Percent"
                  << "\n";
        std::cout << std::string(66, '-') << "\n";

        int rank = 1;
        for (const auto &p : rows)
        {
            std::cout << std::left << std::setw(6) << rank
                      << std::setw(20) << p.getName();
            if (p.getHasTakenQuiz())
            {
                std::cout << std::setw(20) << p.getLastCategory()
                          << std::setw(10)
                          << (std::to_string(p.getLastScore()) + "/" +
                              std::to_string(p.getLastTotal()))
                          << std::fixed << std::setprecision(2)
                          << std::setw(10) << p.getLastPercentage();
            }
            else
            {
                std::cout << std::setw(20) << "-"
                          << std::setw(10) << "-"
                          << std::setw(10) << "-";
            }
            std::cout << "\n";
            ++rank;
        }
        std::cout << "\n";
    }

    /**
     * @brief Dump the entire question bank with options and correct
     *        answers. Used by the "View all questions" menu option.
     */
    void printAllQuestions() const
    {
        std::cout << "\n========================================\n";
        std::cout << "         ALL QUESTIONS (DEBUG)\n";
        std::cout << "========================================\n";

        for (size_t i = 0; i < questionBank.size(); ++i)
        {
            questionBank[i].displayQuestion(static_cast<int>(i + 1));
            std::cout << "  >> Correct answer: "
                      << questionBank[i].getCorrectLetter() << "\n";
        }
        std::cout << "\n";
    }
};

// ============================================================================
// Main Menu Class
// ============================================================================

/**
 * @brief Menu class — console driver for the quiz system. Wraps a
 *        QuizManager pointer and exposes a run() method that loops
 *        over the 6 user-facing options.
 */
class Menu
{
private:
    QuizManager *mgr;

    /**
     * @brief Print the main menu banner and options.
     */
    void showBanner() const
    {
        std::cout << "\n========================================\n";
        std::cout << "           ONLINE QUIZ SYSTEM\n";
        std::cout << "========================================\n\n";
        std::cout << "1. Register player\n";
        std::cout << "2. Take a quiz\n";
        std::cout << "3. View my last score\n";
        std::cout << "4. View leaderboard\n";
        std::cout << "5. View all questions\n";
        std::cout << "6. Exit\n";
        std::cout << "--------------------------------\n";
        std::cout << "Enter your choice (1-6): ";
    }

    /**
     * @brief Trim leading and trailing whitespace from a string.
     */
    static std::string trim(const std::string &s)
    {
        const auto start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos)
        {
            return "";
        }
        const auto end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    /**
     * @brief Prompt the user for a non-empty, trimmed name. Returns
     *        empty string if input failed irrecoverably.
     */
    std::string promptPlayerName(const std::string &purpose) const
    {
        std::string name;
        std::cout << "Enter your name to " << purpose << ": ";
        std::getline(std::cin >> std::ws, name);
        name = trim(name);
        return name;
    }

    /**
     * @brief Read a single integer with full input recovery.
     *        Returns true on success and writes into out; returns
     *        false if input was non-numeric (after clearing state).
     */
    static bool readInt(int &out)
    {
        if (!(std::cin >> out))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return false;
        }
        // Consume the trailing newline so subsequent getline works.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return true;
    }

    /**
     * @brief Option 1 — register a new player.
     */
    void optionRegister()
    {
        std::string name = promptPlayerName("register");
        if (name.empty())
        {
            std::cout << "Name cannot be empty.\n";
            return;
        }
        if (mgr->registerPlayer(name))
        {
            std::cout << "Player '" << name << "' registered.\n";
        }
        else
        {
            std::cout << "Player '" << name << "' is already registered.\n";
        }
    }

    /**
     * @brief Option 2 — take a quiz in a chosen category.
     */
    void optionTakeQuiz()
    {
        std::string name = promptPlayerName("take the quiz");
        if (name.empty())
        {
            std::cout << "Name cannot be empty.\n";
            return;
        }
        if (mgr->findPlayer(name) == nullptr)
        {
            std::cout << "Player not found. Please register first.\n";
            return;
        }

        std::vector<std::string> categories = mgr->getCategories();
        if (categories.empty())
        {
            std::cout << "No categories available.\n";
            return;
        }

        std::cout << "\n--- Available categories ---\n";
        for (size_t i = 0; i < categories.size(); ++i)
        {
            std::cout << "  " << (i + 1) << ". " << categories[i] << "\n";
        }
        std::cout << "---------------------------\n";

        int catIdx = 0;
        std::cout << "Pick a category (1-" << categories.size() << "): ";
        if (!readInt(catIdx))
        {
            std::cout << "Invalid input.\n";
            return;
        }
        if (catIdx < 1 || catIdx > static_cast<int>(categories.size()))
        {
            std::cout << "Invalid category selection.\n";
            return;
        }

        std::string category = categories[static_cast<size_t>(catIdx - 1)];

        int total = 0;
        int score = mgr->takeQuiz(name, category, total);

        std::cout << "\n===== RESULT =====\n";
        std::cout << "Your score: " << score << "/" << total << "\n";
        double percentage = (total > 0)
                                ? (static_cast<double>(score) / static_cast<double>(total)) * 100.0
                                : 0.0;
        std::cout << "Percentage: " << std::fixed << std::setprecision(2)
                  << percentage << "%\n";
        std::cout << "===================\n";
    }

    /**
     * @brief Option 3 — view a player's most recent attempt.
     */
    void optionViewLastScore()
    {
        std::string name = promptPlayerName("view");
        if (name.empty())
        {
            std::cout << "Name cannot be empty.\n";
            return;
        }
        Player *p = mgr->findPlayer(name);
        if (p == nullptr)
        {
            std::cout << "Player not found.\n";
            return;
        }
        if (!p->getHasTakenQuiz())
        {
            std::cout << "Player '" << name << "' has not taken a quiz yet.\n";
            return;
        }

        std::cout << "\n--- Last Quiz Result ---\n";
        std::cout << "Player  : " << p->getName() << "\n";
        std::cout << "Category: " << p->getLastCategory() << "\n";
        std::cout << "Score   : " << p->getLastScore() << "/" << p->getLastTotal() << "\n";
        std::cout << "Percent : " << std::fixed << std::setprecision(2)
                  << p->getLastPercentage() << "%\n";
        std::cout << "------------------------\n";
    }

    /**
     * @brief Option 4 — view the leaderboard.
     */
    void optionLeaderboard() const
    {
        mgr->printLeaderboard();
    }

    /**
     * @brief Option 5 — view all questions (debug / study mode).
     */
    void optionViewAllQuestions() const
    {
        mgr->printAllQuestions();
    }

public:
    explicit Menu(QuizManager *manager) : mgr(manager) {}

    /**
     * @brief Main menu loop. Reads `option` as a std::string and
     *        dispatches via if/else if, mirroring the pattern in
     *        InventoryManagerMenu::run() (see inventoryManagementSystem.cpp).
     */
    void run()
    {
        std::string option;

        while (true)
        {
            showBanner();
            std::cin >> option;

            if (option == "1")
            {
                // Consume the trailing newline so promptPlayerName's
                // getline(cin >> ws, ...) reads a clean line.
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                optionRegister();
            }
            else if (option == "2")
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                optionTakeQuiz();
            }
            else if (option == "3")
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                optionViewLastScore();
            }
            else if (option == "4")
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                optionLeaderboard();
            }
            else if (option == "5")
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                optionViewAllQuestions();
            }
            else if (option == "6")
            {
                std::cout << "Goodbye!\n";
                break;
            }
            else
            {
                // Consume the rest of the line in case it was something
                // like "abc" — keeps subsequent reads sane.
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid choice. Try again.\n";
            }
        }
    }
};

// ============================================================================
// Entry point
// ============================================================================
int main()
{
    QuizManager *manager = nullptr;

    try
    {
        // Create the Quiz Manager — loads sample questions in its ctor.
        manager = new QuizManager();

        // Create the Menu and pass it a pointer to the data layer.
        Menu menu(manager);
        menu.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    if (manager != nullptr)
    {
        delete manager;
    }
    return 0;
}
