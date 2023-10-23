package com.zybooks.weighttracker.repo;

import androidx.lifecycle.LiveData;
import androidx.room.*;
import com.zybooks.weighttracker.model.User;
import java.util.List;

@Dao
public interface UserDao {

    @Query("SELECT id FROM User WHERE email = :email")
    long getUserID(String email);

    @Query("SELECT * FROM User WHERE email = :email")
    int getUserEmail(String email);

    @Query("SELECT * FROM User WHERE password = :password")
    int getUserPass(String password);

    @Query("SELECT * FROM User")
    LiveData<List<User>> getUsers();

    @Insert(onConflict = OnConflictStrategy.IGNORE)
    long addUser(User user);

    @Update
    void updateEmail(User user);

    @Update
    void updatePassword(User user);

    @Update
    void updatePhone(User user);

    @Delete
    void deleteUser(User user);

}
