package cc.ioctl.nfcncihost.activity;

import android.content.res.TypedArray;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowInsets;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

/**
 * Generic-purpose magic for Activity here.
 */
public class AppActivity extends AppCompatActivity {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Window window = getWindow();
        View decorView = window.getDecorView();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            decorView.post(() -> {
                WindowInsets insets = decorView.getRootWindowInsets();
                int h = 0;
                if (insets != null) {
                    h = insets.getSystemWindowInsetBottom();
                }
                if (h >= dip2px(40)) {
                    TypedArray a = obtainStyledAttributes(new int[]{android.R.attr.navigationBarColor});
                    window.setNavigationBarColor((a.getColor(0, 0) & 0x00ffffff) | 0x20000000);
                    a.recycle();
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
                        window.setNavigationBarContrastEnforced(false);
                    }
                } else {
                    window.setNavigationBarColor(Color.TRANSPARENT);
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
                        window.setNavigationBarContrastEnforced(true);
                    }
                }
            });
        }
    }

    public final int dip2px(float dpValue) {
        float scale = getResources().getDisplayMetrics().density;
        return (int) (dpValue * scale + 0.5f);
    }

    public final int dip2sp(float dpValue) {
        float scale = getResources().getDisplayMetrics().density /
                getResources().getDisplayMetrics().scaledDensity;
        return (int) (dpValue * scale + 0.5f);
    }

    public final int px2sp(float pxValue) {
        float fontScale = getResources().getDisplayMetrics().scaledDensity;
        return (int) (pxValue / fontScale + 0.5f);
    }
}
