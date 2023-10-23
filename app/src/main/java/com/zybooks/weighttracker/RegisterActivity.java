package com.zybooks.weighttracker;

import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import com.zybooks.weighttracker.model.User;
import com.zybooks.weighttracker.viewmodel.UserListViewModel;

public class RegisterActivity extends AppCompatActivity {
    UserListViewModel mUserListViewModel;

    EditText mUserEmail;
    EditText mUserPhone;
    EditText mUserPassword;
    EditText mConfirmPassword;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);
        mUserListViewModel = new UserListViewModel(getApplication());
    }

    public void onRegisterButtonClick(View view) {
        mUserEmail = findViewById(R.id.register_email_input);
        mUserPhone = findViewById(R.id.register_phone_input);
        mUserPassword = findViewById(R.id.register_password_input);
        mConfirmPassword = findViewById(R.id.register_confirm_password);

        if (mUserListViewModel.getUserEmail(mUserEmail.getText().toString()) == 0 &&
            mUserPassword.getText().toString().equals(mConfirmPassword.getText().toString())){

            User newUser = new User(mUserEmail.getText().toString(),  mUserPassword.getText().toString(), mUserPhone.getText().toString());

            mUserListViewModel.addUser(newUser);

            Toast.makeText(this, "Success! Please log-in with your new account info..", Toast.LENGTH_SHORT).show();

            Intent newUserIntent = new Intent(RegisterActivity.this, LoginActivity.class);
            startActivity(newUserIntent);

            //long userID = mUserListViewModel.getUserID(mUserEmail.getText().toString());
            /*
            SharedPreferences namedSharedPref = getSharedPreferences("myprefs", Context.MODE_PRIVATE);
            SharedPreferences.Editor editor = namedSharedPref.edit();

            editor.putString("Email:", mUserEmail.getText().toString());
            editor.putLong("ID", userID);
            editor.putBoolean("Logged in", true);
            editor.apply();
            */
        }else{
            Toast.makeText(this, "Email address already in use.", Toast.LENGTH_SHORT).show();
        }


    }
}
