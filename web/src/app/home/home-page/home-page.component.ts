import { Component } from '@angular/core';
// import { IUser } from '../../shared/model/iuser';
import { AuthService } from '../../services/auth.service';
import { User } from 'oidc-client-ts';
import { RouterOutlet } from '@angular/router';


@Component({
  selector: 'app-home-page',
  standalone: true,
  imports: [RouterOutlet],
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
    });
  }
}
