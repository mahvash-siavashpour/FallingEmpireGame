#ifndef MYCONIO_H_INCLUDED
#define MYCONIO_H_INCLUDED
void console_color(int ForgC, int BackC)//color of the console
 {
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    //Get the handle to the current output buffer...
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    //This is used to reset the carat/cursor to the top left.
    COORD coord = {0, 0};
    //A return value... indicating how many chars were written.not used but we need to capture this since it will be.
    //written anyway (passing NULL causes an access violation).
    DWORD count;
    //This is a structure containing all of the console info.it is used here to find the size of the console.
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    //Here we will set the current color
    SetConsoleTextAttribute(hStdOut, wColor);
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //This fills the buffer with a given character (in this case 32=space).
        FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);

        FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count );
        //This will set our cursor position for the next print statement.
        SetConsoleCursorPosition(hStdOut, coord);
    }
    return;
}

void text_color(int ForgC)//text color
 {
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    //We use csbi for the wAttributes word.
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //Mask out all but the background attribute, and add in the forgournd     color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
 }


#endif // MYCONIO_H_INCLUDED
