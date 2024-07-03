import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { IUser } from '../../model/iuser';
import { BehaviorSubject, Observable, Observer } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class DatabaseService {
  private currentUser:BehaviorSubject<IUser> = new BehaviorSubject({fName:'Guest',lName:'',email:'',password:'', imgURL: 'D:/Projects/Angular/hawkeye/src/assets/aaa.jpeg'});

  constructor(private http: HttpClient) {}

  public getCurrentUser(){
    return this.currentUser;
  }
  public createUser(newUser:IUser){
    this.http.post('https://hawkeye-bd4f3-default-rtdb.asia-southeast1.firebasedatabase.app/users.json', newUser).subscribe();
  }
}