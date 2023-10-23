package com.zybooks.weighttracker.model;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.PrimaryKey;

@Entity
public class User {
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "id")
    private long mID;

    @ColumnInfo(name = "email")
    private String mEmail;

    @ColumnInfo(name = "password")
    private String mPassword;

    @ColumnInfo(name = "phone")
    private String mPhone;

    // constructor & setter
    public User(String email, String password, String phone){
       setEmail(email);
       setPassword(password);
       setPhone(phone);
    };

    // setters
    public void setID(long id){
        this.mID = id;
    }

    public void setEmail(String email){
        this.mEmail = email;
    }

    public void setPassword(String password){
        this.mPassword = password;
    }

    public void setPhone(String phone){
        this.mPhone = phone;
    }


    // Getters
    public String getEmail(){
        return mEmail;
    }
    public String getPassword(){
        return mPassword;
    }
    public String getPhone(){
        return mPhone;
    }
    public long getID(){
        return mID;
    }


};
