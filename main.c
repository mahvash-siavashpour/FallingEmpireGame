#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#include <math.h>
#include "Library.h"
#include "MyConio.h"
#define FILE1 "CHOICES.txt"
#define FILE2 "SAVE/thesave.bin"
#define FILE3 "SAVE/power.bin"

//____________________________________________________________________________________________________________________________________________________________________________
int main()
{
    console_color(0,15);
    int      people=50,treasury=50,court=50,choose=0,exit=1,fpp,number,primary_number,status;
    time_t   seed=time(NULL);
    problem *head=make_linkedlist(FILE1,&number),*cur,*prev;//make the linked list
    srand(seed);
    cur=head;
    prev=head;
    saved_info my_info;//save players info in this
    text_color(1);
    printf("<The Falling Empire>\n");
    play_music(600,700,800);
    primary_number=number;
    char player_name[200];
    text_color(9);
    if(if_power(FILE3)==1)//if there was a power cut last time
    {
        text_color(6);
        printf("\nYou Have an Unsaved Game! What Do You Do?\n\n|1| Resume\n|2| New Game\n>");
        scanf("%d",&choose);
        text_color(9);
    }
    if(choose==1)//resume the power cut game
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
        text_color(5);
        printf("\nHello %s!\nIts Good To See You Back\n\n",player_name);
        text_color(0);
        fclose(fp);
        Sleep(2000);
    }

    else if(choose==2 || choose==0)//start a new game regardless of the power cut game
    {
        printf("Choose a Name:\n>");
        if(choose==2) gets(player_name);
        gets(player_name);
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
            text_color(5);
            printf("\nGood To See You Back %s!\nYou Can Either :\n|1| Resume Your Game\n|2| Start a New Game\n>",player_name);
            text_color(0);
            scanf("%d",&temp);
            if(temp==2) resume=0;
            Sleep(100);
        }
        else
        {
            text_color(5);
            printf("\nHi %s!\n",player_name);
            Sleep(1000);
        }
        if(resume==1)//load a game
        {
            int i,j,read_temp;
            char temp[200];
            saved_info my_info_temp;
            FILE *fp=fopen(FILE2,"rb");
            assert(fp!=NULL);
            fseek(fp,fpp,SEEK_SET);
            fread(&my_info_temp,sizeof(my_info),1,fp);
            cur=head;
            prev=head;
            if(my_info_temp.status!=-1)
            {
                my_info=my_info_temp;
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
            fclose(fp);
        }
    }

    system("cls");
    text_color(1);
    printf("<The Falling Empire>\n");
    text_color(2);
    printf("|People: %d|___|Treasury: %d|___|Court: %d|\n\n",people,treasury,court);
    text_color(0);
    while(1)//the entire GAME!____________________________________________________________________________________________________________________________________________________
    {
        int i,end_loop=abs(rand())%number,choice;
        float average;
        cur=head;
        for (i=0;i<end_loop;i++)  cur=cur->next;//random problem choosing
        (my_info.problems[cur->count])--;
        (cur->possibility)--;
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
        if(number==0)//if no problem left
        {
            head=make_linkedlist(FILE1,&number);
            for(i=0;i<primary_number;i++)
            {
                my_info.problems[i]=3;
            }
            number=primary_number;
        }
        scanf("%d",&choice);
        while(choice!=1 && choice!=2 && choice!=0)
        {
            text_color(12);
            printf("Invalid Input\nChoose Again>");
            scanf("%d",&choice);
        }
        if(choice==0)
        {
            exit=0;
        }
        else if(choice==1 || choice ==2)
        {
            choice--;
            //do the changes to the government
            people+=(cur->people[choice]);     if(people<0) people=0;     else if(people>100) people=100;
            treasury+=(cur->treasury[choice]); if(treasury<0) treasury=0; else if(treasury>100) treasury=100;
            court+=(cur->court[choice]);       if(court<0) court=0;       else if(court>100) court=100;
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
            while(choice!=1 && choice!=2)
            {
                text_color(12);
                printf("Invalid Input\nEnter Again>");
                scanf("%d",&choice);
            }
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
                while(ch!=1 && ch!=2)
                {
                    text_color(12);
                    printf("Invalid Input\nEnter Again>");
                    scanf("%d",&ch);
                }
                if(ch==1)
                {
                    score_boared(FILE2,player_name);
                    break;
                }
                return 0;
            }
        }

    }
    getch();
    text_color(0);
    return 0;
}
