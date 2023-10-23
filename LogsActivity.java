package com.zybooks.weighttracker;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProvider;
import androidx.recyclerview.widget.ItemTouchHelper;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import com.google.android.material.bottomnavigation.BottomNavigationView;

import android.content.Intent;
import android.os.Bundle;
import android.os.PersistableBundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.zybooks.weighttracker.model.Entry;
import com.zybooks.weighttracker.viewmodel.EntryListViewModel;
import com.zybooks.weighttracker.viewmodel.UserListViewModel;

import java.util.List;

public class LogsActivity extends AppCompatActivity {
    private EntryListViewModel mEntryListViewModel;
    private UserListViewModel mUserListViewModel;
    private
    String userEmail;
    long userID;
    public static final int ADD_ENTRY_REQUEST = 1;
    public static final int EDIT_ENTRY_REQUEST = 2;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_logs);

        BottomNavigationView bottomNavigationView = findViewById(R.id.bottom_navigation);

        // Set selected
        bottomNavigationView.setSelectedItemId(R.id.activity_logs);

        bottomNavigationView.setOnNavigationItemSelectedListener(new BottomNavigationView.OnNavigationItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {

                if(item.getItemId() == R.id.nav_logs){
                    return true;
                }else{
                    Intent accountIntent = new Intent(LogsActivity.this, AccountActivity.class);
                    startActivity(accountIntent);
                    finish();
                }
                return false;
            }
        });


        mEntryListViewModel =  new EntryListViewModel(getApplication());
        mUserListViewModel = new UserListViewModel(getApplication());

        userEmail = getIntent().getStringExtra("email");
        userID =  mUserListViewModel.getUserID(userEmail);

        FloatingActionButton addEntryButton = findViewById(R.id.FAB);
        addEntryButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(LogsActivity.this, AddEditEntryActivity.class);
                startActivityForResult(intent, ADD_ENTRY_REQUEST);
            }
        });

        RecyclerView mRecyclerView = findViewById(R.id.weight_entries_list);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));

        EntryAdapter adapter = new EntryAdapter();
        mRecyclerView.setAdapter(adapter);

        mEntryListViewModel = new ViewModelProvider(this).get(EntryListViewModel.class);

        mEntryListViewModel.getEntries(userID).observe(this, new Observer<List<Entry>>() {
            @Override
            public void onChanged(List<Entry> entries) {
                    adapter.setEntries(entries);
                }
        });

        new ItemTouchHelper(new ItemTouchHelper.SimpleCallback(0,
                ItemTouchHelper.LEFT){

            @Override
            public boolean onMove(@NonNull RecyclerView recyclerView, @NonNull RecyclerView.ViewHolder viewHolder, @NonNull RecyclerView.ViewHolder target) {
                return false;
            }

            @Override
            public void onSwiped(@NonNull RecyclerView.ViewHolder viewHolder, int direction) {
                mEntryListViewModel.deleteEntry(adapter.getEntryAtPos(viewHolder.getAbsoluteAdapterPosition()));
                Toast.makeText(LogsActivity.this, "Entry deleted.", Toast.LENGTH_SHORT).show();
            }
        }).attachToRecyclerView(mRecyclerView);

        adapter.setOnItemClickListener(new EntryAdapter.OnItemClickListener() {
            @Override
            public void onItemClick(Entry entry) {
                Intent intent = new Intent(LogsActivity.this, AddEditEntryActivity.class);
                intent.putExtra(AddEditEntryActivity.EXTRA_ID, entry.getID());
                intent.putExtra(AddEditEntryActivity.EXTRA_DATE, entry.getDate());
                intent.putExtra(AddEditEntryActivity.EXTRA_WEIGHT, entry.getWeight());
                startActivityForResult(intent, EDIT_ENTRY_REQUEST);
            }
        });

    }

    @Override
    public void onSaveInstanceState(@NonNull Bundle outState, @NonNull PersistableBundle outPersistentState) {
        super.onSaveInstanceState(outState, outPersistentState);
    }

    @Override
    protected void onRestoreInstanceState(@NonNull Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if(requestCode == ADD_ENTRY_REQUEST && resultCode == RESULT_OK){
            String date = data.getStringExtra(AddEditEntryActivity.EXTRA_DATE);
            String weight = data.getStringExtra(AddEditEntryActivity.EXTRA_WEIGHT);

            Entry newEntry = new Entry(date, weight);
            userEmail = getIntent().getStringExtra("email");
            userID =  mUserListViewModel.getUserID(userEmail);

            newEntry.setUserID(userID);

            mEntryListViewModel.addEntry(newEntry);
            Toast.makeText(this, "Entry saved.", Toast.LENGTH_SHORT).show();
        }else if(requestCode == EDIT_ENTRY_REQUEST && resultCode == RESULT_OK){
            long id = data.getIntExtra(AddEditEntryActivity.EXTRA_ID, -1);

            if(id == -1){
                Toast.makeText(this, "Entry cannot be updated.", Toast.LENGTH_SHORT).show();
                return;
            }
            String date = data.getStringExtra(AddEditEntryActivity.EXTRA_DATE);
            String weight = data.getStringExtra(AddEditEntryActivity.EXTRA_WEIGHT);

            Entry entry = new Entry(date, weight);
            entry.setID(id);
            userEmail = getIntent().getStringExtra("email");
            userID =  mUserListViewModel.getUserID(userEmail);

            entry.setUserID(userID);
            mEntryListViewModel.updateEntry(entry);
            Toast.makeText(this, "Entry updated.", Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(this, "Entry not saved.", Toast.LENGTH_SHORT).show();
        }
    }

    public void addEntryClick(View view) {
        EditText mDateEditText = findViewById(R.id.add_date);
        EditText mWeightEditText = findViewById(R.id.add_weight);

        if(mDateEditText.getText().toString().trim().isEmpty() || mWeightEditText.getText().toString().trim().isEmpty()){
            Toast.makeText(this, "Please enter both fields", Toast.LENGTH_SHORT).show();
        }else{
            Entry newEntry = new Entry(mDateEditText.getText().toString(), mWeightEditText.getText().toString());

            userEmail = getIntent().getStringExtra("email");
            userID =  mUserListViewModel.getUserID(userEmail);

            newEntry.setUserID(userID);

            mEntryListViewModel.addEntry(newEntry);

            mDateEditText.setText("");
            mWeightEditText.setText("");
        }
    }

    public void setGoalWeight(View view){
        EditText userGoalWeight = findViewById(R.id.user_goal_weight);
        userGoalWeight.setText(userGoalWeight.getText().toString());
    }

    public boolean onCreateOptionsMenu(Menu menu){
        MenuInflater menuInflater = getMenuInflater();
        menuInflater.inflate(R.menu.delete_all_entries_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        if (item.getItemId() == R.id.delete_all) {
            mEntryListViewModel.deleteAllEntries();
            Toast.makeText(this, "All entries deleted.", Toast.LENGTH_SHORT).show();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }


}
