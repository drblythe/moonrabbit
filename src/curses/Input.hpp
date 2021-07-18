#ifndef INPUT_HPP
#define INPUT_HPP

#include <ncurses.h>
#include <string>

namespace Curses
{

class Input
{
public:
    Input();
    int listen();
    std::string getKey();
private:
};

static Input *INPUT = new Input();

}
#endif
