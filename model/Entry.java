package com.zybooks.weighttracker.model;

import static androidx.room.ForeignKey.CASCADE;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.ForeignKey;
import androidx.room.PrimaryKey;

@Entity(foreignKeys = @ForeignKey(entity = User.class, parentColumns = "id", childColumns = "userID", onDelete = CASCADE))
public class Entry {
    @PrimaryKey(autoGenerate = true)
    @ColumnInfo(name = "id")
    private long mID;

    @ColumnInfo(name = "date")
    private String mDate;

    @ColumnInfo(name = "weight")
    private String mWeight;

    @ColumnInfo(name = "userID")
    private long mUserID;

    // constructor & setter
    public Entry(String date, String weight){
        setDate(date);
        setWeight(weight);
    }

    public void setID(long id){
        this.mID = id;
    }

    public void setDate(String date){
        this.mDate = date;
    }

    public void setWeight(String weight){
        this.mWeight = weight;
    }
    public void setUserID(long userID){
        this.mUserID = userID;
    }


    // Getters
    public long getID(){
        return mID;
    }
    public String getDate(){
        return mDate;
    }
    public String getWeight(){
        return mWeight;
    }

    public long getUserID(){
        return mUserID;
    }

}
