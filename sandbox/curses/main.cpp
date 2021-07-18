#include "../../src/curses/Curses.hpp"
#include "../../src/curses/Display.hpp"
#include "../../src/curses/Input.hpp"
#include "../../src/system/Filer.hpp"
#include "../../src/system/System.hpp"
#include <filesystem>

using namespace Curses;
using namespace System;

int main ()
{
    static bool err = false;

    // Set starting working dir
    err = FILER->setCwd(fs::path("/home/susi"));

    std::string input;
    int rowIndex = 0;
    for (fs::directory_entry file : FILER->getFileList())
        {
            Curses::DISPLAY->print(file.path().filename(), rowIndex++, 0);
        }

    while (true)
        {
            input = INPUT->getKey();
            if (input == "q")
                {
                    break;
                }
            DISPLAY->print("You typed " + input, rowIndex, 0);
        }
    DISPLAY->terminate();
    return 0;
}

void
displayFileList()
{
}
