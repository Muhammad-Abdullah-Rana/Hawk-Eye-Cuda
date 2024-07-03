import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { RouterModule, Routes } from '@angular/router';
import { UsersOptionsComponent } from './users-options/users-options.component';
import { CreateUserComponent } from './create-user/create-user.component';
import { EditUsersComponent } from './edit-users/edit-users.component';

const routes: Routes = [
  {
    path: '',
    component: UsersOptionsComponent
  },
  {
    path: 'create-user',
    component: CreateUserComponent
  },
  {
    path: 'edit-users',
    component: EditUsersComponent
  },
];

@NgModule({
  declarations: [],
  imports: [
    CommonModule,
    RouterModule.forChild(routes),
  ]
})
export class UsersModule { }
