import { CommonModule } from '@angular/common';
import { Component } from '@angular/core';
import { AuthService } from '../../services/auth.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-auth-callback',
  standalone: true,
  imports: [CommonModule],
  template: '',
})
export class AuthCallbackComponent {

  constructor(private authService: AuthService, private router: Router) { }

  ngOnInit(): void {
    this.authService.completeLogin().then(() => {
      this.router.navigate(['/']);
    });
  }

}
