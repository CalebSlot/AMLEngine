#include "Demos.h"

int main()
{
    AMLEngine::Core ame(800, 600, "AMLEngineDemos");

    Demos demos(ame);

    demos.Play();


    return 0;
}

