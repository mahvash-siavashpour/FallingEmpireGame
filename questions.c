#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <assert.h>
void console_color(int ForgC, int BackC)
 {
 WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
               //Get the handle to the current output buffer...
 HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
                     //This is used to reset the carat/cursor to the top left.
 COORD coord = {0, 0};
                  //A return value... indicating how many chars were written
                    //   not used but we need to capture this since it will be
                      //   written anyway (passing NULL causes an access violation).
  DWORD count;

                               //This is a structure containing all of the console info
                      // it is used here to find the size of the console.
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

void text_color(int ForgC)
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
int main()
{
    console_color(15,28);
    printf("How Many Questions Are You Going To Write?\n");
    int n,i;
    scanf("%d",&n);
    system("cls");
    for(i=0;i<n;i++)
    {
        text_color(14);
        printf("Question Number%d:\n",i+1);
        text_color(15);
        FILE *fp=fopen("CHOICES.txt","a");
        assert(fp!=NULL);
        char file_name[20];
        printf("Write The File Name Here:\n>");
        scanf("%s",file_name);
        fprintf(fp,"%s\n",file_name);
        FILE *fpp=fopen(file_name,"w");
        assert(fpp!=NULL);
        char question[200],ans1[200],ans2[200];
        int tmp;
        printf("Write The Question Here:\n>");
        gets(question);
        gets(question);
        fprintf(fpp,"%s\n",question);
        printf("Write The First Option Here:\n>");
        gets(ans1);
        fprintf(fpp,"%s\n",ans1);
        text_color(23);
        printf("Write The Affects of This Option Here:\n>");
        scanf("%d",&tmp);
        fprintf(fpp,"%d\n",tmp);
        printf(">");
        scanf("%d",&tmp);
        fprintf(fpp,"%d\n",tmp);
        printf(">");
        scanf("%d",&tmp);
        fprintf(fpp,"%d\n",tmp);
        text_color(15);
        printf("Write The Second Option Here:\n>");
        gets(ans2);
        gets(ans2);
        fprintf(fpp,"%s\n",ans2);
        text_color(23);
        printf("Write The Affects of This Option Here:\n>");
        scanf("%d",&tmp);
        fprintf(fpp,"%d\n",tmp);
        printf(">");
        scanf("%d",&tmp);
        fprintf(fpp,"%d\n",tmp);
        printf(">");
        scanf("%d",&tmp);
        fprintf(fpp,"%d\n",tmp);
        fclose(fpp);
        fclose(fp);
        system("cls");

    }
    return 0;
}
