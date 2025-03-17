package io.github.maxmilite.raspberrypi;

import android.content.Intent;
import android.os.StrictMode;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

public class LoginActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);
    }

    public void connect(View view) {
        EditText editText = findViewById(R.id.address);
        Toast.makeText(this, "Connecting to server at " + editText.getText().toString() + ".", Toast.LENGTH_LONG).show();
        SocketClient.asyncConnect(this, editText.getText().toString(), () -> {
            Toast.makeText(this, "Connection Established.", Toast.LENGTH_LONG).show();
            Intent intent = new Intent();
            intent.setClass(this, TestActivity.class);
            startActivity(intent);
        }, () -> {
            Toast.makeText(this, "Connection failed.", Toast.LENGTH_LONG).show();
        });

    }
}