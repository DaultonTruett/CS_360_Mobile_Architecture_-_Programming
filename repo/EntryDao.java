package com.zybooks.weighttracker.repo;

import androidx.room.*;
import androidx.lifecycle.LiveData;
import com.zybooks.weighttracker.model.Entry;
import java.util.List;

@Dao
public interface EntryDao {
    @Query("SELECT * FROM Entry WHERE id = :id")
    LiveData<Entry> getEntry(long id);

    @Query("SELECT * FROM Entry WHERE userID = :userID ORDER BY id DESC")
    LiveData<List<Entry>> getEntries(long userID);

    @Insert
    long addEntry(Entry entry);

    @Update
    void updateEntry(Entry entry);

    @Delete
    void deleteEntry(Entry entry);

    @Query("DELETE FROM entry")
    void deleteAllEntries();
}
