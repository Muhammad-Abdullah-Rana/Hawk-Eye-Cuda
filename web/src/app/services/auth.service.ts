import { Injectable } from '@angular/core';
import { UserManager, User, SignoutResponse, UserManagerSettings } from 'oidc-client-ts';

@Injectable({
  providedIn: 'root'
})
export class AuthService {
  private userManager: UserManager;
  private user: User | null = null;

  constructor() {
    const settings : UserManagerSettings = {
        authority: 'http://localhost:5000',
        client_id: 'angularApp',
        client_secret: 'secret',
        redirect_uri: 'http://localhost:4200/auth-callback',
        post_logout_redirect_uri: 'http://localhost:4200/secure',
        response_type: 'code',
        scope: 'openid profile angularApp',
        loadUserInfo: true,
        
      };
    this.userManager = new UserManager(settings);

    this.userManager.events.addUserLoaded(user => {
      this.user = user;
    });

    this.userManager.events.addUserUnloaded(() => {
      this.user = null;
    });
  }

  public getUser(): Promise<User | null> {
    return this.userManager.getUser();
  }

  public login(): Promise<void> {
    return this.userManager.signinRedirect();
  }

  public completeLogin(): Promise<void> {
    return this.userManager.signinRedirectCallback().then(user => {
      this.user = user;
    });
  }

  public logout(): Promise<void> {
    return this.userManager.signoutRedirect();
  }

  public completeLogout(): Promise<void> {
    return this.userManager.signoutCallback().then(user => {
      this.user = null;
    });
  }

  public isLoggedIn(): boolean {
    return this.user != null;
  }

  public getUserName(){
    return this.user?.profile?.name;
  }

  public getUserRoles(): any {
    return this.user?.profile?.role;
  }
}
