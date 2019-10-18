# ZDoList

ZDoList is a Taskmanager for handling simple tasks.
The main advantage is that you can drag/drop mails or copy mails from your favorite mail client directly to the software, and the software will retain a direct link to the mail client and, if needed, can open the mail within the client.

At the moment this does although only work with **Thunderbird** but will in the future include other
mail clients.

## File format

The software works by using todo.txt file format http://todotxt.org/
with the exception that **context**, **project** and **url:** are confined by '

## Installation

ZDoList is created using Qt Creator, so you need to download the repositories
and then compile the code using Qt. No other dependencies for compilation are needed.

## Dependencies

The software does not have any dependencies when it comes to compiling but need the *thunderlink* plugin for thunderbird at the moment. The drag/drop functionality from thunderbird works without this plugin but you will not be able to see the *subject line* of the mail and if you move the mail, the link to it will not function.

### Installing Thunderlink

If you want to make it work with Thunderbird you need to install
the *thunderlink* plugin.

1. Goto **Add-ons**
1. Search for *Thunderlink*
1. Install the software
1. Goto **Add-ons** preferences
1. Change the *string 7* to *ZDoList* in first
   textbox
   and then in multi-row textbox

     \<thunderlink\>|\<subject\>

### Using Thunderlink

1. If you followed instructions you can then copy the thunderlink
   using *CTRL+ALT+7*
1. When creating new new task do so as normal BUT paste from clipboard directly into the *new task* textbox.
1. The software should automatically detect that this is a task from thunderbird
