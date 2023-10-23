package com.zybooks.weighttracker.viewmodel;

import android.app.Application;

import androidx.lifecycle.AndroidViewModel;
import androidx.lifecycle.LiveData;

import com.zybooks.weighttracker.model.User;
import com.zybooks.weighttracker.repo.UserRepository;

import java.util.List;

public class UserListViewModel extends AndroidViewModel {

    private UserRepository userRepo;

    public UserListViewModel(Application application){
        super(application);
        userRepo = UserRepository.getInstance(application.getApplicationContext());
    };

    public long getUserID(String email){
        return userRepo.getUserID(email);
    }

    public int getUserEmail(String email){
        return userRepo.getUserEmail(email);
    }


    public int getUserPass(String pass){
        return userRepo.getUserPass(pass);
    }

    public LiveData<List<User>> getUsers(){
        return userRepo.getUsers();
    }
    public void addUser(User user){
        userRepo.addUser(user);
    }

}
