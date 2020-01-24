#include "game.h"
#include <limits>
#include <string>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <regex>
#include <sstream>
#include <vector>
/*
 * ADVANTAGES/DISADVANTAGES: Linked List:
 * Advantage: You don't need to reallocate the list every time you want to add
 * a command. You simply create a new node and change where the head points.
 * Disadvantage: You must go through the entire list to find a specific index.
 * To get a random node, you must go through the entire list until that number of
 * nodes have been passed. With an array, you can select a specific index.[O(1)]
 *
 * ADVANTAGES/DISADVANTAGES: Array
 * Advantage: Allowed me to directly access profiles through an index.
 * Disadvanage: Using an array for something that is going to be added
 * dynamically is a bad idea. And then to prepend to the zero index? 
 * That causes you to have to copy the entire array one item at a time
 * to a new array. It is incredily time ineffcient, especially when you
 * get to a high n. If it were up to me, I definitely would not have used
 * an array here. 
 */

/*
 * Public functions
 * ----------------------------------------------------------------------------
 */

Game::Game()
{
        srand(time(NULL));
        profiles = nullptr;
        list = nullptr;
        current_arr_size = 0;
}

Game::~Game()
{

}

/*
 * Initializes the list and profile array and
 * load the csv files.
 */
void Game::init()
{
        list = new List<Command, Description>();
        loadFile(Commands);
        loadFile(Profiles);
}

void Game::run()
{
        std::cout << "Welcome to my game." << std::endl;
        bool loaded = false;
        bool is_running = true;
        int answer = 0;
        while (is_running) {
                showMenu();
                std::cin >> answer;
                switch (answer) {
                case Rules:
                        showRules();
                        break;
                case Play:
                        playGame();
                        break;
                case Load:
                        loaded = loadPreviousGame();
                        if(loaded == false)
                            break;
                        playGame(loaded);
                        break;
                case Add:
                        getNewCommand(Add);
                        break;
                case Remove:
                        getNewCommand(Remove);
                        break;
                case Exit:
                        exit();
                        is_running = false;
                        break;
                default:
                        std::cin.clear();
                        std::cin.ignore();
                        break;
                }
        }
}

/*
 * Saves both data sets to files and cleans up the dynamic 
 * memory.
 */
void Game::exit()
{
        saveFile(Commands);
        saveFile(Profiles);
        cleanup();
}

/*
 * Private functions
 * ----------------------------------------------------------------------------
 */

void Game::cleanup()
{
        delete list;
        delete[] profiles;
}

/*
 * Depending on the CSV type, either loads the profile
 * or commands csv files. The commands are a linked list
 * and the profiles are in an array.
 */
void Game::loadFile(CSV type)
{
        std::string data1, data2, line, path = "";
        profiles = new Profile[current_arr_size];
        if (type == Commands) {
                path = command_path;
                std::fstream file(path);
                while (std::getline(file, line)) {
                        parseLine(line, data1, data2);
                        list->prepend(data1, data2);
                }
                file.close();
        } else if (type == Profiles) {
                path = profile_path;
                std::fstream file(path);
                while (std::getline(file, line)) {
                        parseLine(line, data1, data2);
                        insertAtFront(*new Profile(data1, std::stoi(data2)));
                }
                file.close();
        } else {
                std::cerr << "Error retrieving files." << std::endl;
                return;
        }
}
/*
 * Puts the specified profile at the front of the profiles array.
 * This is an incredily ineffcient function, but it was required
 * to fulfill project requirements.
 */
void Game::insertAtFront(Profile &profile)
{
        if (current_arr_size > 0) {
                Profile *tmp = new Profile[current_arr_size + 1];
                for (size_t i = 0; i < current_arr_size; i++) {
                        tmp[i + 1] = profiles[i];
                }
                delete[] profiles;
                profiles = tmp;
                current_arr_size++;
                profiles[0] = profile;
        } else {
                profiles = new Profile[1];
                profiles[0] = profile;
                current_arr_size++;
        }
}

/*
 * Saves the list or profile array to file. For the commands 
 * see the list function toString().
 */
void Game::saveFile(CSV type)
{
        std::string path, output;
        if (type == Commands) {
                path = command_path;
                std::fstream file(path,
                                  std::fstream::trunc | std::fstream::out);
                output = list->toString();
                file << output;
                file.close();
        } else if (type == Profiles) {
                path = profile_path;
                std::fstream file(path,
                                  std::fstream::trunc | std::fstream::out);
                for (size_t i = 0; i < current_arr_size; i++) {
                        file << profiles[i].getName() << ","
                             << profiles[i].getScore() << std::endl;
                }
                file.close();

        } else {
                std::cerr << "Error saving file. Could not select path"
                          << std::endl;
                return;
        }
}

/*
 * Parses out the quotes and splits the line.
 */
void Game::parseLine(std::string line, std::string &data1, std::string &data2)
{
        std::regex r("[\"]+");
        line = std::regex_replace(line, r, "");
        std::stringstream ss(line);
        std::getline(ss, data1, ',');
        std::getline(ss, data2, ',');
}

