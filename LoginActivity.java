package com.zybooks.weighttracker;

import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.zybooks.weighttracker.model.AuthenticatedUser;
import com.zybooks.weighttracker.viewmodel.UserListViewModel;

import androidx.appcompat.app.AppCompatActivity;

public class LoginActivity extends AppCompatActivity {
    private UserListViewModel mUserListViewModel;
    EditText mUserEmail;
    EditText mUserPassword;
    Button mLoginButton;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        mUserListViewModel = new UserListViewModel(getApplication());

        mLoginButton = findViewById(R.id.login_button);

    }

    public void onLoginButton(View view) {
        mUserEmail = findViewById(R.id.email_input);
        mUserPassword = findViewById(R.id.password_input);

            if (mUserListViewModel.getUserEmail(mUserEmail.getText().toString()) != 0 &&
                    mUserListViewModel.getUserPass(mUserPassword.getText().toString()) != 0) {

                //long userID = mUserListViewModel.getUserID(mUserEmail.getText().toString());
                //AuthenticatedUser authenticatedUser = new AuthenticatedUser(mUserEmail.getText().toString(), userID, true);

                Intent loggedInUser = new Intent(LoginActivity.this, LogsActivity.class);
                loggedInUser.putExtra("email", mUserEmail.getText().toString());
                startActivity(loggedInUser);

            } else {
                Toast.makeText(this, "Login info not found.", Toast.LENGTH_SHORT).show();
            }
    }

    public void onSignUpButton(View view){
        Intent registration = new Intent(LoginActivity.this, RegisterActivity.class);
        startActivity(registration);
    }
}
