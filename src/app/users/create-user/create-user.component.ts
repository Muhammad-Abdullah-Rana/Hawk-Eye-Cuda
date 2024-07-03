import { Component } from '@angular/core';
import { FormControl, FormGroup, FormsModule, ReactiveFormsModule, Validators } from '@angular/forms';
import { MatButtonModule } from '@angular/material/button';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatIconModule } from '@angular/material/icon';
import { MatInputModule } from '@angular/material/input';
import { RouterLink } from '@angular/router';
import { DatabaseService } from '../../shared/services/database/database.service'

@Component({
  selector: 'app-create-user',
  standalone: true,
  imports: [RouterLink, MatFormFieldModule, MatInputModule, FormsModule, ReactiveFormsModule, MatIconModule, MatButtonModule],
  templateUrl: './create-user.component.html',
  styleUrl: './create-user.component.scss'
})

export class CreateUserComponent {

  createUserFormGroup!: FormGroup;
  protected hide:boolean = true;
  private databaseService:DatabaseService;
  constructor(databaseService:DatabaseService) {
    this.databaseService = databaseService;
    this.createUserFormGroup = new FormGroup({
      fName: new FormControl('', [Validators.required, Validators.minLength(3)]),
      lName: new FormControl('', [Validators.required, Validators.minLength(3)]),
      email: new FormControl('', [Validators.required, Validators.email]),
      password: new FormControl('', [Validators.required, Validators.pattern(/^(?=.*[0-9])(?=.*[!@#$%^&*])[a-zA-Z0-9!@#$%^&*]{6,16}$/)])
    });
  }

  protected getFNameErrorMessage() {
    if (this.createUserFormGroup.get('fName').hasError('required')) {
      return 'You must enter a First Name';
    }

    return this.createUserFormGroup.get('fName').hasError('minLength') ? 'Your First Name should be atleast three letters long' : '';
  }
  
  protected getLNameErrorMessage() {
    if (this.createUserFormGroup.get('lName').hasError('required')) {
      return 'You must enter a Last Name';
    }

    return this.createUserFormGroup.get('lName').hasError('minLength') ? 'Your Last Name should be atleast three letters long' : '';
  }

  protected getEmailErrorMessage() {
    if (this.createUserFormGroup.get('email').hasError('required')) {
      return 'You must enter an email';
    }

    return this.createUserFormGroup.get('email').hasError('email') ? 'You must enter a valid email' : '';
  }
  
  protected getPasswordErrorMessage() {
    if (this.createUserFormGroup.get('password').hasError('required')) {
      return 'You must enter a password';
    }
    return this.createUserFormGroup.get('password').hasError('pattern') ? 'You must enter a valid password' : '';
  }
  createUser(){
    this.databaseService.createUser({fName:'Muhammad', lName:'Abdullah', email:'m.abdullah4298@gmail.com', password:'asdf', imgURL: 'https://material.angular.io/assets/img/examples/shiba1.jpg'});
  }
}
