package sdu.revolution.client.network;

import sdu.revolution.client.Main;
import sdu.revolution.client.engine.main.Utils;

import java.io.*;
import java.net.Socket;

public class RevolutionClient {
    public String serverAddress, port;
    public Socket client;
    public DataOutputStream outputStream;
    public DataInputStream inputStream;
    public RevolutionClient() throws InterruptedException {
        serverAddress = Utils.getConfig("server-address");
        port = Utils.getConfig("server-port");
        boolean connected = false;
        while (!connected) {
            try {
                Main.Logger.info("Connecting to server at " + serverAddress + ":" + port + ".");
                client = new Socket(serverAddress, Integer.parseInt(port));
                Main.Logger.info("Connection Established.");
                outputStream = new DataOutputStream(client.getOutputStream());
                inputStream = new DataInputStream(client.getInputStream());
                connected = true;
            } catch (IOException e) {
                Main.Logger.info("Connection failed, reconnecting.");
                Thread.sleep(5000);
            }
        }
    }

    public boolean registerOnline(String name) throws IOException {
        outputStream.writeUTF("register " + name);
        return inputStream.readUTF().equals("success");
    }

    public void close() throws IOException {
        outputStream.writeUTF("goodbye");
        client.close();
    }

    public String sendMessage(String message) throws IOException {
        outputStream.writeUTF(message);
        return inputStream.readUTF();
    }
}
