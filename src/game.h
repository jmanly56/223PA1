#ifndef GAME_H
#define GAME_H
#include "list.h"
#include "profile.h"

/*
 * Author: Jonathan Manly
 * Desciprtion:
 *      Game class controls all elements of the game.
 *      This class is a friend of the Profile class.
 */

class Game
{
    public:
        /* Functions  */
        Game();
        ~Game();
        void init();
        void run();
        void exit();

    private:
        /* Types */
        enum CSV { Commands, Profiles };
        enum MenuOptions { Rules = 1, Play, Load, Add, Remove, Exit };
        typedef std::string Description;
        typedef std::string Command;
        struct Question {
                Command cmd;
                Description descriptions[3];
                Description answer;
        };
        /* Variables */
        const std::string profile_path = "profiles.csv";
        const std::string command_path = "commands.csv";
        size_t current_arr_size;
        size_t num_profiles;
        bool list_edited;
        Profile *current;
        Profile *profiles; // Going to be an array
        List<Command, Description> *list; // cmd,desc

        /* Functions */
        void cleanup();
        void loadFile(CSV type);
        void insertAtFront(Profile &profile);
        void parseLine(std::string line, std::string &data1,
                       std::string &data2);
        void saveFile(CSV type);
        void showRules();
        void showMenu();
        void playGame(bool loaded = false);
        int getNumQuestions();
        Question getQuestion();
        void createQuestion(Node<Command, Description> *node, Question &q);
        int askQuestion();
        Profile *createUser();
        Profile *findUser();
        bool nameTaken(std::string name);
        bool loadPreviousGame();
        void getNewCommand(MenuOptions option);
        bool addCommand(const std::string cmd, const std::string desc);
        bool removeCommand(const std::string cmd);
};

#endif // GAME_H
