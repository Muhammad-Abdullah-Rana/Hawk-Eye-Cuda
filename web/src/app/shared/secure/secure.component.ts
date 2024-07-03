import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { AuthService } from '../../services/auth.service';
import { MatButtonModule } from '@angular/material/button';
import { MatCardModule } from '@angular/material/card';

@Component({
  selector: 'app-secure',
  standalone: true,
  imports: [CommonModule, MatButtonModule,
    MatCardModule],
  templateUrl: './secure.component.html',
  styleUrl: './secure.component.scss'
})
export class SecureComponent implements OnInit {

  isLoggedIn = false;

  constructor(private authService: AuthService) { }

  ngOnInit(): void {
    // this.authService.getUser().then(user => {
    //   if (!user) {
    //     // this.authService.login();
    //   } else {
    //     this.isLoggedIn = true;
    //   }
    // });
  }

  login() {
    this.authService.login();
  }
}
