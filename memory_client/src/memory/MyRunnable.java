/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package memory;
import java.io.*;
import java.net.*;
import java.nio.ByteBuffer;
/*
 *
 * @author piotr
 */
public class MyRunnable implements Runnable {
    private Socket sock;
    public InputStream is,is2;
    byte[] buffer,buffer2;
    int[][] tabl;
    MyRunnable(Socket socket)
    {
        this.sock=socket;
        try
        {
            is = sock.getInputStream();
            is2 = sock.getInputStream();
        }catch(IOException ioE){ioE.printStackTrace();}
    }
    @Override
    public void run() {
       try{
           buffer = new byte[10000];
           buffer2 = new byte[10000];
            while(true){
            
                int v2=is.read(buffer,0,4);
                System.out.println(v2);
                
                int value=(int)buffer[0];
                //System.out.println(value);
                
                int v3=is2.read(buffer2);
                //System.out.println(v3);
                 int q=0;
                 while((int)buffer2[q]==0)
                     q++;
                 //System.out.println("q "+q);
                tabl=new int[value][value];
                int tmp1=0,tmp2=0;
                if(v3>0 && v2 > 3){
                    for(int i=0;i<v3;i++)
                    {   
                        //System.out.println((int)buffer2[i]);
                        tabl[tmp1][tmp2]=(int)buffer2[i*4+q]; 
                        if(tmp2==value-1)
                        {
                            tmp2=0;
                            tmp1++;
                            //tabl[tmp1][tmp2]=(int)buffer2[i*4];
                        }else if(tmp1<value)
                        {
                            tmp2++;
                        }
                                               
                        if(tmp1==value)break;         
                    }     
                    for(int i=0;i<value;i++)
                    {
                        for(int j=0;j<value;j++)
                            System.out.print(tabl[i][j]+" ");
                        System.out.println();
                    }
                }
            }
        }catch(IOException ioE){ioE.printStackTrace();}
        }
}
