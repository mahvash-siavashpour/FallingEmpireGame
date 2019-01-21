#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#include <math.h>
#define FILE1 "CHOICES.txt"
#define FILE2 "SAVE/thesave.bin"
#define FILE3 "SAVE/power.bin"

struct problems//the entire thing
{
    char   question[200];
    char   ans1[200];
    int    people[2],court[2],treasury[2];
    char   ans2[200];
    int    possibility;//3-0
    int    count;//number of the node
    struct problem *next;
};typedef struct problems problem;

struct saved_info//information we save
{
    char name[200];//players name
    int  problems[200];//possibility of nodes in an array
    int  people,treasury,court;
    int  status;//saved meanwhile playing or when lost

};typedef struct saved_info saved_info;

void console_color(int ForgC, int BackC)//color of the console
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
problem *create_new(problem new_problem)//create new nodes for each problem
{
    problem *new_node=(problem *)malloc(sizeof(problem));
    assert((new_node!=NULL));
    strcpy(new_node->question,new_problem.question);
    strcpy(new_node->ans1,new_problem.ans1);
    strcpy(new_node->ans2,new_problem.ans2);
    new_node->people[0]=new_problem.people[0];
    new_node->people[1]=new_problem.people[1];
    new_node->court[0]=new_problem.court[0];
    new_node->court[1]=new_problem.court[1];
    new_node->treasury[0]=new_problem.treasury[0];
    new_node->treasury[1]=new_problem.treasury[1];
    new_node->possibility=new_problem.possibility;
    new_node->next=NULL;
}

int add_end(problem **head,problem *new_problem)//add nodes to the end of link list
{
    int my_count=0;
    problem *cur=*head;
    if(*head==NULL)
    {
        *head=new_problem;
        (*head)->count=0;
        return 1;
    }
    while(cur->next != NULL)
    {
        cur=cur->next;
        my_count++;
    }
    cur->next=new_problem;
    cur->count=my_count;
    return 1;
}

int delete_problem(problem **head,problem *waste)//delete nodes by their address
{
    problem *cur=*head,*prev;
    if(*head==NULL) return -1;
    while(cur != waste && cur!=NULL)
    {
        prev=cur;
        cur=cur->next;
    }
    if(cur==NULL) return -2;
    if(cur==*head)
    {
        *head=NULL;
        free(cur);
        return 1;
    }
    prev->next=cur->next;
    free(cur);
    return 1;
}

