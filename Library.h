#ifndef LIBRARY_H_INCLUDED
#define LIBRARY_H_INCLUDED
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
    new_node->count=new_problem.count;
    new_node->next=NULL;
    return new_node;
}

int add_end(problem **head,problem *new_problem)//add nodes to the end of link list
{
    int my_count=1;
    problem *cur=*head;
    if(*head==NULL)
    {
        *head=new_problem;
        return 1;
    }
    while(cur->next != NULL)
    {
        cur=cur->next;
        my_count++;
    }
    cur->next=new_problem;
    return 1;
}

int delete_problem(problem **head,problem *waste)//delete nodes by their address
{
    problem *cur=*head,*prev;
    if(*head==NULL) printf("No Nodes To Delete!\n");
    while(cur!=NULL)
    {
        if(cur == waste) break;
        prev=cur;
        cur=cur->next;
    }
    if(cur==NULL) printf("Node Not Found!\n");
    if(cur==*head)
    {
        *head=cur->next;
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
        text_color(4);
        text_color(0);
        problem *new_prob=create_new(new_val);
        add_end(&head,new_prob);
        fclose(fpp);
        (*number)++;

    }
    fclose(fp);
    return head;
}
int find_player(char file_name[],char player_name[],int *fpp)//check if the player has any history
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
    if(info.status==1)
    {
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}
void swap(int *a,int *b)//swap 2 integers
{
    int temp=*a;
    *a=*b;
    *b=temp;
}
void score_boared(char file_name[],char player_name[])//prints the score board
{
    FILE *fp=fopen(file_name,"r");
    int i,j,color1=11,color2=10;
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
        if(strcmp(info.name,player_name)==0)
        {
            color1=5;
            color2=18;
        }
        text_color(color1);
        printf("  Player Rank %-2d: %-25s",i+1,info.name);
        text_color(color2);
        printf("-->Score: %d\n",score[i]);
        text_color(0);
        color1=11;
        color2=10;
    }
    fclose(fp);
    text_color(8);
    printf("\nPress Any Key To Exit...");
}
void play_music(int a1,int a2,int a3)//play a suitable music :)
{
    Beep(a1,400);
    Beep(a2,400);
    Beep(a3,400);
}

#endif // LIBRARY_H_INCLUDED
