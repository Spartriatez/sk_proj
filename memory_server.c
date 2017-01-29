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

#define SERVER_PORT 2002
#define QUEUE_SIZE 5        
#define BUFSIZE 100
#define N 8
#define MaxUser 10
struct TThreadData{
    int *secSocket;
    int **tabl;
    int *active;
    int *wiadomosc;
    pthread_t tid[2];
};

struct TThreadData for_thread;
int x;

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
        if(tb[i]>N){
            sum+=1;
        }
        else 
            sum+=0;
    }
   if(sum==0)
   {    
       for(i=0;i<4;i++)
          tab[i]=tb[i];
       return 2;
   }else 
       return 1;
}

int sprawdz_poprawnosc(int *tb,int *wart)
{
    if(for_thread.tabl[tb[0]][tb[1]]==for_thread.tabl[tb[2]][tb[3]] &&( tb[0]!=tb[2] && tb[1]!=tb[3]))
    {   wart[0]=for_thread.tabl[tb[0]][tb[1]];
        wart[1]=for_thread.tabl[tb[2]][tb[3]];
        for_thread.tabl[tb[0]][tb[1]]=-1;
        for_thread.tabl[tb[2]][tb[3]]=-1;
        return 2;
    }
    else
    {
        wart[0]=for_thread.tabl[tb[0]][tb[1]];
        wart[1]=for_thread.tabl[tb[2]][tb[3]];
        return 1;
    }
}

void pokaz_matrix()
{
    int i,j;
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
            printf("%d ",for_thread.tabl[i][j]);
        printf("\n");
    }
}

int *scal_wiadomosc(int komunikat,int* wspolrzedne,int *wartosci)
{
    int *wiad2;
    wiad2=(int *)malloc((N-1)*sizeof(int));
    int tmp=0;
    int i;
    for(i=0;i<N-1;i++)
    {
        if(i==0)
            wiad2[i]=komunikat;
        else if(i>0 && i<5)
            wiad2[i]=wspolrzedne[tmp++]+1;
        else
        {
            wiad2[i]=wartosci[i-1-tmp];
        }
    }
    return wiad2;
}
socklen_t nTmp;
struct sockaddr_in stAddr;

short end;

int *tb;

int wiad;

void* readingThread(void* data){
    int i;
   int mSocket=*(int *)data;
    int spr;
    int spr2;
    char *buf;
    int *wart;
    wart=(int*)malloc(2*sizeof(int));
   int x;
    while(1) {
    buf=(char *)malloc(BUFSIZE*sizeof(char));
	bzero(buf,BUFSIZE);

	x=read(mSocket, buf, BUFSIZE);
    if(x<= 0 )
    {
        int x=0;
        getpeername(mSocket , (struct sockaddr*)&stAddr, (socklen_t*)&nTmp); 
        printf("Host disconnected  port %d \n" , mSocket);
        for(i=0;i<2;i++){
            
                      if(mSocket== for_thread.secSocket[i])
                      {
                    for_thread.secSocket[i] = 0;
                    for_thread.active[i]=0;
                      break;
                      }
        }
        for(i=0;i<2;i++){
            if(for_thread.active[i]==1){
                        x=3;
                          if(write(for_thread.secSocket[i],&x,sizeof(int)))
                            { //perror("error");
                                printf("for_thread %d\n",end);
                            }
                    }
        }
          close( mSocket );
          free(buf);
         break;
    }
	else if(x > 0){
        printf("xxxxxxxxxxxxxxxx %d\n",x);
    /* for(i=0;i<q;i++){
            if(for_thread.active[i]==1)
            {      */          
                spr=convert(buf,tb);

                if(spr==2)
                {
                    for(i=0;i<4;i++){
                        tb[i]=tb[i]-1;
                    }
                    spr2 =sprawdz_poprawnosc(tb,wart);
                }
                else
                  end=9;
                printf("spr2 %d\n",spr2);
                if(spr2==2)
                     wiad=end=1;
                else
                    wiad= end=2;
                
            //---------------------------------------------
       
        for_thread.wiadomosc=scal_wiadomosc(wiad,tb,wart);
        for(i=0;i<N-1;i++)
            printf("%d ",for_thread.wiadomosc[i]);
        printf("\n");
        if(end>0 && end<3)
        {
           for(i=0;i<2;i++){
             
             if((write(for_thread.secSocket[i],for_thread.wiadomosc,(N-1)*sizeof(int))<0) && (for_thread.active[i]==1))
              { //perror("error");
                  printf("for_thread %d\n",end);
              }
           }
            pokaz_matrix();
            end=9;
        }
        
        free(buf);
    	}
    
        //----------------------------------
      
    }
   return NULL;
  /* int r=1;
printf("koniec\n");
pthread_exit(&r);*/
  // pthread_exit((void*) 0);
}

