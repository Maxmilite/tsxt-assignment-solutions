package io.github.maxmilite.raspberrypi;

import android.annotation.SuppressLint;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.SeekBar;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    public static Vector4B state = new Vector4B();

    @SuppressLint({"ClickableViewAccessibility", "SetJavaScriptEnabled"})
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.forward).setOnTouchListener(new EventHandler(this));
        findViewById(R.id.backward).setOnTouchListener(new EventHandler(this));
        findViewById(R.id.left).setOnTouchListener(new EventHandler(this));
        findViewById(R.id.right).setOnTouchListener(new EventHandler(this));
        WebView webView = findViewById(R.id.web_view);
        webView.getSettings().setJavaScriptEnabled(true);
        webView.setWebViewClient(new WebViewClient());
        webView.loadUrl(String.format("http://%s:8081", SocketClient.serverAddress));
        WebSettings webSettings = webView.getSettings();
        webSettings.setJavaScriptEnabled(true);
        webSettings.setUseWideViewPort(true);
        webSettings.setLoadWithOverviewMode(true);
        SeekBar speedBar = findViewById(R.id.speedBar);
        speedBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                try {
                    SocketClient.sendMessage("execute", "speed " + i + " 0");
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
//        Log.e("Test", webView.getUrl());
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        try {
            SocketClient.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static class EventHandler implements View.OnTouchListener {
        public AppCompatActivity app;

        public EventHandler(AppCompatActivity app) {
            this.app = app;
        }

        @SuppressLint({"NonConstantResourceId", "ClickableViewAccessibility"})
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            Log.e("Test", MotionEvent.actionToString(event.getAction()));
            if (event.getAction() == MotionEvent.ACTION_MOVE)
                return false;
            boolean res = event.getAction() == MotionEvent.ACTION_DOWN;
            switch (v.getId()) {
                case R.id.forward:
                    state.up = res;
                    break;
                case R.id.backward:
                    state.down = res;
                    break;
                case R.id.left:
                    state.left = res;
                    break;
                case R.id.right:
                    state.right = res;
                    break;
                default:
                    break;
            }
            try {
                SocketClient.setVehicleState(state);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            return false;
        }


    }
}