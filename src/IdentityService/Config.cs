using Duende.IdentityServer.Models;

namespace IdentityService;

public static class Config
{
    public static IEnumerable<IdentityResource> IdentityResources =>
        new IdentityResource[]
        {
            new IdentityResources.OpenId(),
            new IdentityResources.Profile(),
        };

    public static IEnumerable<ApiScope> ApiScopes =>
        new ApiScope[]
        {
            new ApiScope("angularApp", "Hawkeye App full access"),
            new ApiScope("angular-client", "Hawkeye App full access"),
            new ApiScope("normal", "Hawkeye normal user access")
        };

    public static IEnumerable<Client> Clients(IConfiguration configuration) =>
        new Client[]
        {
            new Client
            {
                ClientId = "postman",
                ClientName = "Postman",
                AllowedScopes = { "openid", "profile", "angularApp" },
                RedirectUris = { "https://www.getpostman.com/oauth2/callback" },
                ClientSecrets = { new Secret("NotASecret".Sha256()) },
                AllowedGrantTypes = GrantTypes.ResourceOwnerPassword,
            },
            new Client
            {
                ClientId = "angularApp",
                ClientName = "angularApp",
                ClientSecrets = { new Secret("secret".Sha256()) },
                AllowedGrantTypes = GrantTypes.CodeAndClientCredentials,
                RequirePkce = false,
                RedirectUris = { "http://localhost:4200/auth-callback" },
                PostLogoutRedirectUris = { "http://localhost:4200/secure" },
                AllowOfflineAccess = true,
                AllowedScopes = { "openid", "profile", "angularApp" },
                AccessTokenLifetime = 3600*24*3000,
                AlwaysIncludeUserClaimsInIdToken = true,
            },
            new Client
            {
                ClientId = "angular-client",
                ClientName = "Angular Client",
                AllowedGrantTypes = GrantTypes.Code,
                RequireClientSecret = false,
                RedirectUris = { "http://localhost:4200/auth-callback" },
                PostLogoutRedirectUris = { "http://localhost:4200/" },
                AllowedCorsOrigins = { "http://localhost:4200" },
                AllowedScopes = { "openid", "profile", "api1" },
                AllowAccessTokensViaBrowser = true,
                RequirePkce = true
            },
            new Client
            {
                ClientId = "normal",
                ClientName = "normal",
                ClientSecrets = { new Secret("secret".Sha256()) },
                AllowedGrantTypes = GrantTypes.CodeAndClientCredentials,
                RequirePkce = false,
                RedirectUris = { configuration["ClientApp"] + "/api/auth/callback/id-server" },
                AllowOfflineAccess = true,
                AllowedScopes = { "openid", "profile", "normal" },
                AccessTokenLifetime = 3600*24*3000,
                AlwaysIncludeUserClaimsInIdToken = true,
            }
        };
}
