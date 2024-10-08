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

## Instructions

The software automatically loads the todo.txt upon startup. If none is found in the folder defined
in the INI file, the software ask to create one.
Also, the software automatically saves the file upon exit.

### Creating tasks

Task can be created using *Edit > New task* where you can create a new task.
Task can also be created by drag-dropping mails from mail clients.

### Changing tasks

By *right-clicking*  on the task you can set a number of features for the task, like *edit task*, *set context*, *set project*, *set priority*, *set due date*, *delete*
You can also create project and context directly when setting these items.

You can also set a task to completed by checking the task.

### sort the task

Task are by default sorted according to *context*, *project*, *priority*, *due date*
If needed the task can also be sorted by *priority* or *due date*.
