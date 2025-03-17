package io.github.maxmilite.raspberrypi.ui.home;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.SeekBar;
import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import io.github.maxmilite.raspberrypi.R;
import io.github.maxmilite.raspberrypi.SocketClient;
import io.github.maxmilite.raspberrypi.Vector4B;
import io.github.maxmilite.raspberrypi.databinding.FragmentHomeBinding;

import java.io.IOException;

public class HomeFragment extends Fragment {

    private FragmentHomeBinding binding;

    public static Vector4B state = new Vector4B();

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }

    public static class EventHandler implements View.OnTouchListener {

        @SuppressLint({"NonConstantResourceId", "ClickableViewAccessibility"})
        @Override
        public boolean onTouch(View v, MotionEvent event) {
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

    @SuppressLint("SetJavaScriptEnabled")
    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        HomeViewModel homeViewModel = new ViewModelProvider(this).get(HomeViewModel.class);

        binding = FragmentHomeBinding.inflate(inflater, container, false);
        View root = binding.getRoot();
        root.findViewById(R.id.forward).setOnTouchListener(new HomeFragment.EventHandler());
        root.findViewById(R.id.backward).setOnTouchListener(new HomeFragment.EventHandler());
        root.findViewById(R.id.left).setOnTouchListener(new HomeFragment.EventHandler());
        root.findViewById(R.id.right).setOnTouchListener(new HomeFragment.EventHandler());
        WebView webView = root.findViewById(R.id.web_view);
        webView.getSettings().setJavaScriptEnabled(true);
        webView.setWebViewClient(new WebViewClient());
        webView.loadUrl(String.format("http://%s:8081", SocketClient.serverAddress));
        WebSettings webSettings = webView.getSettings();
        webSettings.setJavaScriptEnabled(true);
        webSettings.setUseWideViewPort(true);
        webSettings.setLoadWithOverviewMode(true);
        SeekBar speedBar = root.findViewById(R.id.speedBar);
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
        return root;
    }
}