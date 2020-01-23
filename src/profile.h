#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>

/*
 * Author: Jonathan Manly
 * Description:
 *      Profile class that contains the player's name
 *      and score. class Game is a friend so it can
 *      edit the private variables. To be honest, this
 *      could be a struct, except I wanted explicit constructor
 *      and accessor methods.
 */

class Profile
{
    public:
        /* Functions  */
        Profile(std::string name = "foo", int score = 0);
        int getScore() const;
        std::string getName() const;

    private:
        friend class Game;
        /* Variables */
        std::string name;
        int score;

        /* Functions */
};

#endif // PLAYER_H
