/*
 * Author: John Welch
 * Auburn ID: jdw0156
 * Filename: project4_Welch_jdw0156.cpp
 * Compile: g++ project4_Welch_jdw0156.cpp -o project4_Welch_jdw0156
 * Resources: Internet for C++ syntax, linkedlist help, and Dr Li slides
 */

// Only God knows how this code works at this point
// Goodluck to anyone trying to understand it

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <limits>
#include <stdexcept>

// Forward declarations
class QuestionNode;
class MCQOption;

// Class for storing multiple choice options
class MCQOption {
public:
    std::string choice;
    MCQOption* next;
    char label; 
    MCQOption(std::string c, char l) : choice(c), next(nullptr), label(l) {}
};

// Base question node class
class QuestionNode {
public:
    enum QuestionType { MCQ, TF, WR };

    QuestionType type;
    std::string question;
    std::string correct;
    QuestionNode* next;
    QuestionNode* prev; 
    MCQOption* choices; // Only used for MCQ
    double score;
    std::string userAnswer;

    QuestionNode() : next(nullptr), prev(nullptr), choices(nullptr), score(0.0) {}

    // Destructor to clean up MCQ choices
    ~QuestionNode() {
        MCQOption* current = choices;
        while (current != nullptr) {
            MCQOption* temp = current;
            current = current->next;
            delete temp;
        }
    }
};

// Quiz management class
class QuizManager {
public:
    int questionCount;
    double totalscore;

public:
    QuestionNode* head; // Pointer to first node
    QuestionNode* tail; // Pointer to last node

