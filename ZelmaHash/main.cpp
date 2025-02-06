#include "include/userInterface.h"
#include "include/zelmaCracker.h"

int main()
{
    srand(time(NULL));

    int menuIndex = 0;

    while(true)
    {
        switch (menuIndex)
        {
        case 0:
            menuIndex = mainMenu();
            continue;
        
        case 1:
            menuIndex = newUser();
            continue;

        case 2:
            menuIndex = logIn();
            continue;

        case 3:
            menuIndex = runZelmaCracker(SINGLE);
            continue;

        case 4:
            menuIndex = runZelmaCracker(DATABASE);
            continue;

        default:
            break;
        }
        break;
    }

    return 0;
}