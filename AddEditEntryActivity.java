package com.zybooks.weighttracker;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.EditText;
import android.widget.Toast;

public class AddEditEntryActivity extends AppCompatActivity {
    public static final String EXTRA_ID = "com.zybooks.weighttracker.EXTRA_ID";
    public static final String EXTRA_DATE = "com.zybooks.weighttracker.EXTRA_DATE";
    public static final String EXTRA_WEIGHT = "com.zybooks.weighttracker.EXTRA_WEIGHT";
    private EditText mUserDate;
    private EditText mUserWeight;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_entry);

        mUserDate = findViewById(R.id.add_date);
        mUserWeight = findViewById(R.id.add_weight);

        getSupportActionBar().setHomeAsUpIndicator(R.drawable.baseline_close_24);

        Intent intent = getIntent();

        if(intent.hasExtra(EXTRA_ID)){
            setTitle("Edit entry");
            mUserDate.setText(intent.getStringExtra(EXTRA_DATE));
            mUserWeight.setText(intent.getStringExtra(EXTRA_WEIGHT));
        }else{
            setTitle("Add entry");
        }
    }
    private void saveEntry(){
        String date = mUserDate.getText().toString();
        String weight = mUserWeight.getText().toString();

        if(date.trim().isEmpty() || weight.trim().isEmpty()){
            Toast.makeText(this, "Please enter both fields.", Toast.LENGTH_SHORT).show();
            return;
        }
        Intent data = new Intent();
        data.putExtra(EXTRA_DATE, date);
        data.putExtra(EXTRA_WEIGHT, weight);

        long id = getIntent().getIntExtra(EXTRA_ID,-1);
        if(id != -1){
            data.putExtra(EXTRA_ID, id);
        }
        setResult(RESULT_OK, data);
        finish();
    }

    //@Override
    public boolean onCreateOptionsMenu(Menu menu){
        MenuInflater menuInflater = getMenuInflater();
        menuInflater.inflate(R.menu.add_edit_entry_menu, menu);
        return super.onCreateOptionsMenu(menu);
    }

    public boolean onOptionsItemSelected(MenuItem item){
        if(item.getItemId() == R.id.save_entry){
            saveEntry();
        }
        return true;
    }
}