# Miniflux proxy

Yay, Miniflux supports webhooks. Boo, it doesn't work with discord because Miniflux sends a different format than Discord accepts.

The miniflux proxy is a metafeature that defines the webhook `username:password@<hazel base url>/api/miniflux/webhook` (Where the username corresponds to a specific configuration, and the password is, as usual, a password used for authentication; think of it more as an API key), and converts Miniflux's webhook output into something digestable on other platforms.

Note that this particular feature is primarily meant to be a way to forward the URL, as a direct replacement for [an old feed reader I wrote](https://github.com/LunarWatcher/DiscordWebhooks.cpp), that had a severe bug (that I have yet to identify, because I couldn't be bothered) that periodically wiped config.

## Config
```
"miniflux_proxies": {   
    "some_username_used_as_an_internal_identifier": {
        "events": ["new_entries"],
        "passphrase": "A password",
        "receiverUrl": "https://example.com/webhook",
        "secret": "",
        "format": "discord"
    }
}
```

**Descriptions:**
* `some_username_used_as_an_internal_identifier`: Essentially the name of the particular webhook config. This particular key can and should be modified, as it's the `username` part of `username:password@hazel/api/miniflux/webhook`
* `events`: The miniflux webhook events to forward. There are no special keys, and the entries must correspond  to [a miniflux webhook event](https://miniflux.app/docs/webhooks.html), of which there are only two at the time of writing: `new_entries` and `save_entry`. See the docs for your version of Miniflux to see what events are available. Hazel does not add any restrictions to what events you can subscribe to, so invalid events are just never going to be triggered.
* `passphrase`: A password used to protect the endpoint. It's more of an API key 
* `secret`: Miniflux' generated webhook secret, as seen in integrations. Will be used for validation if non-empty. Currently, no validation is implemented, so the secret is not required nor used, but reserved for future use.
    * It's strongly recommended to set it, as it means there's an additional layer of security if the username and/or password are weak, and the server is poorly protected.
* `format`: Currently, only "discord" is allowed.
