package com.zybooks.weighttracker.model;

public class AuthenticatedUser {
    private String authUserEmail;
    private long authUserID;
    private  boolean isAuthenticated;

    public AuthenticatedUser(String email, long id, boolean auth){
        authUserEmail = email;
       authUserID = id;
       isAuthenticated = auth;
    }

    public String getAuthUserEmail(){
        return this.authUserEmail;
    }

    public long getAuthUserID() {
        return this.authUserID;
    }

    public boolean getIsAuthenticated(){
        return this.isAuthenticated;
    }


}
