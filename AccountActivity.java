package com.zybooks.weighttracker;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.MenuItem;
import android.view.View;

import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.zybooks.weighttracker.viewmodel.UserListViewModel;

public class AccountActivity extends AppCompatActivity {
    UserListViewModel mUserListViewModel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_account);
        mUserListViewModel = new UserListViewModel(getApplication());

        BottomNavigationView bottomNavigationView = findViewById(R.id.bottom_navigation);

        // Set selected
        bottomNavigationView.setSelectedItemId(R.id.activity_account);

        bottomNavigationView.setOnNavigationItemSelectedListener(new BottomNavigationView.OnNavigationItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {

                if(item.getItemId() == R.id.nav_account){
                    return true;
                }else{
                    Intent LogsIntent = new Intent(AccountActivity.this, LogsActivity.class);
                    startActivity(LogsIntent);
                    finish();
                }
                return false;
            }
        });
    }
    public void onLogoutClick(View view){
        Intent logout = new Intent(AccountActivity.this, LoginActivity.class);
        startActivity(logout);
    }

    public void enableDarkMode(View view){

    }
}