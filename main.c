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
    int possibility;
    int count;
    struct problem *next;
};typedef struct problems problem;

struct saved_info//what we save
{
    char name[200];
    int problems[200];
    int people,treasury,court;

};typedef struct saved_info saved_info;

void SetColor(int ForgC)
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
        (*number)++;

    }
    return head;
}
int find_player(char file_name[],char player_name[],int *fpp)//checks if the game should start from the beginning or resume
{
    FILE *fp=fopen(file_name,"r+b");
    if(fp==NULL)printf("3");
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
    return -1;
}
int save(char file_name[],saved_info *info)
{
    printf("%s",file_name);
    saved_info *read_info;
    int fpp;
    if(find_player(file_name,info->name,&fpp))
    {
        FILE *fp=fopen(file_name,"r+b");
        if(fp==NULL)printf("1");
        fseek(fp,fpp,SEEK_SET);
        fwrite(info,1,sizeof(saved_info),fp);
        fclose(fp);
        return 1;
    }
    FILE *fp=fopen(file_name,"r+b");
    if(fp==NULL)printf("2");
    fseek(fp,0,SEEK_END);
    fwrite(info,1,sizeof(saved_info),fp);
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
    int people=50,treasury=50,court=50,number,primary_number;
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
            printf("Your Empire Has Fallen\nWhat Would You Like To Do?\n|1| Save and Exit\n|2| Exit\n");
            int choice;
            scanf("%d",&choice);
            if(choice==2)
            {
                printf("Goodbye!\n");
                break;
            }
            if(save("savings.bin",&my_info))
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
