#include "profile.h"

Profile::Profile(std::string name, int score)
{
        this->name = name;
        this->score = score;
}

int Profile::getScore() const
{
        return score;
}

std::string Profile::getName() const
{
        return name;
}
