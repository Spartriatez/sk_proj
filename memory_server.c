#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 1214
#define QUEUE_SIZE 5
#define BUFSIZE 10000
#define N 8

struct matrix_data
{
    int x;
    int **tabl;
};
struct matrix_data matrix;
int sprawdz(int **tab,int x, int y)
{
    if(tab[x][y]==0)
        return 2;
    else
        return 1;
}
void generuj(int **tab,int kafelek,int n)
{
    int x,y,i,j;
    int bool;
    srand(time(NULL));
    for(i=0;i<kafelek;i++)
    {
        for(j=0;j<2;j++)
        {
            x=rand()%(n);
            y=rand()%(n);
            bool=sprawdz(tab,x,y);
            if(bool==2)
            {
                tab[x][y]=i+1;
            }
            else
            {
                while(bool!=2)
                {
                    x=rand()%(n);
                    y=rand()%(n);
                    bool=sprawdz(tab,x,y);
                }
                tab[x][y]=i+1;
            }
                
        }
    }    
  
 
}
int **alloc_2d_int(int rows, int cols) {
    int *data = (int *)malloc(rows*cols*sizeof(int));
    int **array= (int **)malloc(rows*sizeof(int*));
    for (int i=0; i<rows; i++)
        array[i] = &(data[cols*i]);

    return array;
}
int convert(char *t,int *tab)
{
    int tb[4];
    int k=0;
    int i=0;
    int ile_znakow=0;
    char temp[4];
    int q;
    printf("%ld\n",sizeof(t));
    while(i<sizeof(t))
    {
        if(t[i]==' ')
        {
            tb[k++]=atoi(temp);
            for(q=0;q<ile_znakow;q++)
                temp[q]=0;
            ile_znakow=0;
            i++;
        }else if(i==sizeof(t)-1)
        {
            tb[k]=atoi(temp);
            break;
        }
        else{
            temp[ile_znakow++]=t[i];
            i++;
        
        }
    }
    int sum=0;
    for(i=0;i<4;i++)
    {
        if(tb[i]>=N)
        {
            sum+=1;
        }
        else 
            sum+=0;
    }
  // printf("sum %d\n",sum);
   if(sum==0)
   {    
       for(i=0;i<4;i++)
           tab[i]=tb[i];
       return 2;
   }
   else
       return 1;
}

void sprawdz_poprawnosc(int *tb)
{
    if(ntohl(matrix.tabl[tb[0]][tb[1]])==ntohl(matrix.tabl[tb[2]][tb[3]]))
    {
        matrix.tabl[tb[0]][tb[1]]=htonl(255);
        matrix.tabl[tb[2]][tb[3]]=htonl(255);
    }
}

struct TThreadData{
    int secSocket;
    short end;
};

void* readingThread(void* data){
    int odp;
    struct TThreadData* for_thread=(struct TThreadData*)data;
    while(1) {
      //  printf("%d\n",for_thread->end);
        if(for_thread->end==0)
        {
            write(for_thread->secSocket,&matrix.x,sizeof(matrix.x));
            write(for_thread->secSocket,*(matrix.tabl),N*N*sizeof(int));
            for_thread->end=1;
        }
        //if(for_thread->end==1) {for_thread->end=1;}
    }
    return NULL;
}

int main(int argc, char* argv[]){
    
    /*----------------------------------*/
    int **tab;
    int i,j;
    int kafelek=N*N/2;
    matrix.x=N;
    tab=(int**)malloc(N*sizeof(int*));
    for(i=0;i<N;i++)
        tab[i]=(int*)malloc(N*sizeof(int));
       
       
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            tab[i][j]=0;
    generuj(tab,kafelek,N);
    matrix.tabl=alloc_2d_int(N,N);
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++){
              matrix.tabl[i][j]=htonl(tab[i][j]);
              printf("%d ",tab[i][j]);
        }
        printf("\n");
    }
    printf("\n");
  
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++){
              printf("%d ",matrix.tabl[i][j]);
        }
        printf("\n");
    }
     printf("\n");
    printf("matrix %d\n",ntohl(matrix.tabl[1][1]));
    /*----------------------------------*/
    int nSocket, nClientSocket;
    int nBind, nListen;
    int nFoo = 1;
    socklen_t nTmp;
    struct sockaddr_in stAddr, stClientAddr;

    /* address structure */
    memset(&stAddr, 0, sizeof(struct sockaddr));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(SERVER_PORT);
   // printf("%d\n",stAddr.sin_port);
    /* create a socket */
    nSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (nSocket < 0)
    {
        fprintf(stderr, "%s: Can't create a socket.\n", argv[0]);
        exit(1);
    }
    setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

    /* bind a name to a socket */
    nBind = bind(nSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
    if (nBind < 0)
    {
        fprintf(stderr, "%s: Can't bind a name to a socket.\n", argv[0]);
        exit(1);
    }
    /* specify queue size */
    nListen = listen(nSocket, QUEUE_SIZE);
    if (nListen < 0)
    {
        fprintf(stderr, "%s: Can't set queue size.\n", argv[0]);
    }
    int odp;
    char buffer[BUFSIZE];
    /* block for connection request */
    nTmp = sizeof(struct sockaddr);
    nClientSocket = accept(nSocket, (struct sockaddr*)&stClientAddr, &nTmp);
    if (nClientSocket < 0)
    {
        fprintf(stderr, "%s: Can't create a connection's socket.\n", argv[0]);
        exit(1);
    }
    pthread_t tid;
    struct TThreadData for_thread;
    for_thread.secSocket=nClientSocket;
    for_thread.end=0;
    pthread_create(&tid, NULL, readingThread, (void*)&for_thread);
    int flags = fcntl(for_thread.secSocket, F_GETFL, 0);
    fcntl(for_thread.secSocket, F_SETFL, flags | O_NONBLOCK);
    int *tb;
    tb=(int*)malloc(4*sizeof(int));
    while(1){
        //while ((odp = read (for_thread.secSocket, buffer, BUFSIZE)) == 0); // wait for a message;
            int spr;
            odp = read (for_thread.secSocket, buffer, BUFSIZE);
            if((odp==-1)&&(errno==EAGAIN)) {sleep(1); continue;}
            if((odp<0)||(buffer[0]=='n')){for_thread.end=1; break;}
            else{
                spr=convert(buffer,tb);
               for(i=0;i<4;i++)
                    printf("%d ",tb[i]);
                //printf(" %d\n",spr);
               sprawdz_poprawnosc(tb);
                if(spr==2)
                    for_thread.end=0;
                else
                    for_thread.end=1;
                //write(1,buffer,odp);
                
            }
    }
    //for_thread.end=1;
    pthread_cancel(tid);
    close(nClientSocket);
    close(nSocket);
    return(0);
}