    // Helper to convert string to lowercase for comparison
    std::string toLower(std::string str) {
        for (size_t i = 0; i < str.length(); ++i) {
            str[i] = tolower(str[i]);
        }
        return str;
    }

public:
    QuizManager() : head(nullptr), tail(nullptr), questionCount(0), totalscore(0.0) {}
    ~QuizManager() {
        QuestionNode* current = head;
        while (current != nullptr) {
            QuestionNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    void editQuestion() {
    if (!head) {
        std::cout << "No questions to edit." << std::endl;
        return;
    }

    // Display the questions
    QuestionNode* current = head;
    int index = 1;
    int actualQuestionCount = 0; 
    while (current) {
        current = current->next;
        index++;
        actualQuestionCount++;
    }

    int questionIndex;
    std::cout << "Select a question to edit, or type quit() [1-" << actualQuestionCount << "]: ";
    std::string input;
    std::getline(std::cin, input);

    if (input == "quit()") {
        std::cout << "Edit operation cancelled." << std::endl;
        return;
    }

    try {
        questionIndex = std::stoi(input);
    } 
    catch (const std::invalid_argument&) {
        std::cerr << "Invalid input. Please enter a number or type quit()." << std::endl;
        return;
    }
    while (questionIndex < 1 || questionIndex > actualQuestionCount) {
            std::cout << "Select a question to edit, or type quit() [1-" << actualQuestionCount << "]: ";
            std::getline(std::cin, input);
            if (input == "quit()") {
                std::cout << "Edit operation cancelled." << std::endl;
                return;
            }
            try {
                questionIndex = std::stoi(input);
                break;
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid input. Please enter a number or type quit()." << std::endl;
                std::cout << std::endl;
            }
        }

    current = head;
    for (int i = 1; i < questionIndex; i++) {
        current = current->next;
    }

    // Display the selected question
    std::cout << "===============================\n";
    std::cout << "=== QUESTION " << questionIndex << " SAVED VALUES ===\n";
    std::cout << "===============================\n";
    std::cout << "1. Type: " << (current->type == QuestionNode::MCQ ? "mcq" : (current->type == QuestionNode::TF ? "tf" : "wr")) << "\n";
    std::cout << "2. Question: " << current->question << "\n";
    if (current->type == QuestionNode::MCQ) {
        std::cout << "3. Answer choices:\n";
        MCQOption* choice = current->choices;
        while (choice) {
            std::cout << "\t" << choice->label << ". " << choice->choice << "\n";
            choice = choice->next;
        }
    }
    std::cout << "4. Correct answer: " << current->correct << "\n";
    std::cout << "===============================\n";

    std::cout << "Type a number to edit, or type quit(): ";
    std::getline(std::cin, input);
    if (input == "quit()") {
        std::cout << "Edit operation cancelled." << std::endl;
        return;
    }

    int editChoice;
    try {
        editChoice = std::stoi(input);
    } catch (const std::invalid_argument&) {
        std::cerr << "Invalid input. Please enter a number or type quit()." << std::endl;
        return;
    }

    switch (editChoice) {
        case 2:
            std::cout << "Enter new question text: ";
            std::getline(std::cin, current->question);
            std::cout << "Question updated successfully." << std::endl;
            break;
        case 3:
            if (current->type == QuestionNode::MCQ) {
                std::cout << "Editing answer choices.\n";
                char label = 'A';
                MCQOption* choice = current->choices;
                while (choice) {
                    std::cout << "Current choice " << label << ": " << choice->choice << "\n";
                    std::cout << "Enter choice " << label << " ";
                    std::string newChoiceText;
                    std::getline(std::cin, newChoiceText);
                    choice->choice = newChoiceText;
                    choice = choice->next;
                    label++;
                }
                std::cout << "Choices updated successfully." << std::endl;
            } 
            else {
                std::cerr << "[Editing choices is only for MCQ.]" << std::endl;
            }
            break;
        case 4:
            std::cout << "Enter new correct answer: ";
            std::getline(std::cin, current->correct);
            std::cout << "Correct answer updated successfully." << std::endl;
            break;
        default:
            std::cerr << "Invalid selection for editing." << std::endl;
            break;
    }
}
 

    void deleteQuestion() {
    if (!head) {
        std::cout << "No questions to delete." << std::endl;
        return;
    }

    // Display the list of questions with their index
    QuestionNode* current = head;
    int questionIndex = 1;
    while (current) {
        std::cout << questionIndex << ". " << current->question << std::endl;
        current = current->next;
        questionIndex++;
    }

    // Ask user for the index to delete
    int deleteIndex;
    std::cout << "Select a question to delete [1-" << (questionIndex - 1) << "]: ";
    std::cin >> deleteIndex;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (deleteIndex < 1 || deleteIndex >= questionIndex) {
        std::cerr << "[That question does not exist!]" << std::endl;
        return;
    }

    // Find the question node
    current = head;
    for (int i = 1; i < deleteIndex; i++) {
        current = current->next;
    }

    // Unlink and delete node
    if (current->prev) {
        current->prev->next = current->next;
    } else {
        head = current->next; 
    }

    if (current->next) {
        current->next->prev = current->prev;
    } else {
        tail = current->prev; 
    }

    delete current;
    questionCount--;

    std::cout << "Question " << deleteIndex << " deleted." << std::endl;
}


    void createNewQuestion() {
    std::string questionType;
    std::string questionText;
    std::string correctAnswer;
    double pointValue;
    int questionIndex = questionCount + 1; 
    std::cout << "=== Question " << questionIndex << " ===\n";

    // Get question type
    std::cout << "Type of question [mcq/tf/wr]: ";
    std::cin >> questionType;
    questionType = toLower(questionType);
    std::cout << std::endl;

    if (questionType != "mcq" && questionType != "tf" && questionType != "wr") {
        std::cerr << "[Command not recognized, please try again!]" << std::endl;
        while (questionType != "mcq" && questionType != "tf" && questionType != "wr") 
        {
            std::cout << "Type of question [mcq/tf/wr]: ";
            std::cin >> questionType;
            questionType = toLower(questionType);
            std::cout << std::endl;
        }
        
    }

    // Clear input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Create a new question node
    QuestionNode* newQuestion = new QuestionNode();
    newQuestion->choices = nullptr;

    if (questionType == "mcq") {
        newQuestion->type = QuestionNode::MCQ;

        std::cout << "Enter a question: ";
        std::getline(std::cin, questionText);
        newQuestion->question = questionText;
        std::cout << "[At any time, type ‘quit()’ to exit]" << std::endl;
        std::cout << std::endl;

        // Ask user to enter choices
        char label = 'A';
        while (true) {
            std::string choiceText;
            std::cout << "Enter choice " << label << ": ";
            std::getline(std::cin, choiceText);
            if (toLower(choiceText) == "quit()"){
                std::cout << std::endl;
                break;
            }

            MCQOption* newChoice = new MCQOption(choiceText, label);
            if (!newQuestion->choices) {
                newQuestion->choices = newChoice;
            } else {
                MCQOption* current = newQuestion->choices;
                while (current->next) {
                    current = current->next;
                }
                current->next = newChoice;
            }
            label++;

            // Stop adding choices if we reach the maximum
            if (label > 'Z') {
                std::cerr << "[Maximum number of choices reached]" << std::endl;
                break;
            }
        }

        // Get correct answer
        std::cout << "Select correct answer: ";
        std::cin >> correctAnswer;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer
        newQuestion->correct = toLower(correctAnswer);

    } else if (questionType == "tf") {
        newQuestion->type = QuestionNode::TF;
        std::cout << "Enter a question: ";
        std::getline(std::cin, questionText);
        newQuestion->question = questionText;
        std::cout << std::endl;

        // Get correct answer (T/F)
        std::cout << "Select correct answer [true/false]: ";
        std::cin >> correctAnswer;
        while (toLower(correctAnswer) != "true" && toLower(correctAnswer) != "false") {
            std::cerr << "[Invalid answer. Please enter true or false.]" << std::endl;
            std::cout << "Select correct answer [true/false]: ";
            std::cin >> correctAnswer;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        newQuestion->correct = toLower(correctAnswer);

    } else if (questionType == "wr") {
        newQuestion->type = QuestionNode::WR;
        std::cout << "Enter a question: ";
        std::getline(std::cin, questionText);
        newQuestion->question = questionText;
        std::cout << std::endl;

        // Get correct answer
        std::cout << "Type correct answer: ";
        std::getline(std::cin, correctAnswer);
        newQuestion->correct = toLower(correctAnswer);
    }

    // Get point value
    while (true) {
        std::cout << "Enter point value: ";
        std::cin >> pointValue;
        if (std::cin.fail() || pointValue < 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "[Not a valid point value, please try again!]" << std::endl;
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer
            break;
        }
    }
    newQuestion->score = pointValue;

    // Link the new question to the list
    if (!head) {
        head = tail = newQuestion;
    } else {
        tail->next = newQuestion;
        newQuestion->prev = tail;
        tail = newQuestion;
    }

    questionCount++;
    std::cout << "Question saved.";
    std::cout << std::endl;
    std::cout << std::endl;
}


double assessment() {
    QuestionNode* current = nullptr;
    int questionIndex = 1;

    while (true) {
        if (!current) {
            std::cout << "Do you want to?\n";
            std::cout << "1. Go to next question.\n2. Jump to question.\n3. Submit.\n";
            std::cout << "Select an action: ";
            int action;
            std::cin >> action;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl;

            if (action == 3) {
                std::cout << "/!\\ Assessment Complete.\n";
                // Display session log here
                std::cout << "=== SESSION LOG ===" << std::endl;
                std::cout << "Total questions: " << questionCount << std::endl;
                int correctAnswers = 0;
                double score = 0.0;
                current = head;

                while (current) {
                    if (toLower(current->userAnswer) == current->correct) {
                        score += current->score;
                        correctAnswers++;
                    }
                    current = current->next;
                }

                std::cout << "Total correct answers: " << correctAnswers << std::endl;
                current = head;
                int count = 1;
                while (current) {
                    std::cout << "\tQuestion " << count << ": " << current->question << std::endl;
                    std::cout << "\tYour answer: " << current->userAnswer << std::endl << std::endl;
                    current = current->next;
                    count++;
                }
                std::cout << "Final score: " << score << std::endl << std::endl;
                return score;
                break;
            } else if (action == 2) {
                std::cout << "Jump to question [1-" << questionCount << "]: ";
                std::cin >> questionIndex;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (questionIndex < 1 || questionIndex > questionCount) {
                    std::cerr << "[That question does not exist!]" << std::endl;
                    continue;
                }
                current = head;
                for (int i = 1; i < questionIndex; i++) {
                    current = current->next;
                }
            } else if (action == 1) {
                current = head;
                questionIndex = 1;
                while (current && !current->userAnswer.empty()) {
                    current = current->next;
                    questionIndex++;
                }
                if (!current) {
                    std::cout << "[No more unanswered questions.]" << std::endl;
                    current = nullptr;
                }
            }
        }

        if (current) {
            // Display current question
            std::cout << "Question " << questionIndex << ": " << current->question << "\n";
            if (current->type == QuestionNode::MCQ) {
                char label = 'A';
                MCQOption* choice = current->choices;
                while (choice) {
                    std::cout << label << ". " << choice->choice << "\n";
                    choice = choice->next;
                    label++;
                }
                std::cout << "Your answer: ";
                std::string answer;
                std::getline(std::cin, answer);
                current->userAnswer = answer;
                std::cout << std::endl;
            } else if (current->type == QuestionNode::TF) {
                std::cout << "Your answer [true/false]: ";
                std::string answer;
                std::getline(std::cin, answer);
                current->userAnswer = answer;
                std::cout << std::endl;
            } else if (current->type == QuestionNode::WR) {
                std::cout << "Your answer: ";
                std::string answer;
                std::getline(std::cin, answer);
                current->userAnswer = answer;
                std::cout << std::endl;
            }

            std::cout << "Do you want to?\n";
            std::cout << "1. Go to next question.\n2. Jump to question.\n3. Submit.\n";
            std::cout << "Select an action: ";
            int action;
            std::cin >> action;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl;

            if (action == 1) {
                QuestionNode* temp = current->next;
                bool foundUnanswered = false;

                while (true) {
                    if (!temp) {
                        temp = head; // Loop back to the start
                    }
                    if (temp->userAnswer.empty()) {
                        current = temp;
                        questionIndex = 1;
                        QuestionNode* countNode = head;
                        while (countNode != temp) {
                            questionIndex++;
                            countNode = countNode->next;
                        }
                        foundUnanswered = true;
                        break;
                    }
                    temp = temp->next;
                    if (temp == current) {
                        break; // Full loop completed
                    }
                }

                if (!foundUnanswered) {
                    std::cout << "[No more unanswered questions.]" << std::endl;
                    current = nullptr;
                }
            } else if (action == 2) {
                std::cout << "Jump to question [1-" << questionCount << "]: ";
                std::cin >> questionIndex;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (questionIndex < 1 || questionIndex > questionCount) {
                    std::cerr << "[That question does not exist!]" << std::endl;
                    continue;
                }
                current = head;
                for (int i = 1; i < questionIndex; i++) {
                    current = current->next;
                }
            } else if (action == 3) {
                std::cout << "/!\\ Assessment Complete.\n";
                // Display session log here
                std::cout << "=== SESSION LOG ===" << std::endl;
                int correctAnswers = 0;
                double score = 0.0;
                current = head;
                while (current) {
                    if (toLower(current->userAnswer) == current->correct) {
                        score += current->score;
                        correctAnswers++;
                    }
                    current = current->next;
                }
                std::cout << "Correct answers: " << correctAnswers << "/" << questionCount << std::endl;
                current = head;
                int count = 1;
                while (current) {
                    std::cout << "\tQuestion " << count << ": " << current->question << std::endl;
                    std::cout << "\tYour answer: " << current->userAnswer << std::endl << std::endl;
                    current = current->next;
                    count++;
                }
                std::cout << "Final score: " << score << "/" << totalscore << std::endl << std::endl;
                return score;
                break;
            }
        }
    }
}

};


#define UNIT_TESTING
#ifdef UNIT_TESTING

//Test drivers
void test1() {
    QuizManager quiz;
    std::cout << "Unit Test Case 1: Verify behavior with zero questions.\n";
    assert(quiz.questionCount == 0 && "Test Failed: Expected 0 questions initially.");
    std::cout << "Case 1 Passed: No questions present.\n";
    std::cout << std::endl;
}

void test2() {
    QuizManager quiz;
    std::cout << "Unit Test Case 2: Add one T/F question and check incorrect answer.\n";

    // Create and add a T/F question
    QuestionNode* newQuestion = new QuestionNode();
    newQuestion->type = QuestionNode::TF;
    newQuestion->question = "Is the earth flat?";
    newQuestion->correct = "false";
    newQuestion->score = 10;
    quiz.head = quiz.tail = newQuestion;
    quiz.questionCount = 1;

    // Simulate answering incorrectly
    newQuestion->userAnswer = "true";
    assert(newQuestion->userAnswer != newQuestion->correct && "Test Failed: Expected incorrect answer.");
    std::cout << "Case 2 Passed: Incorrect answer handled correctly.\n";
    std::cout << std::endl;
}

void test3() {
    QuizManager quiz;
    std::cout << "Unit Test Case 3: Add one T/F question and check correct answer.\n";

    // Create and add a T/F question
    QuestionNode* newQuestion = new QuestionNode();
    newQuestion->type = QuestionNode::TF;
    newQuestion->question = "Is water wet?";
    newQuestion->correct = "true";
    newQuestion->score = 10;
    quiz.head = quiz.tail = newQuestion;
    quiz.questionCount = 1;

    // Simulate answering correctly
    newQuestion->userAnswer = "true";
    assert(newQuestion->userAnswer == newQuestion->correct && "Test Failed: Expected correct answer.");
    std::cout << "Case 3 Passed: Correct answer verified.\n";
    std::cout << std::endl;
}

void test4() {
    QuizManager quiz;
    std::cout << "Unit Test Case 4: Add and delete a question.\n";

    // Create and add a question
    QuestionNode* newQuestion = new QuestionNode();
    newQuestion->question = "Sample question?";
    quiz.head = quiz.tail = newQuestion;
    quiz.questionCount = 1;

    // Verify initial state
    assert(quiz.questionCount == 1 && "Test Failed: Expected 1 question after adding.");
    std::cout << "Initial question added successfully.\n";

    // Delete the question
    quiz.deleteQuestion();

    // Verify deletion
    assert(quiz.questionCount == 0 && !quiz.head && "Test Failed: Expected 0 questions after deletion.");
    std::cout << "Case 4 Passed: Question deleted successfully.\n";
    std::cout << std::endl;
}

void test5() {
    QuizManager quiz;
    std::cout << "Unit Test Case 5: Add multiple questions and verify.\n";

    // Add multiple questions
    for (int i = 0; i < 3; ++i) {
        QuestionNode* newQuestion = new QuestionNode();
        newQuestion->question = "Sample question " + std::to_string(i + 1) + "?";
        if (!quiz.head) {
            quiz.head = quiz.tail = newQuestion;
        } else {
            quiz.tail->next = newQuestion;
            newQuestion->prev = quiz.tail;
            quiz.tail = newQuestion;
        }
        quiz.questionCount++;
    }

    // Verify count
    assert(quiz.questionCount == 3 && "Test Failed: Expected 3 questions added.");
    std::cout << "Case 5 Passed: Multiple questions added successfully.\n";
    std::cout << std::endl;
}


int main() {
    std::cout << "***This is a debugging version ***" << std::endl;
    std::cout << std::endl;
    test1();
    test2();
    test3();
    test4();
    test5();
    std::cout << "*** End of Debugging Version ***" << std::endl;
    return 0;
}

#else

int main() {
    // Instantiate QuizManager
    QuizManager quiz;
    std::cout << "*** Welcome to John's Testing Service ***\n" << std::endl;
    bool running = true;
    while (running) {
        std::cout << "Do you want to?" << std::endl;
        std::cout << "\t1. Create new question." << std::endl;
        std::cout << "\t2. Edit question." << std::endl;
        std::cout << "\t3. Delete question." << std::endl;
        std::cout << "\t4. Finish." << std::endl;
        int choice;
        std::cout << "Select an action: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        std::cout << std::endl;

        switch (choice) {
            case 1:
                quiz.createNewQuestion();
                break;
            case 2:
                quiz.editQuestion();
                break;
            case 3:
                quiz.deleteQuestion();
                break;
            case 4:
                running = false;
                break;
            default:
                std::cerr << "[Command not recognized, please try again!]" << std::endl;
                break;
        }
    }

    // Display session log
    std::cout << "=== SESSION LOG ===" << std::endl;
    std::cout << "Total questions: " << quiz.questionCount << std::endl;
    quiz.totalscore = 0.0;
    QuestionNode* current = quiz.head;
    while (current) {
        quiz.totalscore += current->score;
        current = current->next;
    }
    std::cout << "Total point values: " << quiz.totalscore << std::endl;
    std::cout << "\n";

    // Begin assessment y/n
    std::cout << "/!\\ Begin assessment? [y/n]: ";
    char response;
    std::cin >> response;
    while (tolower(response) != 'y' && tolower(response) != 'n') {
        std::cerr << "Invalid response. Please enter y or n." << std::endl;
        std::cout << "/!\\ Begin assessment? [y/n]: ";
        std::cin >> response;
    }
    if (tolower(response) == 'y') {
        quiz.assessment();
    } 
        
    std::cout << "*** Thank you for using the testing service. Goodbye! ***" << std::endl;
    return 0;
}
#endif
