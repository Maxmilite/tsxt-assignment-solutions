package io.github.maxmilite.raspberrypi.ui.gallery;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;
import io.github.maxmilite.raspberrypi.R;
import io.github.maxmilite.raspberrypi.SocketClient;
import io.github.maxmilite.raspberrypi.databinding.FragmentGalleryBinding;

import java.io.IOException;

public class GalleryFragment extends Fragment {

    private GalleryViewModel galleryViewModel;
    private FragmentGalleryBinding binding;

    public View onCreateView(@NonNull LayoutInflater inflater,
            ViewGroup container, Bundle savedInstanceState) {
        galleryViewModel =
                new ViewModelProvider(this).get(GalleryViewModel.class);

        binding = FragmentGalleryBinding.inflate(inflater, container, false);
        View root = binding.getRoot();
        RadioGroup group = root.findViewById(R.id.radio_group);
        group.setOnCheckedChangeListener((radioGroup, i) -> {
            int id = group.getCheckedRadioButtonId();
            RadioButton button = root.findViewById(id);
            int val = group.indexOfChild(button) - 1;
            try {
                SocketClient.sendMessage("switch", "music " + val);
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