void Game::showRules()
{
        char input = '0';
        std::cout << "Rules: " << std::endl;
        std::cout
                << "A Linux command will be displayed, and the goal is to"
                << std::endl
                << "guess which of three possible choices is the correct"
                << std::endl
                << "description of that command. If you guess correctly,"
                << std::endl
                << "you will get one point. If you guess incorrectly, you will"
                << std::endl
                << "lose a point." << std::endl;
        while (input != 'y') {
                std::cout << "Enter y to continue" << std::endl;
                std::cin >> input;
        }
}


void Game::showMenu()
{
        std::cout << "Main Menu: " << std::endl;
        std::cout << "1. Game Rules" << std::endl;
        std::cout << "2. Play Game" << std::endl;
        std::cout << "3. Load Previous Game" << std::endl;
        std::cout << "4. Add Command" << std::endl;
        std::cout << "5. Remove Command" << std::endl;
        std::cout << "6. Save and Exit" << std::endl;
        std::cout << "Enter your choice: ";
}

/*
 * Get a user(or uses one already loaded) and number of questions.
 * It then loops and asks however many questions.
 * See also: createUser(), getNumQuestions(), askQuestion()
 */
void Game::playGame(bool loaded)
{
        int num_questions = 0;
        bool valid = loaded;
        char input = '0';
        while (!valid) {
                current = createUser();
                valid = current != nullptr;
                if(!valid) {
                        std::cout << "Return to main menu? y/n: ";
                        std::cin >> input;
                        if(input == 'y')
                                return;
                }
        }
        std::cout << "Playing as " << current->getName() << "." << std::endl;
        num_questions = getNumQuestions();
        if (num_questions == -1) {
                std::cerr << "Error  parsing number of questions" << std::endl;
                return;
        }
        for (int i = 0; i < num_questions; i++) {    
                current->score += askQuestion();
                std::cout << "Continue? y/n:" << std::endl;
                while(true)
                {
                        std::cin >> input;
                        if(input == 'y')
                                break;
                        else if(input == 'n')
                                return;
                }
        }
        current = nullptr;
}

/*
 * Asks a user the question it recieves from the getQuestion()
 * function. Return 1 or -1 depending on if they get the correct answer.
 */
int Game::askQuestion()
{
        Question q = getQuestion();
        std::cout << "Player: " << current->getName() << "\t"
                << "Score: " << current->getScore() << std::endl;
        std::cout << q.cmd << std::endl;
        bool valid = false;
        int answer = 0;
        for (size_t i = 0; i < 3; i++) {
                std::cout << i + 1 << ". " << q.descriptions[i] << std::endl;
        }
        while (!valid) {
                std::cout << "Enter your answer: ";
                std::cin >> answer;
                if (answer >= 1 && answer <= 3) {
                        if (q.descriptions[answer - 1] == q.answer) {
                                std::cout << "You guessed correctly!"
                                        << std::endl;
                                std::cout << "You gain one point!" << std::endl;
                                return 1;
                        } else {
                                std::cout << "You answered wrong!" << std::endl;
                                std::cout << "You lose a point!" << std::endl;
                                std::cout
                                        << "The correct answer is:\n" << q.answer
                                        << std::endl;
                                return -1;
                        }
                } else {
                        std::cout << "Invalid answer" << std::endl;
                        std::cin.clear();
                        std::cin.ignore();
                }
        }
        return 0;
}

int Game::getNumQuestions()
{
        bool valid = false;
        int answer = 0;
        while (!valid) {
                std::cout
                        << "How many questions would you like to have? (5-30):";
                std::cin >> answer;
                if (answer >= 5 && answer <= 30)
                        return answer;
                else {
                        std::cerr << "Invalid input" << std::endl;
                        std::cin.clear();
                        std::cin.ignore();
                }
        }
        return -1;
}

/*
 * Asks the user a random question and selects two other
 * random descriptions. Uses a static vector to keep
 * track of used commands. Can visually lag trying to find an unused
 * command. 
 */
Game::Question Game::getQuestion()
{
        static std::vector<int> usedCommands;
        if (list_edited) {
                usedCommands.clear();
                usedCommands.resize(1);
                list_edited = false;
        }
        if (usedCommands.size() == list->getSize())
        {
                usedCommands.clear();
                usedCommands.resize(1);
        }
        bool valid = false;
        int rnd = 0;
        Question q;
        bool found = false;
        while (!valid) {
                srand(time(NULL));
                found = false;
                rnd = rand() % list->getSize();
                if (usedCommands.size() > 0) {
                        for (const int a : usedCommands) {
                                if(a == rnd){
                                        found = true;
                                        break;
                                }
                        }
                }
                if (found)
                        continue;
                else {
                        Node<Command, Description> *ptr = list->at(rnd);
                        usedCommands.push_back(rnd);
                        valid = true;
                        createQuestion(ptr, q);
                }
        }

        return q; /* The only way this will return is if valid == true */
}

/*
 * Receives a node and creates a question with
 * two other random descriptions. Returns a Question
 * struct which is defined in game.h.
 */
