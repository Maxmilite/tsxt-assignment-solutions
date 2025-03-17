package io.github.maxmilite.raspberrypi.ui.slideshow;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;
import io.github.maxmilite.raspberrypi.R;
import io.github.maxmilite.raspberrypi.SocketClient;
import io.github.maxmilite.raspberrypi.databinding.FragmentSlideshowBinding;

import java.io.IOException;

public class SlideshowFragment extends Fragment {

    private SlideshowViewModel slideshowViewModel;
    private FragmentSlideshowBinding binding;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        slideshowViewModel =
                new ViewModelProvider(this).get(SlideshowViewModel.class);

        binding = FragmentSlideshowBinding.inflate(inflater, container, false);
        View root = binding.getRoot();
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
        Switch LTSwitch = root.findViewById(R.id.switch_lt);
        Switch USSwitch = root.findViewById(R.id.switch_us);
        Switch CISwitch = root.findViewById(R.id.switch_ci);
        int res;
        try {
            SocketClient.sendMessage("query", "line_tracing");
            res = Integer.parseInt(SocketClient.readMessage());
            if (res == 1) LTSwitch.setChecked(true);
            Thread.sleep(100);
            SocketClient.sendMessage("query", "ultra_sonic");
            res = Integer.parseInt(SocketClient.readMessage());
            if (res == 1) USSwitch.setChecked(true);
            Thread.sleep(100);
            SocketClient.sendMessage("query", "color_identification");
            res = Integer.parseInt(SocketClient.readMessage());
            if (res == 1) CISwitch.setChecked(true);
            Thread.sleep(100);
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        LTSwitch.setOnCheckedChangeListener((compoundButton, b) -> {
            try {
                SocketClient.sendMessage("switch", "line_tracing " + (b ? 1 : 0));
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
        USSwitch.setOnCheckedChangeListener((compoundButton, b) -> {
            try {
                SocketClient.sendMessage("switch", "ultra_sonic " + (b ? 1 : 0));
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
        CISwitch.setOnCheckedChangeListener((compoundButton, b) -> {
            try {
                SocketClient.sendMessage("switch", "color_identification " + (b ? 1 : 0));
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}