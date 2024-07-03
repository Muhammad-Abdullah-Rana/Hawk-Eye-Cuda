import { Component } from '@angular/core';
import { FormControl, FormGroup, FormsModule, ReactiveFormsModule, Validators } from '@angular/forms';
import { MatButtonModule } from '@angular/material/button';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatIconModule } from '@angular/material/icon';
import { MatInputModule } from '@angular/material/input';
import { RouterLink } from '@angular/router';

@Component({
  selector: 'app-edit-users',
  standalone: true,
  imports: [FormsModule, ReactiveFormsModule, MatButtonModule, MatFormFieldModule, MatIconModule, MatInputModule, RouterLink],
  templateUrl: './edit-users.component.html',
  styleUrl: './edit-users.component.scss'
})
export class EditUsersComponent {


  editUserFormGroup!: FormGroup;
  protected hide:boolean = true;
  
  constructor() {
    this.editUserFormGroup = new FormGroup({
      fName: new FormControl('', [Validators.required, Validators.minLength(3)]),
      lName: new FormControl('', [Validators.required, Validators.minLength(3)]),
      email: new FormControl('', [Validators.required, Validators.email]),
      password: new FormControl('', [Validators.required, Validators.pattern(/^(?=.*[0-9])(?=.*[!@#$%^&*])[a-zA-Z0-9!@#$%^&*]{6,16}$/)])
    });
  }

  protected getFNameErrorMessage() {
    if (this.editUserFormGroup.get('fName').hasError('required')) {
      return 'You must enter a First Name';
    }

    return this.editUserFormGroup.get('fName').hasError('minLength') ? 'Your First Name should be atleast three letters long' : '';
  }
  
  protected getLNameErrorMessage() {
    if (this.editUserFormGroup.get('lName').hasError('required')) {
      return 'You must enter a Last Name';
    }

    return this.editUserFormGroup.get('lName').hasError('minLength') ? 'Your Last Name should be atleast three letters long' : '';
  }

  protected getEmailErrorMessage() {
    if (this.editUserFormGroup.get('email').hasError('required')) {
      return 'You must enter an email';
    }

    return this.editUserFormGroup.get('email').hasError('email') ? 'You must enter a valid email' : '';
  }
  
  protected getPasswordErrorMessage() {
    if (this.editUserFormGroup.get('password').hasError('required')) {
      return 'You must enter a password';
    }
    return this.editUserFormGroup.get('password').hasError('pattern') ? 'You must enter a valid password' : '';
  }
  createUser(){

  }
}
