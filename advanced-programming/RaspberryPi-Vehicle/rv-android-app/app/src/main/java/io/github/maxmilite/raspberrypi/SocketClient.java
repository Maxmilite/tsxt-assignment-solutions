package io.github.maxmilite.raspberrypi;

import android.os.Looper;
import android.util.Log;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;

import java.io.*;
import java.net.Socket;

public class SocketClient {
    public static String serverAddress;
    public static int port;
    public static Socket client;
    public static BufferedReader bufferedReader;
    public static PrintWriter printWriter;
    public static void close() throws IOException {
        printWriter.println("exit");
        printWriter.flush();
        client.close();
    }

    public static String readMessage() throws IOException {
        String s = bufferedReader.readLine();
        s = s.replace("\0", "");
        Log.e("Message", s);
        return s;
//        return bufferedReader.readLine();
    }

    public static void sendMessage(String option, String message) throws IOException {
        printWriter.println(option + " " + message);
        printWriter.flush();
    }

    public static void setVehicleState(Vector4B cur) throws IOException {
        String s = "st";
        if (cur.up && !cur.down) s = "f";
        if (cur.down && !cur.up) s = "r";
        if (cur.left && !cur.right) s = "lf";
        if (cur.right && !cur.left) s = "rf";
        sendMessage("execute", s);
    }

    public static void asyncConnect(AppCompatActivity activity, String address, Runnable cb, Runnable fail) {
        new Thread(() -> {
            Looper.prepare();
            boolean result = connect(activity, address);
            if (result) cb.run();
            else fail.run();
        }).start();
    }

    public static boolean connect(AppCompatActivity activity, String address) {
        serverAddress = address;
        port = 8887;
        boolean connected = false;
        while (!connected) {
            try {
                client = new Socket(serverAddress, port);
                bufferedReader = new BufferedReader(new InputStreamReader(client.getInputStream()));
                printWriter = new PrintWriter(client.getOutputStream());
                connected = true;
            } catch (IOException e) {
                return false;
            }
        }

        return true;
    }
}
