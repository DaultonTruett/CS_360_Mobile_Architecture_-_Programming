package com.zybooks.weighttracker.viewmodel;

import android.app.Application;
import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.LiveData;
import com.zybooks.weighttracker.model.Entry;
import com.zybooks.weighttracker.repo.UserRepository;
import java.util.List;
public class EntryListViewModel extends AndroidViewModel {

    private UserRepository entryRepo;
    private UserRepository userRepo;


    public EntryListViewModel(Application application){
        super(application);
        entryRepo = UserRepository.getInstance(application.getApplicationContext());
    }

    public LiveData<Entry>getEntry(long id){
        return entryRepo.getEntry(id);
    }
    public LiveData<List<Entry>> getEntries(long userID){
        return entryRepo.getEntries(userID);
    }

    public void addEntry(Entry entry){
        entryRepo.addEntry(entry);
    }

    public void deleteEntry(Entry entry){
        entryRepo.deleteEntry(entry);
    }

    public void deleteAllEntries(){
        entryRepo.deleteAllEntries();
    }

    public void updateEntry(Entry entry){
        entryRepo.updateEntry(entry);
    }

}
