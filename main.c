#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dos.h>
#include <windows.h>

struct problems//the entire shit
{
    char question[200];
    char ans1[200];
    int people[2],court[2],treasury[2];
    char ans2[200];
    int possibility;//3-0
    int count;//number of the node
    struct problem *next;
};typedef struct problems problem;

struct saved_info//what we save
{
    char name[200];//players name
    int problems[200];//possibility of nodes in an array
    int people,treasury,court;
    int status;//saved meanwhile playing or when lost

};typedef struct saved_info saved_info;

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
problem *create_new(problem new_problem)//create new nodes
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

problem *make_linkedlist(char name[],int *number)//makes a linked list using the name of the text file given
{
    (*number)=0;
    char q_file[200];
    FILE *fp=fopen(name,"r");
    assert(fp!=NULL);//check if we can even open the choices file!
    problem new_val,*head=NULL;
    while(1)
    {
        if(feof(fp)) break;
        fscanf(fp,"%s",q_file);
        FILE *fpp=fopen(q_file,"r");
        assert(fpp!=NULL);//check if we can even open the question file!
        //read the shit:
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
        new_val.count=0;
        problem *new_prob=create_new(new_val);
        add_end(&head,new_prob);
        fclose(fpp);
        (*number)++;

    }
    fclose(fp);
    return head;
}
/*int find_player(char file_name[],char player_name[],int *fpp)//checks if the player has any history
{
    FILE *fp=fopen(file_name,"a+b");
    if(fp==NULL)printf("3");
    fseek(fp,0,SEEK_SET);
    saved_info *read;
    while(1)
    {
        if(feof) break;
        fread(read,1,sizeof(saved_info),fp);
        if(strcmp(read->name,player_name)==0)
        {
            *fpp=ftell(fp)-sizeof(saved_info);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return -1;
}
int save(char file_name[],saved_info *info)
{
    saved_info *read_info;
    int fpp;
    if(find_player(file_name,info->name,&fpp))
    {
        FILE *fp=fopen(file_name,"a+b");
        if(fp==NULL)printf("1");
        fseek(fp,fpp,SEEK_SET);
        fwrite(info,1,sizeof(saved_info),fp);
        fclose(fp);
        return 1;
    }
    FILE *fp=fopen(file_name,"a+b");
    if(fp==NULL)printf("2");
    fseek(fp,0,SEEK_END);
    fwrite(info,1,sizeof(saved_info),fp);
    fclose(fp);
    return 1;
}*/
int find_player(char file_name[],char player_name[],int *fpp,int primary_number)//if the player has a history
{
    int i,read_temp;
    (*fpp)=0;
    saved_info info;
    FILE *fp=fopen(file_name,"r");
    assert(fp!=NULL);
    fseek(fp,0,SEEK_SET);
    while(1)
    {
        if(fscanf(fp,"%s",info.name)==EOF)
        {
            fclose(fp);
            return -1;
        }
        printf("len1: %d    len2: %d\n",strlen(info.name),strlen(player_name));
        if(strcmp(info.name,player_name)==0)
        {
            printf("similar\n");
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
int save(char file_name[],saved_info info,int primary_number)//save the game here
{
    int j,i,fpp;
    int read_temp;
    char temp[200];
    if(find_player(file_name,info.name,&fpp,primary_number)==1)
    {
        printf("****\n");
        FILE *fp=fopen(file_name,"r+");
        assert(fp!=NULL);
        fseek(fp,0,SEEK_SET);
        printf("%d\n",fpp);
        for(j=0;j<fpp;j++)
        {
            fgets(temp,200,fp);
            int a=ftell(fp);
            printf("%d\n",a);
            for(i=0;i<4+primary_number;i++)
            {
                fscanf(fp,"%d",&read_temp);
                //printf("%d\n",read_temp);
                int a=ftell(fp);
                printf("%d\n",a);
            }
            fgets(temp,200,fp);
            a=ftell(fp);
            printf("%d\n",a);
            printf("%s\n",temp);
        }
        fprintf(fp,"%s\n",info.name);
        fprintf(fp,"%d\n",info.status);
        fprintf(fp,"%d\n",info.people);
        fprintf(fp,"%d\n",info.treasury);
        fprintf(fp,"%d\n",info.court);
        printf("here\n");
        for(i=0;i<primary_number;i++)
        {
            fprintf(fp,"%d\n",info.problems[i]);
        }
        fclose(fp);
        return 1;
    }
    FILE *fp=fopen(file_name,"r+");
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
void play_music(void)
{
    Beep(300,200);
    Beep(350,200);
    Beep(400,200);
}
int main()
{
    console_color(0,23);
    int people=50,treasury=50,court=50,number,primary_number,fpp,status;
    time_t seed=time(NULL);
    srand(seed);
    saved_info my_info;//save players info in this
    play_music();//get the name
    printf("Choose a Name: \n");
    char player_name[200];
    scanf("%s",player_name);
    strcpy(my_info.name,player_name);
    problem *head=make_linkedlist("CHOICES.txt",&number);//make the linked list
    primary_number=number;
    problem *cur=head;
    int i;
    for(i=0;i<primary_number;i++)
    {
        my_info.problems[i]=3;
    }
    if(find_player("save.txt",player_name,&fpp,primary_number))//load a game
    {
        int i,j,read_temp;
        char temp[200];
        FILE *fp=fopen("save.txt","r");
        assert(fp!=NULL);
        fseek(fp,0,SEEK_SET);
        for(j=0;j<fpp;j++)
        {
            fgets(temp,200,fp);
            for(i=0;i<4+primary_number;i++)
            {
                fscanf(fp,"%d",&read_temp);
                //printf("%d\n",read_temp);
            }
            fgets(temp,200,fp);//waste
        }
        fgets(temp,200,fp);//players name
        fscanf(fp,"%d",&status);
        if(status!=-1)
        {
            fscanf(fp,"%d",&people);
            fscanf(fp,"%d",&treasury);
            fscanf(fp,"%d",&court);
            for(i=0;i<primary_number;i++)
            {
               fscanf(fp,"%d",&my_info.problems[i]);
               cur->possibility=my_info.problems[i];
               if(cur->possibility==0)
                {
                    my_info.problems[cur->count]=-1;
                    delete_problem(&head,cur);
                    number--;
                }
               cur=cur->next;
            }
        }
    }
    printf("|People: %d| |Treasury: %d| |Court: %d| \n",people,treasury,court);
    while(1)//the entire GAME!
    {
        int i,end_loop=rand()%number,choice;
        float average;
        cur=head;
        for(i=0;i<end_loop;i++)//random problem choosing
            cur=cur->next;
        my_info.problems[cur->count]--;
        cur->possibility--;
        printf("%s\n|1| %s\n|2| %s",cur->question,cur->ans1,cur->ans2);
        if(cur->possibility==0)
        {
            my_info.problems[cur->count]=-1;
            delete_problem(&head,cur);
            number--;
        }
        if(head==NULL)//if no problem left
        {
            head=make_linkedlist("CHOICES.txt",&number);
            for(i=0;i<primary_number;i++)
            {
                my_info.problems[i]=3;
            }
            number=primary_number;
        }
        scanf("%d",&choice);
        choice--;
        //do the changes to the government
        people+=cur->people[choice];     if(people<0) people=0;     else if(people>100) people=100;
        treasury+=cur->treasury[choice]; if(treasury<0) treasury=0; else if(treasury>100) treasury=100;
        court+=cur->court[choice];       if(court<0) court=0;       else if(court>100) court=100;

        my_info.people=people;
        my_info.treasury=treasury;
        my_info.court=court;

        average=(people+treasury+court)/3;
        printf("|People: %d| |Treasury: %d| |Court: %d| \n",people,treasury,court);
        if(people==0 || treasury==0 || court==0 || average<=10)//end of game
        {
            my_info.status=-1;
            printf("Your Empire Has Fallen\nWhat Would You Like To Do?\n|1| Save and Exit\n|2| Exit\n");
            int choice;
            scanf("%d",&choice);
            if(choice==2)
            {
                printf("Goodbye!\n");
                break;
            }
            if(save("save.txt",my_info,primary_number))
            {
                printf("Saved Successfully!\n");
                break;
            }

        }

    }
    //Beep(300, 500);
    //printf("\a");
    return 0;
}
