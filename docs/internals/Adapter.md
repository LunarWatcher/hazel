# Adapters

## General config
```json
"adapters": {
    "unique adapter ID; used in various feature configs (such as the miniflux proxy)": {
        "type": "enum, see this document for legal values",
        "_comment": "Note that other config variables depend on the type used."
    }
}
```

## Terminology
* Config variables: Refers to variables present in `adapters.adapterID`. These define the default config for the adapter.
* Local adapter config: refers to variables that can be used in `adapter_config` in, for example, the miniflux proxy. These are the override options for (usually) a subset of the config variables. Note that in some cases, some options are only available as local adapter config variables, if the variable is considered too local to have a sane default for.
    * Note: Many of the local adapter config keys are identical to their config variable counterparts, and therefore lack descriptions. To see what a local adapter config without a description does, look at the config variable list

## Available adapters
### Discord (Webhook interface)

**Type key:** `discord`

#### Config variables
* `dest_url`: The webhook URL to send content to
* `username`: The username to use on Discord. Default: "Hazel"

#### Local adapter config
* `username`: Overrides the `username` config variable.

### Ntfy

**Type key:** `ntfy`

#### Config variables
* `default_topic`: The topic to send notifications to
* `dest_url`: The URL to the NTFY instance. **DO NOT** include the topic in the URL.
* `api_token`: A Bearer token for authorization. See [NTFY's documentation](https://docs.ntfy.sh/publish/#access-tokens) for details on generating one.

#### Local adapter config
* `topic`: Overrides the `default_topic` config variable.
* `priority`: See [NTFY's documentation](https://docs.ntfy.sh/publish/#message-priority). Default: 3
* `title`: See [NTFY's documentation](https://docs.ntfy.sh/publish/#message-title). Default: "Hazel update"
* `tags`: See [NTFY's documentation](https://docs.ntfy.sh/publish/#tags-emojis). Default: None
