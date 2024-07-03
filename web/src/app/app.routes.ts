import { Routes } from '@angular/router';
import { AuthCallbackComponent } from './shared/auth-callback/auth-callback.component';
import { SecureComponent } from './shared/secure/secure.component';
import { AuthGuard } from './shared/guard/auth.guard';

export const routes: Routes = [
  {
    path: 'auth-callback', component: AuthCallbackComponent
  },
  {
    path: 'secure', component : SecureComponent
  },
  {
    path: 'signin',
    loadChildren: () => import('./signin/signin.module').then(m => m.SigninModule)
  },
  {
    path: 'home',
    loadChildren: () => import('./home/home.module').then(m => m.HomeModule),
    canActivate: [AuthGuard]
  },
  {
    path: 'video-player',
    loadChildren: () => import('./video-player/video-player.module').then(m => m.VideoPlayerModule),
    canActivate: [AuthGuard]
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
