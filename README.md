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
1. **Add-ons** preferences does not work well in later version
1. Use *clickable subject* which has the format

   \<A HREF="\<thunderlink\>">\<subject\>\</A\>
1. *clickable subject* has also the shortcut *CTRL+ALT+5*

### Using Thunderlink

1. If you followed instructions you can then copy the thunderlink
   using *CTRL+ALT+5*
1. When creating new new task do so as normal BUT paste from clipboard directly into the *new task* textbox.
1. The software should automatically detect that this is a task from thunderbird

## Instructions

The software automatically loads the todo.txt upon startup. If none is found in the folder defined
in the INI file, the software ask to create one.
Also, the software automatically saves the file upon exit.

You can also set where the todo.txt should be by using *preferences*
This menu will although automatically start if no INI file was found.

### Creating tasks

Task can be created using *Edit > New task* where you can create a new task. You can also create
new *context* and new *project* using *Edit > New context*  and *Edit > New project*

### Changing tasks

By *right-clicking*  on the task you can set a number of features for the task, like *edit task*, *set context*, *set project*, *set priority*, *set due date*, *delete*

You can also set a task to completed by checking the task.

### sort the task

Task are by default sorted according to *context*, *project*, *priority*, *due date*
If needed the task can also be sorted by *priority* or *due date*.
