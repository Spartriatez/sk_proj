/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package memory;
import java.io.*;
import java.net.*;
import java.util.logging.Level;
import java.util.logging.Logger;
/*
 *
 * @author piotr
 */

public class MyRunnable implements Runnable {
    
    private Socket sock;
    public InputStream is,is2;
    public byte[] buffer,buffer2;
    public int[][] tabl;
    public int[] wyn;
    public int value;
    public int[] calosc;
    MyRunnable(Socket socket)
    {
        this.sock=socket;
        try
        {
            is = sock.getInputStream();
            is2 = sock.getInputStream();
        }catch(IOException ioE){ioE.printStackTrace();}
    }
      
    public void zmiana()
    {
        tabl[wyn[0]][wyn[1]]=-1;
        tabl[wyn[2]][wyn[3]]=-1;
    }
    
   public void pokaz_macierz()
   {
       for(int i=0;i<value;i++)
       {
           for(int j=0;j<value;j++)
               System.out.print(tabl[i][j]+" ");
           System.out.println();
       }
   }
     public void podmien(int zgoda)throws InterruptedException
    {
        switch(zgoda)
         {
            case 1:{ 
                    tabl[wyn[0]][wyn[1]]=wyn[4];
                    tabl[wyn[2]][wyn[3]]=wyn[5];
                    pokaz_macierz();
                    Thread.sleep(10000);
                    zmiana();
                    System.out.println();
                    pokaz_macierz();
                    break;
                    }
            case 2:{ int temp1,temp2;
                    temp1=tabl[wyn[0]][wyn[1]];
                    temp2=tabl[wyn[2]][wyn[3]];
                    tabl[wyn[0]][wyn[1]]=wyn[4];
                    tabl[wyn[2]][wyn[3]]=wyn[5];
                    pokaz_macierz();
                    System.out.println();
                    Thread.sleep(10000); 
                    tabl[wyn[0]][wyn[1]]=temp1;
                    tabl[wyn[2]][wyn[3]]=temp2;
                     pokaz_macierz();
                     break;
                    }
        }
    }
   
    int[][] wykonaj_tabele(int n)
    {
       int[][] tab=new int[n][n];
       int iter=1;
       for(int i=0;i<n;i++)
       {
           for(int j=0;j<n;j++)
               tab[i][j]=iter++;
       }    
       return tab;
    }
     
    public int[] z_klienta_do_serwera(String[] wart)
    {
       int[] tmp=new int [2];
      
       for(int i=0;i<2;i++)
           tmp[i]=Integer.parseInt(wart[i]);
        calosc = new int[4];
        int temp=0;
        int temp2=0;
       
       for(int i=0;i<value;i++)
        {
            for(int j=0;j<value;j++)
            {
               if(temp<2){
              //  System.out.println("temp "+temp);
                if(tabl[i][j]==tmp[temp])
                {
                    calosc[temp2++]=i;
                    calosc[temp2++]=j;
                    i=0;
                    j=0;
                    temp++;
                }           
               }else
               {
                   i=value;
                   j=value;
               }
            }
        }     
    /* for(int q=0;q<4;q++)
           System.out.println(calosc[q]);*/
       return calosc;
    }
    
    @Override
    public void run() {
       try{
           buffer = new byte[10000];
           buffer2 = new byte[10000];
                             
                int v=is.read(buffer,0,4);
                System.out.println(v);
                
                value=(int)buffer[0];
                while(value<2)
                {
                    System.out.println("Poczekaj na kolejnego przeciwnika");
                     v=is.read(buffer,0,4);
                     value=(int)buffer[0];
                }
                tabl=new int[value][value];
              
                 tabl=wykonaj_tabele(value);
                pokaz_macierz();         
                System.out.println();
            wyn=new int[6];
            int zgoda=9;
                while(true)
                {
                   int v2=is2.read(buffer2);
                                    
                     int q=0;
                      while((int)buffer2[q]==0)
                             q++;
                      
                     // System.out.println("q" +q);
                       zgoda=(int)buffer2[0];
                       if(zgoda<3){
                       q=4;
                        //System.out.println("q "+q);
                      for(int i=0;i<6;i++)
                      {
                          if(i<4)
                             wyn[i]=(int)buffer2[i*4+q]-1;
                          else
                             wyn[i]=(int)buffer2[i*4+q]; 
                      }
                      
                         for(int i=0;i<6;i++)
                              System.out.print(wyn[i]+" ");  
                         System.out.println();
                       }
                          switch(zgoda)   
                          {   
                          case 1:  {
                                    
                                    podmien(zgoda);
                                     zgoda=0;
                                     break;
                                    }
                          case 2: {

                                    podmien(zgoda);
                                     zgoda=0;
                                     break;
                                   }
                          case 3: {
                              System.out.println("Przerwano polaczenie z drugim uzykownikem.Wygrales!!!");
                              break;
                          }
                        
                          }
                }
        }catch(IOException ioE){ioE.printStackTrace();} catch (InterruptedException ex) {
            Logger.getLogger(MyRunnable.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
   
  
}
