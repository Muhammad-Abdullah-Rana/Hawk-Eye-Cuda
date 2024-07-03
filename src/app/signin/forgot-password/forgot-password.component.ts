import { Component } from '@angular/core';
import {FormControl, Validators, FormsModule, ReactiveFormsModule, FormGroup} from '@angular/forms';
import {MatInputModule} from '@angular/material/input';
import {MatFormFieldModule} from '@angular/material/form-field';
import {MatIconModule} from '@angular/material/icon';
import {MatButtonModule} from '@angular/material/button';
import { RouterLink } from '@angular/router';

@Component({
  selector: 'app-forgot-password',
  standalone: true,
  imports: [RouterLink, MatFormFieldModule, MatInputModule, FormsModule, ReactiveFormsModule, MatIconModule, MatButtonModule],
  templateUrl: './forgot-password.component.html',
  styleUrl: './forgot-password.component.scss'
})

export class ForgotPasswordComponent {
  forgotPasswordFormGroup!: FormGroup;
  
  constructor() {
    this.forgotPasswordFormGroup = new FormGroup({
      email: new FormControl('', [Validators.required, Validators.email])
    });
  }

  protected getEmailErrorMessage() {
    if (this.forgotPasswordFormGroup.get('email').hasError('required')) {
      return 'You must enter an email';
    }

    return this.forgotPasswordFormGroup.get('email').hasError('email') ? 'You must enter a valid email' : '';
  }

  public forgot_password(){

  }
}
