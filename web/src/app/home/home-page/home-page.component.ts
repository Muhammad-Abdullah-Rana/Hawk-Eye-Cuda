import { Component } from '@angular/core';
// import { IUser } from '../../shared/model/iuser';
import { DatabaseService } from '../../shared/services/database/database.service';
import { AuthService } from '../../services/auth.service';
import { User } from 'oidc-client-ts';

@Component({
  selector: 'app-home-page',
  standalone: true,
  imports: [],
  templateUrl: './home-page.component.html',
  styleUrl: './home-page.component.scss'
})
export class HomePageComponent {
  protected user: User;
  // protected currentUser: IUser;
  constructor(
    private auth:AuthService){
    this.auth.getUser().then((user : User) => {
      if(user){
        this.user = user;
      }
    })
  }
}
