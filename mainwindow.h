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
#include "paramdialog.h"
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
    void addToDo(ToDo *todo,bool firstentry);
    QHBoxLayout *addLayout(QVBoxLayout *vlayout);
    QString getProjectColor(ToDoTag *projecttag);
    void drawAllTasks();
    void gotoMail(ToDo *todo);
    void setProject(ToDo *todo);
    void setContext(ToDo *todo);
    void editTask(ToDo *todo);
    void setPriority(ToDo *todo);
    void setDueDate(ToDo *todo);
    void deleteTask(ToDo *todo);
    void addNewTask(ToDo *todo);
    void addTaskProject(ToDo *todo,ToDo *todoproject,QString description,QString url);
    void setCompleted(QCheckBox *checkbox,ToDo *todo);
    void toggleCollapsed(ToDo *todo);
    QString setTextColor(QString text,QString htmlfontcolor);
    string getFileName(QString filename);
    void copyMailFile(QString filename);
    void deleteMailFile(QString filename);

  private slots:
    void on_actionExit_triggered();
    void on_actionNew_task_triggered();
    void on_actionPreferences_triggered();
    void on_actionAbout_triggered();
    void checkAndSaveTasks();
    void ShowContextMenu(const QPoint &pos,ToDo *todo);
    void on_actionFilter_nonprioritized_triggered();
    void on_actionCollapsed_triggered();
    void on_actionExpanded_triggered();
    void on_actionsortcontext_triggered();
    void on_actionsortpriority_triggered();
    void on_actionsortdue_date_triggered();
    void on_actionsortpriority_days_left_triggered();

    private:
    Ui::MainWindow *ui;
    ToDo *maintodo;
    int nmaintodo,mailsoftware,sortorder,default_sortorder,completedtasks,daysdeletecompleted;
    bool hidecompleted,nonprioritized,default_collapsed;
    const int HEIGHTOFFSET=200;
    const int WIDTHOFFSET=130;
    ToDoTag *project,*context;
    QString todofilepath;
    QWidget *taskWidget;
    QVBoxLayout *taskLayout;
  };
//------------------------------------------------------------------------------
#endif // MAINWINDOW_H
