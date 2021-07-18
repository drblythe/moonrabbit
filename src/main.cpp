#include "curses/Curses.hpp"
#include "curses/Display.hpp"
#include "curses/Input.hpp"
#include "system/Filer.hpp"
#include "system/System.hpp"
#include <curses.h>
#include <filesystem>

using namespace Curses;
using namespace System;

int main ()
{
    static bool err = false;

    // Set starting working dir
    err = FILER->setCwd(fs::path("/home/susi"));

    std::string input;
    int activeRowIndex = 0;

    bool run = true;
    while (run)
        {
            // TODO: rename getnumfiles in dir to "fileCount"
            for (int i = 0; i < FILER->getNumFilesInDir(); i++)
                {
                    if (i == activeRowIndex)
                        {
                            DISPLAY->print(FILER->getFileList().at(i).path().filename(), i, 0,
                                           A_REVERSE);
                        }
                    else
                        {
							DISPLAY->print(FILER->getFileList().at(i).path().filename(), i, 0);

                        }
                }
            //for (fs::directory_entry file : FILER->getFileList())
            //    {
            //        Curses::DISPLAY->print(file.path().filename(), activeRowIndex++, 0);
            //    }

            input = INPUT->getKey();
            if (input == "q")
                {
                    run = false;
                }
            else if (input == "j")
                {
					activeRowIndex++;
                }
        }

    DISPLAY->terminate();
    return 0;
}
