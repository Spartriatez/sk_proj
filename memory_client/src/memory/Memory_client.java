/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package memory;
import java.io.*;
import java.net.*;
import memory.MyRunnable;


/**
 *
 * @author piotr
 */
public class Memory_client {

    /**
     * @param args the command line arguments
     */
   
    private static final int PORT = 1235;
    private static final String HOST = "150.254.141.136";
    public static void main(String[] args) throws InterruptedException,IOException, ClassNotFoundException {
        // TODO code application logic here
        Socket clientSocket = null;
        clientSocket=new Socket(HOST, PORT);
       
        MyRunnable mr= new MyRunnable(clientSocket);
        Thread th= new Thread(mr);
        th.start();
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in)); 
        int[] tablica=new int [4];
        while(true)
        {
            String[] wync=new String[2];
            wync[0]=in.readLine();
            wync[1]=in.readLine();
            //System.out.println(wync);
            tablica=mr.z_klienta_do_serwera(wync);
            for(int i=0;i<4;i++)
                System.out.print(tablica[i]+" ");
            
              String v;
        String end=null;
        for(int i=0;i<4;i++)
        {
             v=Integer.toString(tablica[i]);
          if(i==0)
              end=v+" ";
         else if(i==3)
              end+=v;
            else 
              end+=v+" ";
        }
      
            System.out.println("end "+end);
    PrintWriter writer = new PrintWriter(clientSocket.getOutputStream(), true);
    writer.println(end);
        }
    }
}
