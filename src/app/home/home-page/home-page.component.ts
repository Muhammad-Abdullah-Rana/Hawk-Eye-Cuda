import { Component } from '@angular/core';
import { IUser } from '../../shared/model/iuser';
import { DatabaseService } from '../../shared/services/database/database.service';

@Component({
  selector: 'app-home-page',
  standalone: true,
  imports: [],
  templateUrl: './home-page.component.html',
  styleUrl: './home-page.component.scss'
})
export class HomePageComponent {
  protected currentUser: IUser;
  constructor(databaseService:DatabaseService){
    databaseService.getCurrentUser().subscribe(
      response => {
        this.currentUser = response;
      }
    );
  }
}
