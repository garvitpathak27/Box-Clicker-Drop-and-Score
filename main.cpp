// VIDEO LINK https://www.youtube.com/watch?v=PXnhYBG0AEA&t=4245s
#include <iostream>
#include "game.h" // this is a pre compile header ... ///
/*
WHY USE PRE COMPILE HEADER ??

    according to wikipedia

            To reduce compilation times, some compilers allow header files to be compiled into a form that is faster for the compiler to process. This intermediate form is known as a precompiled header, and is commonly held in a file named with the extension . pch or similar, such as . gch under the GNU Compiler Collection.

*/

using namespace std;
using namespace sf;

int main()
{   
    // initializing random
    srand(static_cast<unsigned>(time(NULL)));

    game g;
    while (g.running() && !g.getendgame())
    {
        g.update();
        g.render();
    }
    return 0;
}
