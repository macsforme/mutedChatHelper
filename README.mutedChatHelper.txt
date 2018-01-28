BZFlag Server Plugin: mutedChatHelper
==============================================================================

This plugin allows muted players to send certain pre-determined messages to
public chat, team chat, and in private messages using slash commands. No
configuration is needed other than to load the plugin. The plugin will check
whether joining players have the SPAWN permission, but lack the TALK and
PRIVATEMESSAGE permissions, and if so it will send the players a list of
commands that can be used. These commands are also available to players who
are allowed to talk, but they will not be advertised to them upon joining.

Please note that both the TALK and PRIVATEMESSAGE permissions should be given
or withheld together as a set. If only one or the other is given, players may
lose the ability to talk altogether.


Loading the plugin
--------------------------------------------------------------------------------

This plugin takes no optional arguments, so load it with:
  -loadplugin mutedChatHelper


Server Commands
--------------------------------------------------------------------------------

This plugin implements several custom slash commands, which are sent in a list
to muted players when they join the server. For the sake of avoiding
redundancy, they are not listed here in the README.
