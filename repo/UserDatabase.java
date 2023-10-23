package com.zybooks.weighttracker.repo;

import androidx.room.Database;
import androidx.room.RoomDatabase;
import com.zybooks.weighttracker.model.Entry;
import com.zybooks.weighttracker.model.User;

@Database(entities = {User.class, Entry.class}, version = 2)
public abstract class UserDatabase extends RoomDatabase{

    public abstract EntryDao entryDao();
    public abstract UserDao userDao();
}
