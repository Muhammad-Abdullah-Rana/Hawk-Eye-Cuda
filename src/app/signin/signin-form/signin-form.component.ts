import { Component } from '@angular/core';
import {FormControl, Validators, FormsModule, ReactiveFormsModule, FormGroup} from '@angular/forms';
import {MatInputModule} from '@angular/material/input';
import {MatFormFieldModule} from '@angular/material/form-field';
import {MatIconModule} from '@angular/material/icon';
import {MatButtonModule} from '@angular/material/button';
import { RouterLink } from '@angular/router';


@Component({
  selector: 'app-signin-form',
  standalone: true,
  imports: [RouterLink, MatFormFieldModule, MatInputModule, FormsModule, ReactiveFormsModule, MatIconModule, MatButtonModule],
  templateUrl: './signin-form.component.html',
  styleUrl: './signin-form.component.scss'
})
export class SigninFormComponent {
  
  loginFormGroup!: FormGroup;
  protected hide:boolean = true;
  
  constructor() {
    this.loginFormGroup = new FormGroup({
      email: new FormControl('', [Validators.required, Validators.email]),
      password: new FormControl('', [Validators.required, Validators.pattern(/^(?=.*[0-9])(?=.*[!@#$%^&*])[a-zA-Z0-9!@#$%^&*]{6,16}$/)])
    });
  }

  protected getEmailErrorMessage() {
    if (this.loginFormGroup.get('email').hasError('required')) {
      return 'You must enter an email';
    }

    return this.loginFormGroup.get('email').hasError('email') ? 'You must enter a valid email' : '';
  }
  
  protected getPasswordErrorMessage() {
    if (this.loginFormGroup.get('password').hasError('required')) {
      return 'You must enter a password';
    }
    return this.loginFormGroup.get('password').hasError('pattern') ? 'You must enter a valid password' : '';
  }

  public login(){

  }
}