int main(int argc, char* argv[]){
    
    /*----------------------------------*/
   int **tab;
    int i,j;
    int kafelek=N*N/2;
    x=N;
    int q=0;
    for_thread.wiadomosc=(int *)malloc((N-1)*sizeof(int));
   tab=(int**)malloc(N*sizeof(int*));
    for(i=0;i<N;i++)
        tab[i]=(int*)malloc(N*sizeof(int));
     tb=(int*)malloc(4*sizeof(int));
  
    for_thread.secSocket=(int*)malloc(MaxUser*sizeof(int));
    for(i=0;i<MaxUser;i++)
        for_thread.secSocket[i]=0;
   // for_thread.ktory=(int*)malloc(MaxUser*sizeof(int));
    for_thread.active=(int*)malloc(MaxUser*sizeof(int));
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            tab[i][j]=0;
    generuj(tab,kafelek,N);
    for_thread.tabl=alloc_2d_int(N,N);
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++){
              for_thread.tabl[i][j]=tab[i][j];
             // printf("%d ",tab[i][j]);
        }
    }
    
   for(i=0;i<N;i++)
    {
        free(tab[i]);
    }
    free(tab);
   
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++){
              printf("%d ",for_thread.tabl[i][j]);
        }
        printf("\n");
    }
     printf("\n");
   // printf("matrix %d\n",tabl[1][1]);
    /*----------------------------------*/
    int nSocket;
    int nBind, nListen;
    int nFoo = 1;
   

    /* address structure */
    memset(&stAddr, 0, sizeof(struct sockaddr));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(SERVER_PORT);
 
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
    
    /* block for connection request */
  
    
   
    
    while(1){
    nTmp = sizeof(struct sockaddr);
    for_thread.secSocket[q]= accept(nSocket, (struct sockaddr*)&stAddr, &nTmp);
    if(for_thread.secSocket[q]<0)
    {
        printf("error \n");
    }else{
   // for_thread[q].ktory=q;
    for_thread.active[q]=1;
    
   // printf("Ktory %d\n",for_thread.ktory[q]);
    end=0;
    int create_result = 0;
   
     int mySocket= for_thread.secSocket[q];
    printf("%s: [connection from %s]\n",
                  argv[0], inet_ntoa((struct in_addr)stAddr.sin_addr));
    
    create_result=pthread_create(&for_thread.tid[q], NULL, readingThread,&for_thread.secSocket[q]);
   
     if (create_result){
       printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
       exit(-1);
       }
    
    printf("end %d\n",end);
    if(q==1){
        for(i=0;i<=q;i++)
            write(for_thread.secSocket[i],&x,sizeof(x));
          
            end=9;
           q=0;
    } else
    {
        int l=1;
         write(for_thread.secSocket[q],&l,sizeof(l));
    }
            
    
   q++;
    }
   }
      /*for(i =0;i<q;i++)
        if(for_thread.active[i]==0)
            pthread_cancel(tid[i]);*/
    //close(nClientSocket);
    close(nSocket);
  return(0);
}
