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
    
    private static final int PORT = 1214;
    private static final String HOST = "192.168.1.18";
    public static void main(String[] args) throws IOException, ClassNotFoundException {
        // TODO code application logic here
        Socket clientSocket = null;
        clientSocket=new Socket(HOST, PORT);
        
        MyRunnable mr= new MyRunnable(clientSocket);
        Thread th= new Thread(mr);
        th.start();
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in)); 
        int n=4;
        while(true){
       
        String wync;
        wync=in.readLine();
     
    System.out.println(wync);
    PrintWriter writer = new PrintWriter(clientSocket.getOutputStream(), true);
    writer.println(wync);
        }
    }
}
