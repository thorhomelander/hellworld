import java.util.*;
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
public class FTPServer {
        public static void main(String args[]) throws Exception
        {
                int port = Integer.parseInt(args[0]);;
                ServerSocket soc = new ServerSocket(port);
                System.out.println("FT server started on port number "+port);
                while(true)
                {
                        System.out.println("Waiting for connection..");
                        transferfile t = new transferfile(soc.accept());
                }
        }
}
class transferfile extends Thread
{
        Socket ClientSoc;
        DataInputStream din;
        DataOutputStream dout;
        transferfile(Socket soc)
        {
                try
                {
                        ClientSoc = soc;
                        din = new DataInputStream(ClientSoc.getInputStream());
                        dout = new DataOutputStream(ClientSoc.getOutputStream());
                        System.out.println("FTP Client connected..");
                        start();

                }
                catch(Exception ex)
                {

                }
        }
        void SendFile() throws Exception
        {
                String filename = din.readUTF();
                File f = new File(filename);
                if(!f.exists())
                {
                        dout.writeUTF("File not found");
                        return;
                }
                else
                {
                        dout.writeUTF("Ready");
                        FileInputStream fin = new FileInputStream(f);
                        int ch;
                        do
                        {
                                ch = fin.read();
                                dout.writeUTF(String.valueOf(ch));
                        }
                        while(ch!=-1);
                        fin.close();
                        dout.writeUTF("File Received Successfully");

                }
        }
        void ReceiveFile() throws Exception

        {
                String filename = din.readUTF();
                String option;
                option="N";
                if(filename.compareTo("File not found")==0)
                {
                        return;
                }
                File f = new File(filename);

                if (f.exists())
                {
                        dout.writeUTF("File Already Exists");
                }
                else
                {
                        dout.writeUTF("SendFile");
                        option="Y";
                }
                if(option.compareTo("Y")==0)
                {
                        FileOutputStream fout=new FileOutputStream(f);
                        int ch;
                        String temp;
                        do
                        {
                                temp=din.readUTF();
                                ch=Integer.parseInt(temp);
                                if(ch!=-1)
                                {
                                        fout.write(ch);
                                }
                        }
                        while(ch!=-1);
                        fout.close();
                        dout.writeUTF("File send Successfully");
                }
                else
                {
                        return;
                }
        }
        public void run()
        {
                while(true)
                {
                        try
                        {
                                System.out.println("Waiting for Command ...");
                                String Command=din.readUTF();
                                if(Command.compareTo("GET")==0)
                                {
                                        System.out.println("\tGET Command Received...");
                                        SendFile();
                                        continue;
                                }
                                else if(Command.compareTo("SEND")==0)
                                {
                                        System.out.println("\tSEND Command Received ...");
                                        ReceiveFile();
                                        continue;
                                }
                                else if(Command.compareTo("DISCONNECT")==0)
                                {
                                        System.out.println("\tDisconnect command Received ...");
                                        System.exit(1);
                                }
                        }
                        catch(Exception ex)
                        {
                        }
                }
        }
}
