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
#include "mainwindow.h"
#include "ui_mainwindow.h"
//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setAcceptDrops(true);
  ui->centralWidget->setBackgroundRole(QPalette::Dark);
  ui->scrollArea->takeWidget();
  ui->scrollArea->setWidgetResizable(true);
  taskWidget=new QWidget();
  taskWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  ui->scrollArea->setWidget(taskWidget);
  taskLayout=new QVBoxLayout();
  taskLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
  taskWidget->setLayout(taskLayout);
  project=context=nullptr;
  maintodo=new ToDo[MAXTASKS];
  nmaintodo=mailsoftware=daysdeletecompleted=0;
  default_sortorder=sortorder=SORTORDER::DEFAULT;
  todofilepath="";
  nonprioritized=false;
  archiving=true;
  default_collapsed=false;
  loadTasks();
  }
//------------------------------------------------------------------------------
MainWindow::~MainWindow() {
  delete project;
  delete context;
  delete ui;
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionExit_triggered() {
  close();
  }
//------------------------------------------------------------------------------
void MainWindow::checkAndSaveTasks() {
  ifstream fp(getFileName(TODO_FILENAME).c_str());
  if (fp.good())
    saveTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionNew_context_triggered() {
  bool ok;
  QString s1;
  ToDoTag *tdt1;

  s1=QInputDialog::getText(this,"New context","Name:",QLineEdit::Normal,s1,&ok);
  if (ok) {
    tdt1=addEntry(context,s1);
    if (context==nullptr)
      context=tdt1;
    }
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionNew_project_triggered() {
  bool ok;
  QString s1;
  ToDoTag *tdt1;

  s1=QInputDialog::getText(this,"New project","Name:",QLineEdit::Normal,s1,&ok);
  if (ok) {
    tdt1=addEntry(project,s1);
    if (project==nullptr)
      project=tdt1;
    }
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionNew_task_triggered() {
  addNewTask(nullptr);
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionPreferences_triggered() {
  preferences();
  }
//------------------------------------------------------------------------------
void MainWindow::dropEvent(QDropEvent *event) {
  QString s1;

  foreach(QUrl item,event->mimeData()->urls()) {
    s1=item.url();
    if (item.url().startsWith("imap"))
      s1="Mail (Thunderbird)";
    maintodo[nmaintodo].clear();
    maintodo[nmaintodo].description=s1;
    maintodo[nmaintodo].url=item.url();
    nmaintodo++;
    drawAllTasks();
    checkAndSaveTasks();
    cout << item.url().toStdString() << endl;
    }
  event->acceptProposedAction();
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionAbout_triggered() {
  QMessageBox msgbox;

  msgbox.setWindowTitle("ZDoList");
  msgbox.setTextFormat(Qt::RichText);
  msgbox.setText("<a href='https://github.com/menzzana/ZDoList'>ZDoList version "+QString(SOFTWARE_VERSION)+"-"+QString(GIT_VERSION)+"</a>");
  msgbox.setInformativeText("ZDoList is developed by Henric Zazzi");
  msgbox.setStandardButtons(QMessageBox::Ok);
  msgbox.exec();
  msgbox.close();
  }
//------------------------------------------------------------------------------
void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
  //if (event->mimeData()->hasFormat("text/plain"))
  event->acceptProposedAction();
  }
//------------------------------------------------------------------------------
void MainWindow::ShowContextMenu(const QPoint &pos,ToDo *todo) {
  QMenu *menu;
  QAction *action;

  menu=new QMenu();
  action=new QAction("Add subtask",this);
  menu->addAction(action);
  connect(action,&QAction::triggered, [=] {
      addNewTask(todo);
      }
    );
  action=new QAction("Edit task",this);
  menu->addAction(action);
  connect(action,&QAction::triggered, [=] {
      editTask(todo);
      }
    );
  action=new QAction("Set context",this);
  menu->addAction(action);
  connect(action,&QAction::triggered, [=] {
      setContext(todo);
      }
    );
  action=new QAction("Set project",this);
  menu->addAction(action);
  connect(action,&QAction::triggered, [=] {
      setProject(todo);
      }
    );
  action=new QAction("Set due date",this);
  menu->addAction(action);
  connect(action,&QAction::triggered, [=] {
      setDueDate(todo);
      }
    );
  action=new QAction("Set priority",this);
  menu->addAction(action);
  connect(action,&QAction::triggered, [=] {
      setPriority(todo);
      }
    );
  action=new QAction("Delete",this);
  menu->addAction(action);
  connect(action,&QAction::triggered, [=] {
      deleteTask(todo);
      }
    );
  menu->exec(this->mapToGlobal(pos));
  menu->close();
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionSort_by_context_triggered() {
  sortorder=SORTORDER::DEFAULT;
  drawAllTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionSort_by_priority_triggered() {
  sortorder=SORTORDER::PRIORITY;
  drawAllTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionSort_by_due_date_triggered() {
  sortorder=SORTORDER::DUEDATE;
  drawAllTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionSort_by_priority_days_left_triggered() {
  sortorder=SORTORDER::PRIORITYDAYSLEFT;
  drawAllTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionFilter_nonprioritized_triggered() {
  nonprioritized=ui->actionFilter_nonprioritized->isChecked();
  drawAllTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionCollapsed_triggered() {
  for (int i1=0; i1<nmaintodo; i1++)
    maintodo[i1].collapsed=true;
  drawAllTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionExpanded_triggered() {
  for (int i1=0; i1<nmaintodo; i1++)
    maintodo[i1].collapsed=false;
  drawAllTasks();
  }
//------------------------------------------------------------------------------
// User defined functions
//------------------------------------------------------------------------------
QString MainWindow::setTextColor(QString text,QString htmlfontcolor) {
 return "<font color=\""+htmlfontcolor+"\">"+text+"</font>";
  }
//------------------------------------------------------------------------------
string MainWindow::getFileName(QString filename) {
  return (todofilepath+filename).toStdString();
  }
//------------------------------------------------------------------------------
void MainWindow::preferences() {
  PreferencesDialog prefdialog;

  prefdialog.setFilePath(todofilepath);
  prefdialog.setSoftware(MAILSOFTWARE,mailsoftware);
  prefdialog.setArchiving(archiving);
  prefdialog.setDeleteDays(daysdeletecompleted);
  prefdialog.setCollapsed(default_collapsed);
  prefdialog.setSortOrder(default_sortorder);
  if (prefdialog.exec()==QDialog::Accepted) {
    todofilepath=prefdialog.getFilePath();
    mailsoftware=prefdialog.getSoftware();
    archiving=prefdialog.getArchiving();
    default_collapsed=prefdialog.getCollapsed();
    daysdeletecompleted=prefdialog.getDeleteDays();
    default_sortorder=prefdialog.getSortOrder();
    QSettings settings(QDir::currentPath()+INI_FILENAME, QSettings::IniFormat);
    settings.setValue("ToDoPath",todofilepath);
    settings.setValue("MailSoftware",mailsoftware);
    settings.setValue("Archiving",archiving);
    settings.setValue("DeleteDays",daysdeletecompleted);
    settings.setValue("Collapsed",default_collapsed);
    settings.setValue("DefaultSortOrder",default_sortorder);
    }
  prefdialog.close();
  }
//------------------------------------------------------------------------------
void MainWindow::checkEmptyToDoFile() {
  QMessageBox msgbox;

  ifstream fp(getFileName(TODO_FILENAME).c_str());
  if (!fp.good()) {
    msgbox.setText("Create Todo.txt file");
    msgbox.setInformativeText("File not found. Do you want to create a Task file?");
    msgbox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    if (msgbox.exec()==QMessageBox::Ok) {
      ofstream output(getFileName(TODO_FILENAME));
      ofstream output2(getFileName(DONE_FILENAME));
      }
    else
      close();
    }
  }
//------------------------------------------------------------------------------
void MainWindow::loadTasks() {
  QSettings settings(QDir::currentPath()+INI_FILENAME, QSettings::IniFormat);
  todofilepath=settings.value("ToDoPath",todofilepath).toString();
  mailsoftware=settings.value("MailSoftware",mailsoftware).toInt();
  archiving=settings.value("Archiving",archiving).toBool();
  default_collapsed=settings.value("Collapsed",default_collapsed).toBool();
  daysdeletecompleted=settings.value("DeleteDays",daysdeletecompleted).toInt();
  default_sortorder=settings.value("DefaultSortOrder",default_sortorder).toInt();
  if (todofilepath.isEmpty())
    preferences();
  checkEmptyToDoFile();
  sortorder=default_sortorder;
  nmaintodo=maintodo->load(getFileName(TODO_FILENAME),getFileName(DONE_FILENAME),&context,&project,archiving,daysdeletecompleted,default_collapsed);
  drawAllTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::saveTasks() {
  maintodo->save(getFileName(TODO_FILENAME),nmaintodo);
  }
//------------------------------------------------------------------------------
void MainWindow::addToDo(ToDo *todo,bool firstentry) {
  QFrame *frame;
  QVBoxLayout *vlayout;
  QHBoxLayout *hlayout;
  QLabel *label;
  QCheckBox *checkbox;
  QPushButton *button;
  QString duecolor,projecttext,framestyle;

  if (todo->collapsed && !firstentry)
    return;
  if (nonprioritized && todo->priority>0)
    return;
  frame=new QFrame();
  frame->setFrameShape(QFrame::Box);
  vlayout=new QVBoxLayout();
  frame->setLayout(vlayout);
  framestyle="color: black;";
  taskLayout->addWidget(frame);
  frame->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(frame,&QFrame::customContextMenuRequested,[=](const QPoint &pos) {
      ShowContextMenu(pos,todo);
      }
    );
  hlayout=addLayout(vlayout);
  if (todo->project!=nullptr) {
    framestyle="border-style: outset;border-color:"+getProjectColor(todo->project)+"; border-left-width: 4px;"+framestyle;
    if (firstentry && todo->project!=nullptr) {
      button=new QPushButton();
      button->setFixedWidth(15);
      button->setFixedHeight(15);
      button->setFont(QFont("Ubuntu",9));
      button->setText(todo->collapsed?"+":"-");
      connect(button,&QPushButton::clicked,[=] () {
          toggleCollapsed(todo);
          }
        );
      hlayout->addWidget(button);
      }
    }
  checkbox=new QCheckBox();
  checkbox->setFont(QFont("Ubuntu",9));
  if (todo->completed) {
    checkbox->setChecked(true);
    framestyle=framestyle+"background-color: lightgreen";
    }
  else
    framestyle=framestyle+"background-color: "+PRIORITY_COLOR.at(todo->priority);
  frame->setStyleSheet(".QFrame {"+framestyle+"}");
  checkbox->setText(todo->description);
  checkbox->setMaximumWidth(frame->width()-MAXLABELWIDTHDIFF);
  connect(checkbox,&QCheckBox::stateChanged,[=] () {
      setCompleted(checkbox,todo);
      }
    );
  hlayout->addWidget(checkbox);
  hlayout=addLayout(vlayout);
  if (todo->context!=nullptr) {
    label=new QLabel();
    label->setFont(QFont("Ubuntu",9));
    label->setText(setTextColor("@"+todo->context->description,"darkmagenta"));
    label->setMaximumWidth(frame->width()-MAXLABELWIDTHDIFF);
    hlayout->addWidget(label);
    }
  if (todo->project!=nullptr) {
    label=new QLabel();
    label->setFont(QFont("Ubuntu",9));
    projecttext=setTextColor("+"+todo->project->description,"darkblue");
    label->setText(projecttext);
    label->setMaximumWidth(frame->width()-MAXLABELWIDTHDIFF);
    hlayout->addWidget(label);
    }
  hlayout=addLayout(vlayout);
  if (todo->priority>0) {
    label=new QLabel();
    label->setFont(QFont("Ubuntu",9));
    label->setMaximumWidth(frame->width()-MAXLABELWIDTHDIFF);
    label->setText(setTextColor("<b>"+QString(char(todo->priority+64))+"</b>","black"));
    hlayout->addWidget(label);
    }
  if (todo->due.isValid()) {
    label=new QLabel();
    label->setFont(QFont("Ubuntu",9));
    if (todo->due<QDate::currentDate())
      duecolor="red";
    else
      duecolor="black";
    label->setText(setTextColor(todo->due.toString("yyyy-MM-dd"),duecolor));
    label->setMaximumWidth(frame->width()-MAXLABELWIDTHDIFF);
    hlayout->addWidget(label);
    }
  if (!todo->url.isEmpty()) {
    button=new QPushButton();
    button->setFont(QFont("Ubuntu",9));
    button->setText("Mail");
    hlayout->addWidget(button);
    connect(button,&QPushButton::clicked,[=] () {
        gotoMail(todo);
        }
      );
    }
  }
//------------------------------------------------------------------------------
QHBoxLayout *MainWindow::addLayout(QVBoxLayout *vlayout) {
  QFrame *frame;
  QHBoxLayout *hlayout;

  frame=new QFrame();
  frame->setStyleSheet(".QFrame {border-style: default;color: black;}");
  frame->setFrameShape(QFrame::NoFrame);
  hlayout=new QHBoxLayout();
  hlayout->setContentsMargins(0,0,0,0);
  frame->setLayout(hlayout);
  vlayout->addWidget(frame);
  return hlayout;
  }
//------------------------------------------------------------------------------
QString MainWindow::getProjectColor(ToDoTag *projecttag) {
  ToDoTag *tdt1;
  int i1;

  for (tdt1=project,i1=0; tdt1!=nullptr; tdt1=tdt1->Next,i1++)
    if (projecttag==tdt1)
      break;
  return PROJECT_COLOR.at(i1%PROJECT_COLOR.length());
  }
//------------------------------------------------------------------------------
void MainWindow::drawAllTasks() {
  QString projectname;

  while (QLayoutItem *item=taskLayout->takeAt(0)) {
    Q_ASSERT(!item->layout());
    delete item->widget();
    delete item;
    }
  ui->actionSort_by_context->setChecked(false);
  ui->actionSort_by_priority->setChecked(false);
  ui->actionSort_by_due_date->setChecked(false);
  ui->actionSort_by_priority_days_left->setChecked(false);
  switch (sortorder) {
    case SORTORDER::DEFAULT:
      ui->actionSort_by_context->setChecked(true);
      qsort(maintodo,static_cast<size_t>(nmaintodo),sizeof(ToDo),ToDo::compareTasks);
      break;
    case SORTORDER::PRIORITY:
      ui->actionSort_by_priority->setChecked(true);
      qsort(maintodo,static_cast<size_t>(nmaintodo),sizeof(ToDo),ToDo::compareTasksPriority);
      break;
    case SORTORDER::DUEDATE:
      ui->actionSort_by_due_date->setChecked(true);
      qsort(maintodo,static_cast<size_t>(nmaintodo),sizeof(ToDo),ToDo::compareTasksDueDate);
      break;
    case SORTORDER::PRIORITYDAYSLEFT:
      ui->actionSort_by_priority_days_left->setChecked(true);
      qsort(maintodo,static_cast<size_t>(nmaintodo),sizeof(ToDo),ToDo::compareTasksPriorityDaysLeft);
      break;
    }
  for (int i1=0; i1<nmaintodo; i1++)
    if (maintodo[i1].project!=nullptr)
      for (int i2=i1+1; i2<nmaintodo; i2++)
        if (maintodo[i1].getProjectName().compare(maintodo[i2].getProjectName())==0) {
          shiftInsert(maintodo,i2,i1+1);
          break;
          }
  projectname="";
  for (int i1=0; i1<nmaintodo; i1++) {
    bool b1=maintodo[i1].getProjectName().compare(projectname)!=0;
    if (maintodo[i1].getProjectName().isEmpty())
      b1=true;
    addToDo(&maintodo[i1],b1);
    projectname=maintodo[i1].getProjectName();
    }
  }
//------------------------------------------------------------------------------
void MainWindow::gotoMail(ToDo *todo) {
  string urltype;

  switch(mailsoftware) {
    case 0:
      urltype="thunderbird -mail";
      if (todo->url.toString().startsWith("thunderlink:"))
        urltype="thunderbird -thunderlink";
      break;
    }
  system((urltype+" "+todo->url.toString().toStdString()).c_str());
  }
//------------------------------------------------------------------------------
void MainWindow::setProject(ToDo *todo) {
  bool ok;
  QStringList stringlist;
  QString s1;
  ToDoTag *tdt1;
  int idx,currentidx;

  stringlist << QString("None");
  currentidx=0;
  for (tdt1=project,idx=1; tdt1!=nullptr; tdt1=tdt1->Next,idx++) {
    stringlist << QString(tdt1->description);
    if (tdt1==todo->project)
      currentidx=idx;
    }
  s1=QInputDialog::getItem(this,"Set Project","Project",stringlist,currentidx,false,&ok);
  if (ok) {
    todo->project=getEntry(project,s1);
    drawAllTasks();
    checkAndSaveTasks();
    }
  }
//------------------------------------------------------------------------------
void MainWindow::setContext(ToDo *todo) {
  bool ok;
  QStringList stringlist;
  QString s1;
  ToDoTag *tdt1;
  int idx,currentidx;

  stringlist << QString("None");
  currentidx=0;
  for (tdt1=context,idx=1; tdt1!=nullptr; tdt1=tdt1->Next,idx++) {
    stringlist << QString(tdt1->description);
    if (tdt1==todo->context)
      currentidx=idx;
    }
  s1=QInputDialog::getItem(this,"Set Context","Context",stringlist,currentidx,false,&ok);
  if (ok) {
    todo->context=getEntry(context,s1);
    drawAllTasks();
    checkAndSaveTasks();
    }
  }
//------------------------------------------------------------------------------
void MainWindow::editTask(ToDo *todo) {
  bool ok;
  QString s1;

  s1=todo->description;
  s1=QInputDialog::getText(this,"Edit Task","Description:",QLineEdit::Normal,s1,&ok);
  if (ok) {
    todo->description=s1;
    drawAllTasks();
    checkAndSaveTasks();
    }
  }
//------------------------------------------------------------------------------
void MainWindow::setPriority(ToDo *todo) {
  bool ok;
  QStringList stringlist;
  QString s1;

  stringlist << QString("None");
  for (int i1=1; i1<MAXPRIORITY; i1++)
    stringlist << QString(char(i1+64));
  s1=QInputDialog::getItem(this,"Set Context","Context",stringlist,todo->priority,false,&ok);
  if (ok) {
    todo->priority=stringlist.indexOf(s1);
    drawAllTasks();
    checkAndSaveTasks();
    }
  }
//------------------------------------------------------------------------------
void MainWindow::setDueDate(ToDo *todo) {
  DateDialog datedialog;

  datedialog.setDueDate(todo->due);
  if (datedialog.exec()==QDialog::Accepted)
    todo->due=datedialog.getDueDate();
  else
    todo->due.setDate(0,0,0);
  drawAllTasks();
  checkAndSaveTasks();
  datedialog.close();
  }
//------------------------------------------------------------------------------
void MainWindow::setCompleted(QCheckBox *checkbox,ToDo *todo) {
  todo->completed=checkbox->isChecked();
  todo->completion=QDate::currentDate();
  drawAllTasks();
  checkAndSaveTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::toggleCollapsed(ToDo *todo) {
  for (int i1=0; i1<nmaintodo; i1++)
    if (maintodo[i1].getProjectName().compare(todo->getProjectName())==0)
      maintodo[i1].collapsed=!maintodo[i1].collapsed;
  drawAllTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::deleteTask(ToDo *todo) {
  bool found;

  found=false;
  for (int i1=0; i1<nmaintodo; i1++) {
    if (todo==&maintodo[i1])
      found=true;
    if (found)
      maintodo[i1]=maintodo[i1+1];
    }
  maintodo[nmaintodo].clear();
  nmaintodo--;
  drawAllTasks();
  checkAndSaveTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::addNewTask(ToDo *todo) {
  bool ok;
  QString s1;
  QStringList sl1,sl2;
  int currententries;

  s1=QInputDialog::getText(this,"New task","Description:",QLineEdit::Normal,s1,&ok);
  if (ok && !s1.isEmpty()) {
    currententries=nmaintodo;
    switch (mailsoftware) {
      case THUNDERBIRD:
        if (s1.startsWith("<A HREF=")) {
          sl1=s1.split(" / ");
          for (int i1=0; i1<sl1.size(); i1++)  {
            sl2=sl1.at(i1).split("\">");
            addTaskProject(&maintodo[nmaintodo],todo,sl2.at(1).mid(0,sl2.at(1).indexOf('<')),sl2.at(0).mid(9,-1));
            nmaintodo++;
            }
          }
        break;
      }
    if (currententries==nmaintodo) {
      addTaskProject(&maintodo[nmaintodo],todo,s1,nullptr);
      nmaintodo++;
      }
    drawAllTasks();
    checkAndSaveTasks();
    }
  }
//------------------------------------------------------------------------------
void MainWindow::addTaskProject(ToDo *todo,ToDo *todoproject,QString description,QString url) {
  ToDoTag *pr1;
  QMessageBox msgbox;

  todo->clear();
  todo->description=description;
  todo->url=url;
  if (todoproject==nullptr)
    return;
  if (todoproject->project!=nullptr) {
    todo->project=todoproject->project;
    todo->context=todoproject->context;
    return;
    }
  for (pr1=project; pr1!=nullptr; pr1=pr1->Next)
    if (pr1->description.compare(todoproject->description)==0) {
      msgbox.setWindowTitle("Error");
      msgbox.setText("Project does already exists");
      msgbox.setInformativeText("Creating as normal task. Manually assign project");
      msgbox.setStandardButtons(QMessageBox::Ok);
      msgbox.exec();
      msgbox.close();
      return;
      }
  pr1=addEntry(project,todoproject->description);
  if (project==nullptr)
    project=pr1;
  todoproject->project=pr1;
  todo->project=pr1;
  todo->context=todoproject->context;
  }
//------------------------------------------------------------------------------
