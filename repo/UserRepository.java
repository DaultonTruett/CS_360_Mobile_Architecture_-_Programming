package com.zybooks.weighttracker.repo;

import android.content.Context;
import androidx.room.Room;
import androidx.lifecycle.LiveData;
import com.zybooks.weighttracker.model.User;
import com.zybooks.weighttracker.model.Entry;
import java.util.List;

public class UserRepository {
    private static UserRepository mUserRepo;
    private final UserDao mUserDao;
    private final EntryDao mEntryDao;


    public static UserRepository getInstance(Context context){
        if(mUserRepo == null){
            mUserRepo = new UserRepository(context);
        }
        return mUserRepo;
    }

    private UserRepository(Context context){
       UserDatabase database = Room.databaseBuilder(context, UserDatabase.class, "user.db")
                .allowMainThreadQueries()
                .fallbackToDestructiveMigration().build();

        mUserDao = database.userDao();
        mEntryDao = database.entryDao();
    }

    // user operations on database
    public long getUserID(String email){
        return mUserDao.getUserID(email);
    }

    public int getUserEmail(String email){
        return mUserDao.getUserEmail(email);
    }

    public int getUserPass(String password){
        return mUserDao.getUserPass(password);
    }

    public LiveData<List<User>> getUsers(){
        return mUserDao.getUsers();
    }

    public void addUser(User user){
        long userId = mUserDao.addUser(user);
        user.setID(userId);
    };

    public void deleteUser(User user){
        mUserDao.deleteUser(user);
    }

    public void updateEmail(User user){
        mUserDao.updateEmail(user);
    }

    public void updatePassword(User user){
        mUserDao.updatePassword(user);
    }

    public void updatePhone(User user){
        mUserDao.updatePhone(user);
    }


    // Entries
    public LiveData<Entry> getEntry(long entryId){
        return mEntryDao.getEntry(entryId);
    }
    public LiveData<List<Entry>> getEntries(long userID){
        return mEntryDao.getEntries(userID);
    }
    public void addEntry(Entry entry){
        long entryId = mEntryDao.addEntry(entry);
        entry.setID(entryId);
    }
    public void deleteEntry(Entry entry){
        mEntryDao.deleteEntry(entry);
    }

    public void deleteAllEntries(){};
    public void updateEntry(Entry entry){
        mEntryDao.updateEntry(entry);
    }

}