problem *make_linkedlist(char name[],int *number)//makes a linked list using the name of the main text file given
{
    (*number)=0;
    char  q_file[200];
    FILE  *fp=fopen(name,"r");
    assert(fp!=NULL);//check if we can even open the choices file!
    problem new_val,*head=NULL;
    while(1)
    {
        if(feof(fp)) break;
        fscanf(fp,"%s",q_file);
        FILE *fpp=fopen(q_file,"r");
        assert(fpp!=NULL);//check if we can even open the question file!
        //read the THING
        fgets(new_val.question,200,fpp);
        fgets(new_val.ans1,200,fpp);
        fscanf(fpp,"%d",&new_val.people[0]);
        fscanf(fpp,"%d",&new_val.court[0]);
        fscanf(fpp,"%d",&new_val.treasury[0]);
        fgets(new_val.ans2,200,fpp);
        fgets(new_val.ans2,200,fpp);
        fscanf(fpp,"%d",&new_val.people[1]);
        fscanf(fpp,"%d",&new_val.court[1]);
        fscanf(fpp,"%d",&new_val.treasury[1]);
        new_val.possibility=3;
        new_val.count=(*number);
        problem *new_prob=create_new(new_val);
        add_end(&head,new_prob);
        fclose(fpp);
        (*number)++;

    }
    fclose(fp);
    return head;
}
int find_player(char file_name[],char player_name[],int *fpp)//checks if the player has any history
{
    saved_info  read;
    FILE       *fp=fopen(file_name,"a+b");
    assert(fp!=NULL);
    fseek(fp,0,SEEK_SET);
    (*fpp)=0;
    while(1)
    {
        if(fread(&read,sizeof(read),1,fp)<1) break;
        if(strcmp(read.name,player_name)==0)
        {
            (*fpp)=ftell(fp)-sizeof(read);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return -1;
}
int save(char file_name[],saved_info info)//save the game
{
    int fpp;
    if(find_player(file_name,info.name,&fpp)==1)
    {
        FILE *fp=fopen(file_name,"r+b");
        assert(fp!=NULL);
        fseek(fp,fpp,SEEK_SET);
        fwrite(&info,sizeof(info),1,fp);
        fclose(fp);
        return 1;
    }
    FILE *fp=fopen(file_name,"r+b");
    assert(fp!=NULL);
    fseek(fp,0,SEEK_END);
    fwrite(&info,sizeof(info),1,fp);
    fclose(fp);
    return 1;
}
int power_save(char file_name[],saved_info info,int primary_number)//save each round , so if we have have power issues we wont lose any information
{
    static int a=0;
    FILE      *fp;
    if(a==0)   fp=fopen(file_name,"wb");
    else       fp=fopen(file_name,"r+b");
    assert(fp!=NULL);
    a++;
    fseek(fp,0,SEEK_SET);
    fwrite(&info,sizeof(info),1,fp);
    fclose(fp);
    return 1;
}
int if_power(char file_name[])//check if in the last time playing there was a power cut
{
    FILE *fp=fopen(file_name,"a+b");
    assert(fp!=NULL);
    fseek(fp,0,SEEK_SET);
    saved_info info;
    fread(&info,sizeof(info),1,fp);
    if(info.status==1) return 1;
    return 0;
}
void swap(int *a,int *b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}
void score_boared(char file_name[])
{
    FILE *fp=fopen(file_name,"r");
    assert(fp!=NULL);
    fseek(fp,0,SEEK_SET);
    saved_info info;
    int count=0,score[200]={0},person[200]={0};
    while(1)
    {
        if(fread(&info,sizeof(info),1,fp)<1) break;
        score[count]=info.people+info.treasury+info.court;
        person[count]=count+1;
        count++;
    }
    int i,j;
    for (i=0;i<count;i++)
    {
        for(j=0;j<count-1-i;j++)
        {
            if(score[j]<score[j+1])
            {
                swap(&score[j],&score[j+1]);
                swap(&person[j],&person[j+1]);
            }
        }
    }
    if (count>10) count=10;
    printf("\n");
    for(i=0;i<count;i++)
    {
        fseek(fp,0,SEEK_SET);
        for(j=0;j<person[i];j++) fread(&info,sizeof(info),1,fp);
        text_color(11);
        printf("  Player Rank %-2d: %-10s ",i+1,info.name);
        text_color(10);
        printf("-->Score: %d\n",score[i]);
        text_color(0);
    }
    fclose(fp);
    text_color(8);
    printf("\nPress Any Key To Exit...");
}
/*
//TEXT SAVING VERSION
int find_player(char file_name[],char player_name[],int *fpp,int primary_number)//if the player has a history
{
    int i,read_temp;
    (*fpp)=0;
    saved_info info;
    FILE *fp=fopen(file_name,"a+");
    assert(fp!=NULL);
    fseek(fp,0,SEEK_SET);
    while(1)
    {
        if(fscanf(fp,"%s",info.name)==EOF)
        {
            fclose(fp);
            return -1;
        }
        if(strcmp(info.name,player_name)==0)
        {
            fclose(fp);
            return 1;
        }
        for(i=0;i<4+primary_number;i++)
                fscanf(fp,"%d",&read_temp);
        (*fpp)++;
    }
    fclose(fp);
    return -1;
}
int save(char file_name[],saved_info info,int primary_number)//save the game
{
    int j,i,fpp;
    int read_temp;
    char temp[200];
    if(find_player(file_name,info.name,&fpp,primary_number)==1)
    {
        FILE *fp=fopen(file_name,"r+");
        assert(fp!=NULL);
        fseek(fp,0,SEEK_SET);
        int count=0;
        for(j=0;j<fpp;j++)
        {
            fgets(temp,200,fp);
            int a=ftell(fp);
            for(i=0;i<4+primary_number;i++)
            {
                fscanf(fp,"%d",&read_temp);
            }
            fgets(temp,200,fp);
        }
        int a=ftell(fp);
        fseek(fp,a,SEEK_SET);
        fprintf(fp,"%s\n",info.name);
        fprintf(fp,"%d\n",info.status);
        fprintf(fp,"%d\n",info.people);
        fprintf(fp,"%d\n",info.treasury);
        fprintf(fp,"%d\n",info.court);
        for(i=0;i<primary_number;i++)
        {
            fprintf(fp,"%d\n",info.problems[i]);
        }
        fclose(fp);
        return 1;
    }
    FILE *fp=fopen(file_name,"a");
    assert(fp!=NULL);
    fseek(fp,0,SEEK_END);
    fprintf(fp,"%s\n",info.name);
    fprintf(fp,"%d\n",info.status);
    fprintf(fp,"%d\n",info.people);
    fprintf(fp,"%d\n",info.treasury);
    fprintf(fp,"%d\n",info.court);
    for(i=0;i<primary_number;i++)
    {
        fprintf(fp,"%d\n",info.problems[i]);
    }
    fclose(fp);
    return 1;
}
int power_save(char file_name[],saved_info info,int primary_number)//save each round , so if we have have power issues we wont lose any information
{
    static int a=0;
    FILE *fp;
    if(a==0)
        fp=fopen(file_name,"w");
    else
        fp=fopen(file_name,"r+");
    a++;
    assert(fp!=NULL);
    fseek(fp,0,SEEK_SET);
    fprintf(fp,"%s\n",info.name);
    fprintf(fp,"%d\n",info.status);
    fprintf(fp,"%d\n",info.people);
    fprintf(fp,"%d\n",info.treasury);
    fprintf(fp,"%d\n",info.court);
    int i;
    for(i=0;i<primary_number;i++)
    {
        fprintf(fp,"%d\n",info.problems[i]);
    }
    fclose(fp);
    return 1;
}
int if_power(char file_name[])//check if in the last time playing there was a power cut
{
    FILE *fp=fopen(file_name,"a+");
    assert(fp!=NULL);
    fseek(fp,0,SEEK_SET);
    char temp[200];
    int status;
    fgets(temp,200,fp);
    fscanf(fp,"%d",&status);
    //printf("%d",status);
    if(status==1) return 1;
    return 0;
}*/
void play_music(int a1,int a2,int a3)//play a suitable music :)
{
    Beep(a1,400);
    Beep(a2,400);
    Beep(a3,400);
}
//____________________________________________________________________________________________________________________________________________________
int main()
{
    console_color(0,15);
    int      people=50,treasury=50,court=50,choose=0,exit=1,fpp,number,primary_number,status;
    time_t   seed=time(NULL);
    problem *head=make_linkedlist(FILE1,&number),*cur,*prev;//make the linked list

    cur=head;
    prev=head;
    saved_info my_info;//save players info in this
    srand(seed);
    text_color(1);
    printf("<The Falling Empire>\n");
    play_music(300,350,400);
    primary_number=number;
    char player_name[200];
    if(if_power(FILE3)==1)//if there was a power cut last time
    {
        printf("\nYou Have an Unsaved Game! What Do You Do?\n\n|1| Resume\n|2| New Game\n>");
        scanf("%d",&choose);
    }
    if(choose==1)//resume the power cut game*********************************************************************************************************************************
    {
        int i,j,read_temp;
        FILE *fp=fopen(FILE3,"rb");
        assert(fp!=NULL);
        fseek(fp,0,SEEK_SET);
        fread(&my_info,sizeof(my_info),1,fp);
        people=my_info.people;
        treasury=my_info.treasury;
        court=my_info.court;
        for(i=0;i<primary_number;i++)
        {
            cur->possibility=my_info.problems[i];
            if(cur->possibility==-1)
            {
                delete_problem(&head,cur);
                number--;
                cur=prev;
            }
            prev=cur;
            cur=cur->next;
        }
        strcpy(player_name,my_info.name);
        text_color(4);
        printf("\nHello %s!\nIts Good To See You Back\n\n",player_name);
        text_color(0);
        Sleep(2000);
    }

    else if(choose==2 || choose==0)//start a new game regarding the power cut game******************************************************************************************
    {
        printf("Choose a Name:\n>");
        scanf("%s",player_name);
        strcpy(my_info.name,player_name);
        int i;
        for(i=0;i<primary_number;i++)
        {
            my_info.problems[i]=3;
        }
        int resume=find_player(FILE2,player_name,&fpp);//if the players name exists
        if(resume==1)//resume or new
        {
            int temp;
            text_color(4);
            printf("\nGood To See You Back %s!\nYou Can Either :\n|1| Resume Your Game\n|2| Start a New Game\n>",player_name);
            text_color(0);
            scanf("%d",&temp);
            if(temp==2) resume=0;
            Sleep(100);
        }
        else
        {
            text_color(4);
            printf("\nHi %s!\n",player_name);
            Sleep(1000);
        }
        if(resume==1)//load a game
        {
            int i,j,read_temp;
            char temp[200];
            FILE *fp=fopen(FILE2,"rb");
            assert(fp!=NULL);
            fseek(fp,fpp,SEEK_SET);
            fread(&my_info,sizeof(my_info),1,fp);
            cur=head;
            prev=head;
            if(my_info.status!=-1)
            {
                people=my_info.people;
                treasury=my_info.treasury;
                court=my_info.court;
                for(i=0;i<primary_number;i++)
                {
                    cur->possibility=my_info.problems[i];
                    if(cur->possibility==-1)
                    {
                        delete_problem(&head,cur);
                        number--;
                        cur=prev;
                    }
                    prev=cur;
                    cur=cur->next;
                }
            }
        }
    }

    system("cls");
    text_color(1);
    printf("<The Falling Empire>\n");
    text_color(2);
    printf("|People: %d|___|Treasury: %d|___|Court: %d|\n\n",people,treasury,court);
    text_color(0);
    while(1)//the entire GAME!_______________________________________________________________________________________________________________________
    {
        int i,end_loop=abs(rand())%number,choice;
        float average;
        cur=head;
        for (i=0;i<end_loop;i++)  cur=cur->next;//random problem choosing
        my_info.problems[cur->count]--;
        cur->possibility--;
        text_color(0);
        printf("%s\n",cur->question);
        text_color(13);
        printf("|1| %s|2| %s>",cur->ans1,cur->ans2);
        text_color(0);
        if(cur->possibility==0)
        {
            my_info.problems[cur->count]=-1;
            delete_problem(&head,cur);
            number--;
        }
        if(head==NULL)//if no problem left
        {
            head=make_linkedlist(FILE1,&number);
            for(i=0;i<primary_number;i++)
            {
                my_info.problems[i]=3;
            }
            number=primary_number;
        }
        scanf("%d",&choice);
        if(choice==0)
        {
            exit=0;
        }
        else if(choice==1 || choice ==2)
        {
            choice--;
            //do the changes to the government
            people+=cur->people[choice];     if(people<0) people=0;     else if(people>100) people=100;
            treasury+=cur->treasury[choice]; if(treasury<0) treasury=0; else if(treasury>100) treasury=100;
            court+=cur->court[choice];       if(court<0) court=0;       else if(court>100) court=100;
        }
        else
        {
            text_color(12);
            printf("Invalid Input\n");
            return -1;
        }
        my_info.people=people;
        my_info.treasury=treasury;
        my_info.court=court;
        average=(people+treasury+court)/3;
        system("cls");
        text_color(1);
        printf("<The Falling Empire>\n");
        text_color(2);
        printf("|People: %d|___|Treasury: %d|___|Court: %d|\n\n",people,treasury,court);
        text_color(0);
        my_info.status=1;
        power_save(FILE3,my_info,primary_number);
        if(people==0 || treasury==0 || court==0 || average<=10 || exit==0)//end of game
        {
            int choice,ch;
            my_info.status=0;
            text_color(12);
            if(exit!=0)
            {
                my_info.status=-1;
                play_music(550,600,650);
                printf("Your Empire Has Fallen \n");
            }
            printf("What Would You Like To Do?\n|1| Save and Exit\n|2| Exit\n>");
            scanf("%d",&choice);
            text_color(2);
            if(choice==2)
            {
                printf("\nGoodbye! ^__^\n");
                return 0;
            }
            else if(save(FILE2,my_info) && choice==1)
            {
                power_save(FILE3,my_info,primary_number);
                printf("\nSaved Successfully! ^__^\n");
                getch();
                system("cls");
                text_color(1);
                printf("______Press 1 to See the Score Board and Press 2 to Exit______\n>");
                scanf("%d",&ch);
                if(ch==1)
                {
                    score_boared(FILE2);
                    break;
                }
                return 0;
            }
            else
            {
                text_color(12);
                printf("Invalid Input\n");
                return -1;
            }
        }

    }
    //Beep(300, 500);
    //printf("\a");
    getch();
    text_color(0);
    return 0;
}
