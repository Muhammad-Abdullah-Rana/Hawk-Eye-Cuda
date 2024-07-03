import { Routes } from '@angular/router';

export const routes: Routes = [
  {
    path: 'signin',
    loadChildren: () => import('./signin/signin.module').then(m => m.SigninModule)
  },
  {
    path: 'home',
    loadChildren: () => import('./home/home.module').then(m => m.HomeModule)
  },
  {
    path: 'video-player',
    loadChildren: () => import('./video-player/video-player.module').then(m => m.VideoPlayerModule)
  },
  {
    path: 'users',
    loadChildren: () => import('./users/users.module').then(m => m.UsersModule)
  },
  {
    path:'',
    redirectTo:'home',
    pathMatch:'full'
  }
];