void Game::createQuestion(Node<Command, Description> *node, Question &q)
{
        q.cmd = node->data1;
        q.descriptions[0] = node->data2;
        q.answer = q.descriptions[0];
        int rnd = rand() % list->getSize();
        for (size_t i = 2; i < 4; i++) {
                while (node == list->at(rnd)
                                || q.descriptions[i - 2] == list->at(rnd)->data2) {
                        rnd = rand() % list->getSize();
                }
                q.descriptions[i - 1] = list->at(rnd)->data2;
        }
        rnd = rand() % 3;
        Description tmp = q.descriptions[0];
        q.descriptions[0] = q.descriptions[rnd];
        q.descriptions[rnd] = tmp;
}

/*
 * If the requested username is not taken, creates a user on the 
 * profiles array.
 */
Profile *Game::createUser()
{
        std::string name;
        char inp;
        bool correct = false;
        while (!correct) {
                std::cout << "Enter a profile name(They are case-sensitive): ";
                std::cin >> name;
                std::cout << "Is " << name << " correct? y/n: ";
                std::cin >> inp;
                if (inp == 'y') {
                        if (nameTaken(name)) {
                                std::cout << "That profile name is taken"
                                        << std::endl;
                                std::cout << "Do you want to try again? y/n: ";
                                std::cin >> inp;
                                if(inp == 'n')
                                        return nullptr;

                        } else
                                correct = true;
                }
        }
        Profile *p = new Profile(name);
        insertAtFront(*p);
        return p;
}

/*
 * Locates a user in the profiles array. Will continue to prompy until
 * 1. A profile is made or 
 * 2. The user quits
 */
Profile *Game::findUser()
{
        std::string name;
        char inp;
        bool correct = false;
        while (!correct) {
                std::cout << "Enter a profile name(case-sensitive): ";
                std::cin >> name;
                std::cout << "Is " << name << " correct? y/n: ";
                std::cin >> inp;
                if (inp == 'y') {
                        for (size_t i = 0; i < current_arr_size; i++) {
                                if (name == profiles[i].getName())
                                        return &profiles[i];
                        }
                        std::cout << "Profile not found. try again? y/n: ";
                        std::cin >> inp;
                        correct = inp != 'y';
                }
        }
        return nullptr;
}

bool Game::nameTaken(std::string name)
{
        for (size_t i = 0; i < current_arr_size; i++) {
                if (name == profiles[i].getName())
                        return true;
        }
        return false;
}

/*
 * Prints available profiles. If user enters a valid profile, loads it
 * to current. Otherwise, it will continue to prompt until the user gives
 * up.
 */
bool Game::loadPreviousGame()
{
        bool valid = false;
        std::cout << "Available profiles: " <<std::endl;
        for(size_t i = 0; i < current_arr_size; i++) {
                std::cout << profiles[i].getName() << "\t" << profiles[i].getScore()
                        <<std::endl;
        }
        while (!valid) {
                Profile *p = findUser();
                if (p == nullptr) {
                        return false;
                } else {
                        current = p;
                        return true;
                }
        }
        return false;
}

/*
 * Requests the user to enter a command. Once it is entered
 * it gives the command to either add or remove command.
 * Will continue to prompt until a valid command is entered
 * or the user gives up.
 */
void Game::getNewCommand(MenuOptions option)
{

        std::string cmd, desc;
        char input = '0';
        bool valid = false;
        while(!valid) {
top:
                std::cout << "Enter the name of the command: ";
                std::cin.ignore(128, '\n');
                std::getline(std::cin, cmd);
                if(option != Remove) {
                        std::cout << "Enter the description: ";
                        std::getline(std::cin, desc);
                }
                if(cmd.empty())
                        continue;
                std::cout << "Is " << cmd << " correct? y/n: "; 
                std::cin >> input;
                valid = input == 'y';
        }
        if(option == Add)
                valid = addCommand(cmd, desc);
        else if(option == Remove)
                valid = removeCommand(cmd);
        else return;
        if(valid)
                std::cout << "Operation successful!" << std::endl;
        else {
                std::cout << "I am disinclined to acquiesce to your" 
                        << " request.\nMeans no."
                        << std::endl;
                std::cout << "Would you like to try again? y/n: ";
                std::cin.ignore(128, '\n');
                std::cin >> input;
                if(input == 'y')
                        goto top;
        } 
}

/*
 * Checks the commands list for the given cmd, and if it detects 
 * a duplicate returns false. Otherwise, it prepends the new command
 * to the list.
 */
bool Game::addCommand(const std::string cmd, const std::string desc)
{
        Node<Command, Description> *ptr = list->find(cmd);
        if(ptr == nullptr || ptr->data1 == cmd){
                list_edited = true;
                list->prepend(cmd, desc);
                return true;
        }
        else std::cout << "That command already exists!" << std::endl;
        return false;
}


/*
 * Removes the given command from the list, if it is present.
 * Returns true if sucessfully removed.
 */
bool Game::removeCommand(const std::string cmd)
{
        list_edited = list->remove(cmd);
        if(!list_edited)
                std::cout << "Command not removed" << std::endl;
        return list_edited;
}
