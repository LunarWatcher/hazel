# Installing Hazel

**NOTE:** This is an end-user document for the deployment of the server. If you're setting up Hazel for development, see `Developing.md`.

Note that you must install the dependencies listed in the README before installing Hazel.

Hazel provides an install script to automate most of the process:
```bash
# Default setup
# Note that the nginx file needs to be manually edited if you don't provide variables
curl -L https://raw.githubusercontent.com/LunarWatcher/hazel/master/scripts/install.sh | bash

# Default setup, different user
HAZEL_USER=hazel curl -L https://raw.githubusercontent.com/LunarWatcher/hazel/master/scripts/install.sh | bash

# With more automatic nginx setup
HAZEL_USER=hazel HAZEL_CERT=/etc/nova/nova.cert HAZEL_CERT_KEY=/etc/nova/nova.key HAZEL_DOMAIN=nova.lan curl -L https://raw.githubusercontent.com/LunarWatcher/hazel/master/scripts/install.sh | bash
```

**Variables:**
* `HAZEL_USER`: The user to use for Hazel. Defaults to `hazel` if not provided. **NOTE:** The user does NOT have to exist in advance. The install process creates it automatically
* `HAZEL_CERT`: The cert file to use for nginx. Note that this is used for generating a `.conf` file that's added to `/etc/nginx/conf.d`
* `HAZEL_CERT_KEY`: The private key for the certificate, also for nginx, and also for generating the `.conf` file for nginx
* `HAZEL_DOMAIN`: The domain to use. **Note:** by default, the domain is set to `hazel.${HAZEL_DOMAIN}`, so if you provide `nova.lan`, the generated nginx file will have its `server_name` set to `nginx.nova.lan`

## After running the installer

The installer handles most of the setup process, but not everything. 

### Reverse proxy

#### Option 1: Nginx

As previously spoiled, the installation process generates a file for nginx at `/etc/nginx/conf.d/hazel.conf`. If you provided the environment variables, it'll work out of the box, if your use-case happens to line up with mine. 

If you didn't supply the environment variables, you **must** edit the file. If you supplied them, look it over anyway and make sure everything you want and need is in there, but there's at least a chance no further editing is required. 

Once you're done, restart nginx with `sudo systemctl restart nginx`. Note that the Hazel will **not** yet work at this point, as the server isn't running.

#### Option 2: Other proxy
Hazel has not been verified with other proxies, so consequently, no other config options have been provided. If you use a different proxy, making a pull request with the setup instructions is welcome.

With that in mind, your reverse proxy must:
* Forward the `Authorization` header as-is
* Preferably use a subdomain[^1]

Once you're done, note that the Hazel will **not** yet work at this point, as the server isn't running.

#### Option 3: No proxy
If you don't want to use any reverse proxy, you can disregard these instructions and move on to configuring Hazel. The server will be available at `localhost:6906` (or alternatively another port if configured to use another port) after configuring and starting Hazel, no proxy required.

### Configuring Hazel

```
cd /etc/hazel
sudo cp hazel.template.conf hazel.conf
vim hazel.conf
```

See `docs/config/README.md` for the configuration guide.

## Starting Hazel

When you've completed all the base setup, you can start Hazel with
```
sudo systemctl enable hazel && sudo systemctl start hazel
```
After doing so, you'll be able to access the server either via the IP or your reverse proxy, depending on your setup.


## Docker?

No.

[^1]: If you opt not to use a subdomain for your reverse proxy, I cannot guarantee that hazel will cooperate. If you want to host it at `example.com/hazel/<url>`, your proxy **must** forward the url `/<url>`. If you forward `/hazel/url`, routing will break, and everything will 404. I will not be providing support for this use-case, so if you do this, you're on your own. Have fun 
