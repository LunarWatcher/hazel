# Dashboard

The dashboard is one of Hazel's built-in features. 

## Modules

The dashboard is built around different modules. Note that the modules are separated into distinct types, so they can't be easily mixed in the final output. This may become an option in the future, but is not  currently a goal.

### Link module (`links` config key)

The link module can be used as a way to, shock, link to other stuff. 

Some sites support additional features in the display. These are called "dynamic apps" internally, because I wasn't creative when I named them. These can show, for example, aggregate statistics directly on the dashboard sourced from the service linked to. 

Note that these are optional, including for enabled services, and have to be manually configured.

#### Config format

```json
"dashboard": {
    "links": {
        "name": "Display name in the dashboard",
        "type": "app type; optional, see 'Supported dynamic apps' for the list of allowed values",
        "url": "https://example.com",
        "config": {
            "Contains a set of key-value pairs used for configuring dynamic apps. Unsupported values are quietly ignored. See the docs for each dynamic app for allowed values"
        }
    }
}
```

#### Dynamic app caveats

Dynamic app updates are only done on a single thread. This means that if one update takes a significant amount of time, other updates may be delayed.

It's therefore strongly recommended to not set the `update_frequency` interval too low, as this may result in update cycles taking longer than planned. Additionally, due to implementation reasons, this will slow down clients that want to load at the same time as the update. 

This is a consequence of how the dynamic apps are set up. They're not queried by the client when the page is loaded, but handled fully on the backend. This is done to allow the dashboard to be displayed by other users without leaking API keys and similar.

In the event of future multi-user support, some dynamic apps may be handled etnirely client-sided. 

#### Supported dynamic apps

* Pi-hole (`pihole`)
* Miniflux (`miniflux`)
* Uptime-kuma (`uptime-kuma`)

##### Pi-hole
| Config key name | Description | Type | Required |
| --------------- | ----------- | ---- | -------- |
| `api_key`       | The API key for pihole, as found in pihole's settings | String | Yes |
| `update_frequency` | The number of seconds between updates | integer, >= 5 | No; defaults to 600 |
| `verify_ssl` | Whether or not to verify certificates. This may need to be set to `true` for self-signed certificates | bool | No; defaults to false. |

##### Uptime-kuma

| Config key name | Description | Type | Required |
| --------------- | ----------- | ---- | -------- |
| `dashboard`     | The dashboard to gather data from. Note that a public dashboard must be configured in uptime-kuma | string | Yes |
| `update_frequency` | The number of seconds between updates | integer, >= 5 | No; defaults to 60 |
| `verify_ssl` | Whether or not to verify certificates. This may need to be set to `true` for self-signed certificates | bool | No; defaults to false. |

##### Miniflux

| Config key name | Description | Type | Required |
| --------------- | ----------- | ---- | -------- |
| `api_key`[^1]     | An API key for Miniflux, generated under settings. | string | Yes |
| `update_frequency` | The number of seconds between updates | integer, >= 5 | No; defaults to 1800 |
| `verify_ssl` | Whether or not to verify certificates. This may need to be set to `true` for self-signed certificates | bool | No; defaults to false. |

#### Self-signed certificates

**NOTE:** This only applies if you haven't installed the CA certificate to the system. If you can `curl https://your-domain` without it complaining about an SSL cert problem, it's probably fine. You'll also get an equivalent error in Hazel's logs (`journalctl -r -u hazel`) if the requests fail.

If you can't get this to work, the rest of the section is for you. 

---

For now, self-signed certificates are supported by disabling SSL verification. You can do this by setting `verify_ssl` in dynamic apps to `false`. Note that this can have security implications if  the domains are hijacked.

libcpr, the underlying request library, [supports custom CA verification](https://docs.libcpr.org/advanced-usage.html#certificate-authority-ca-bundle), but this has not been set up because it's tedious and I just don't want to. Pull requests adding support for self-signed certificates are welcome.

[^1]: Miniflux's API does support username and password authentication, but this is not supported by Hazel to minimise username and password hard-coding in config
