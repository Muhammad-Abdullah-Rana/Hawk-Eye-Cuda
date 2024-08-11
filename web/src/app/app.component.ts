import { ChangeDetectionStrategy, Component, OnInit } from '@angular/core';
import { RouterLink, RouterOutlet } from '@angular/router';
import { MatButtonModule } from '@angular/material/button';
import { MatToolbarModule } from '@angular/material/toolbar';
import { MatIconModule } from '@angular/material/icon';
import { MatCardModule } from '@angular/material/card';
import { MatMenuModule } from '@angular/material/menu';
import { AuthService } from './services/auth.service';
import { User } from 'oidc-client-ts';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [CommonModule, RouterOutlet, RouterLink, MatButtonModule, MatToolbarModule, MatIconModule, MatCardModule, MatMenuModule],
  templateUrl: './app.component.html',
  styleUrl: './app.component.scss'
})
export class AppComponent implements OnInit {
  user: User;
  constructor(
    private authService: AuthService
  ) {

  }
  ngOnInit(): void {
    this.authService.getUser().then(user => {
      if(user){
        this.user = user;
        document.getElementById("user").innerText = user?.profile?.name;
        if(user?.profile?.role === 'Admin')
          document.getElementById("addUser").hidden = false;
      }
    })
  }

  login() {
    this.authService.login()
  }

  logout() {
    this.authService.logout();
  }

  addUser() {
    window.location.href = 'http://localhost:5000/Account/Register?returnUrl=%2Fauth-callback';
  }
}
