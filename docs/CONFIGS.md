# FlashBack Client Configs

**All FlashBack Client config files are yaml files located at** `~/.config/flashbackclient`

- [Default Settings](#default-settings)
- [Default Scheduler Configs](#default-scheduler-configs)
- [Target-Specific Configs and Rules](#target-specific-configs-and-scheduler-rules)

## Default Settings
*Located at `~/.config/flashbackclient/flashbackclient.yaml`*

This file provides the default settings for the following settings:
- Encryption (`"none"`, `"file"`, or `"direcory"`, default `"none"` if no value specified)\
*Note - directory-level encoding encrypts directory structure, disabling sequential backup*
- `keep_on_target_move` (default `false`): If the target is moved, setting this to `true` will continue to track it, while `false` will consider the target as deleted.
- `ignore_paths` (default none): A list of regex patterns to ignore when
checking for changes. Patterns can be specified as follows:
  - A recursive pattern: `"pattern"`
  - A non-recursive pattern: `"./pattern"`
  - A wildcard: `"pattern/*"`, `"pattern/*.txt"`
  - A single character wildcard: `"pattern-?"`
  - A range: `"pattern-[a-z]"`
  - A negated pattern: `"!pattern"`, `"pattern-[!a-z]"`
  - An escaped character: `"pattern\\*"`, `"pattern\\[name\\]"`\
*Note: due to the yaml parser used, the backslash must also be escaped: `\\`
must be used instead of `\`*

An example file might look something like this:
```yaml
# ~/.config/flashbackclient/flashbackclient.yaml
# Default settings for FlashBack Client

encrypt: "file"

keep_on_target_move: false

path_ignores:
  - ignore: "./ignore"
  - ignore: "*.ignore"
  - ignore: "./subdir/ignore"
  - ignore: "ignore-num[0-9].txt"
  - ignore: "anychar?.txt"
  - ignore: "not-class[!0-9].txt"
  - ignore: "escaped\\[name\\].txt"
  - ignore: "escaped\\[also-works].txt"
```

## Default Scheduler Configs
*Located at `~/.config/flashbackclient/scheduler/scheduler.yaml`*

This file contains the default rules for the scheduler to use when checking targets.

### Actions
There are four actions:
- `download_changed` checks the server for any file changes and fetches changes, if any are found.
- `upload_changed` checks the server to see if local files are ahead, and sends these files to the server if any are found.
- `sync_files` checks both the `download_changed` and `upload_changed` conditions and performs any necessary action to sync local and remote files.
- `no_action` performs no action and is useful as a placeholder

### Triggers
There are five triggers:
- `on_startup` is triggered when the scheduler starts up
- `on_shutdown` is triggered when the scheduler shuts down
- `on_file_change` is triggered when a file is changed
- `on_schedule` is triggered when a cron expression is met
- `after_interval` is triggered after a certain amount of time has passed since the last trigger

Two triggers, `on_schedule` and `after_interval`, have additional settings that can be used to define the trigger.

`on_schedule` has a `times` setting that can be used to define the cron expressions to trigger the action.

```yaml
times:
  - cron_exp: "* * * * MON-FRI"
  - cron_exp: "* * * * SAT"
  - cron_exp: "00 00 * * *"
  - cron_exp: "00 12 * * *"
  - cron_exp: "00 17 * * FRI"
  - cron_exp: "00 09 * * MON"
```

`after_interval` has an additional setting `file_change_trigger` that can be used to defitriggerne if the wait time should be reset after a file change or not. This setting is set to `false` by default.

`after_interval` also has an `after_last` and `before_next_scheduled` setting that can be used to define the minimum time before the next scheduled action and after the last action. These settings are in minutes, and will be set to the default value of `0` if not specified.

*Note - not specifying either `after_last` or `before_next_scheduled` or setting both to `0` will mean the case will be ignored.*

```yaml
file_change_trigger: false
after_last: 120
before_next_scheduled: 60
```

### Rules
Rules are used to define the conditions for each action to take.

Each rule has an `id` and a `name` that can be used to identify the rule. Both can be used when overriding rules in target-specific configs.

The `action` setting is used to define the action to take when the rule is triggered.

The `cases` setting is used to define the triggers that will trigger the action.

Each case has an `id` that points to a trigger defined in the `triggers` enum.

An example of some possible rules is below:\
Rules with `id` 5 through 7 are examples of more complex rules, utilizing multiple trigges.
```yaml
- id: 0
  name: "Download on startup"
  action: "download_changed"
  cases:
    - id: "on_startup"

- id: 1
  name: "Upload on shutdown"
  action: "upload_changed"
  cases:
    - id: "on_shutdown"

- id: 2
  name: "Sync files noon and midnight"
  action: "sync_files"
  cases:
    - id: "on_schedule"
      times:
        - cron_exp: "00 00 * * *"
        - cron_exp: "00 12 * * *"

- id: 3
  name: "Upload end of work week"
  action: "upload_changed"
  cases:
    - id: "on_schedule"
      times:
        - cron_exp: "00 17 * * FRI"

- id: 4
  name: "Download start of work week"
  action: "download_changed"
  cases:
    - id: "on_schedule"
      times:
        - cron_exp: "00 09 * * MON"

- id: 5
  name: "Sync changed files after 2 hours"
  action: "sync_files"
  cases:
    - id: "on_file_change"
    - id: "after_interval"
      after_last: 120
      before_next_scheduled: 60

- id: 6
  name: "Download on startup on weekdays"
  action: "download_changed"
  cases:
    - id: "on_startup"
    - id: "on_schedule"
      times:
        - cron_exp: "* * * * MON-FRI"

- id: 7
  name: "Upload on shutdown on weekdays"
  action: "upload_changed"
  cases:
    - id: "on_shutdown"
    - id: "on_schedule"
      times:
        - cron_exp: "* * * * MON-FRI"
```

A complete example file might look something like this:
```yaml
# ~/.config/flashbackclient/scheduler/scheduler.yaml
# Default rules for the scheduler

# Rules for triggering actions
rules:
  - id: 0
    name: "Download on startup"
    action: "download_changed"
    cases:
      - id: "on_startup"

  - id: 1
    name: "Upload on shutdown"
    action: "upload_changed"
    cases:
      - id: "on_shutdown"

  - id: 2
    name: "Sync changed files after 2 hours"
    action: "sync_files"
    cases:
      - id: "on_file_change"
      - id: "after_interval"
        after_last: 120
        before_next_scheduled: 60
```

## Target-Specific Configs and Scheduler Rules
*Located at `~/.config/flashbackclient/targets/`*

This directory contains target-specific configs for the scheduler.

Each yaml file defines a target (either a file or a directory) and the rules and settings to use for that target.

The file must begin with a `name`, `path`, and `target_type` setting.

- The `name` setting is the name of the target.
- The `path` setting is the path to the target.
- The `target_type` setting is the type of target. This can be either `file` or `directory`.

Default settings can be overridden by providing a new value for a given setting.

The file may also contain a `rules` setting that contains the rules to use for the target. This is structured the same as the `rules` setting in the default scheduler config.

The file may also contain an `override_rules` setting that contains the rules to override the default rules for the target.

To override a default rule, use the `id` of the rule in the `override_rules` setting.

An example file might look something like this:
```yaml
# ~/.config/flashbackclient/targets/example.yaml
# Example target file defining a target named example

name: "Example"
path: "/home/luke/Dev/FlashBackClient"
target_type: "file"

rules:
  - name: "Sync changed files after 30 minutes"
    action: "sync_files"
    cases:
      - id: "on_file_change"
      - id: "after_interval"
        after_last: 30
        before_next_scheduled: 30

override_rules:
  - id: 2
```
