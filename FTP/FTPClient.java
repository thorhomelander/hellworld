//import java.util.*;
import java .io.*;
import java.net.Socket;

class FTPClient {

        public static void main(String arg[]) throws Exception
        {
                int port = Integer.parseInt(arg[0]);

                Socket soc=new Socket("127.0.0.1" ,port);
                transferfileClient t=new  transferfileClient(soc);

                t.displayMenu(soc);

        }
}
class transferfileClient

{

        Socket ClientSoc;
        DataInputStream din;
        DataOutputStream dout;
        BufferedReader br;
        transferfileClient(Socket soc)
        {
                try
                {
                        ClientSoc=soc;
                        din=new DataInputStream(ClientSoc.getInputStream());
                        dout=new DataOutputStream(ClientSoc.getOutputStream());
                        br=new BufferedReader(new InputStreamReader(System.in));

                }
                catch(Exception ex)
                {
                }
        }
        void SendFile() throws Exception
        {
                String filename;
                System.out.print("Enter File Name:");
                filename=br.readLine();
                File f=new File(filename);
                if(!f.exists())
                {
                        System.out.println("file not Exists...");
                        dout.writeUTF("File not found");
                        return;
                }
                dout.writeUTF(filename);
                String msgFromServer=din.readUTF();
                if(msgFromServer.compareTo("File Already Exists")==0)
                {
                        String Option;
                        System.out.println("file Already Exists.want to Overrite(y/n)?");
                        Option=br.readLine();
                        if(Option=="y")
                        {
                                dout.writeUTF("Y");

                        }
                        else{dout.writeUTF("N");
                                return;
                        }
                }
                System.out.print("Sending file...");
                FileInputStream fin=new FileInputStream(f);
                int ch;
                do
                {
                        ch=fin.read();
                        dout.writeUTF(String.valueOf(ch));

                }
                while(ch!=-1);
                fin.close();
                System.out.println(din.readUTF());


        }
        void ReceiveFile() throws Exception
        {

                String fileName;
                System.out.print("Enter the name :");
                fileName=br.readLine();
                dout.writeUTF(fileName);
                String msgFromServer=din.readUTF();
                if(msgFromServer.compareTo("File not Found")==0)
                {
                        System.out.println("File not found on Server ...");
                        return;
                }
                else if(msgFromServer.compareTo("Ready")==0)
                {
                        System.out.println("Receiving File ...");
                        File f=new File(fileName);
                        if(f.exists())
                        {
                                String Option;
                                System.out.println("File Already Exists. Want to Overwrite(Y/N) ? ");
                                Option= br.readLine();
                                if(Option=="N")
                                {
                                        dout.flush();
                                        return;
                                }
                        }
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
                        System.out.println(din.readUTF());
                }
        }
        public void displayMenu(Socket soc) throws Exception
        {


                while(true)
                {
                        System.out.println("Choose the operation: ");
                        System.out.println("1.Send File");
                        System.out.println("2.Receive File");
                        System.out.println("3.Exit");
                        System.out.println("\n Enter your choice of operation: ");
                        int choice;
                        choice=Integer.parseInt(br.readLine());
                        if(choice==1)
                        {
                                dout.writeUTF("SEND");
                                SendFile();
                        }
                        else if(choice==2)
                        {
                                dout.writeUTF("GET");
                                ReceiveFile();
                        }
                        else if(choice==3)
                        {
                                dout.writeUTF("DISCONNECT");
                                System.exit(1);
                        }

                }
        }
}
