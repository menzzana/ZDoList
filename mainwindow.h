/*******************************************************************************
ZDoList
Copyright (C) 2019  Henric Zazzi <henric@zazzi.se>

ZDoList is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//------------------------------------------------------------------------------
#include <QMainWindow>
#include "global.h"
#include <stdlib.h>
#include <QScrollArea>
#include <QInputDialog>
#include <QMessageBox>
#include <QComboBox>
#include "datedialog.h"
#include "preferencesdialog.h"
#include <qfiledialog.h>
#include <QSettings>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QDir>
#include <QCheckBox>
#include "todo.h"
#include "todotag.h"
//------------------------------------------------------------------------------
namespace Ui {
  class MainWindow;
  }
//------------------------------------------------------------------------------
class MainWindow : public QMainWindow {
  Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void preferences();
    void checkEmptyToDoFile();
    void loadTasks();
    void saveTasks();
    void addToDo(ToDo *todo);
    void drawAllTasks();
    void gotoMail(ToDo *todo);
    void setProject(ToDo *todo);
    void setContext(ToDo *todo);
    void editTask(ToDo *todo);
    void setPriority(ToDo *todo);
    void setDueDate(ToDo *todo);
    void deleteTask(ToDo *todo);
    void setCompleted(QCheckBox *checkbox,ToDo *todo);
    QString setTextColor(QString text,QString htmlfontcolor);

  private slots:
    void on_actionExit_triggered();
    void on_actionNew_context_triggered();
    void on_actionNew_project_triggered();
    void on_actionNew_task_triggered();
    void on_actionPreferences_triggered();
    void on_actionAbout_triggered();
    void closeEvent(QCloseEvent *event);
    void ShowContextMenu(const QPoint &pos,ToDo *todo);
    void on_actionSort_by_context_triggered();
    void on_actionSort_by_priority_triggered();
    void on_actionSort_by_due_date_triggered();
    void on_actionFilter_nonprioritized_triggered();

private:
    const int MAXLABELWIDTHDIFF=10;
    const int MAXMAILBUTTONWIDTH=100;
    const int MAXPRIORITY=5;
    const unsigned int MAXTASKS=1000;
    const QString TODO_FILENAME="/todo.txt";
    const QString INI_FILENAME="/ZDoList.ini";
    const QStringList PRIORITY_COLOR={
      "white","#ffafaf","#f0bfbf","#f0cfcf","#f0dfdf","#ffefef"
      };
    const QStringList MAILSOFTWARE={
      "Thunderbird"
      };
    enum SORTORDER {DEFAULT,PRIORITY,DUEDATE};
    Ui::MainWindow *ui;
    ToDo *maintodo;
    int nmaintodo,mailsoftware,sortorder,daysdeletecompleted;
    bool nonprioritized;
    ToDoTag *project,*context;
    QString todofilename;
    QWidget *taskWidget;
    QVBoxLayout *taskLayout;
  };
//------------------------------------------------------------------------------
#endif // MAINWINDOW_H
