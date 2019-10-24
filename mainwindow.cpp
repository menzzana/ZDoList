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
  sortorder=SORTORDER::DEFAULT;
  todofilename="";
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
void MainWindow::closeEvent(QCloseEvent *event) {
  ifstream fp(todofilename.toStdString().c_str());
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
    tdt1->description=s1;
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
    tdt1->description=s1;
    }
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionNew_task_triggered() {
  bool ok;
  QString s1;
  QStringList sl1,sl2;

  s1=QInputDialog::getText(this,"New task","Description:",QLineEdit::Normal,s1,&ok);
  if (ok) {
    if (s1.startsWith("thunderlink:")) {
      sl1=s1.split(" / ");
      for (int i1=0; i1<sl1.size(); i1++)  {
        sl2=sl1.at(i1).split("|");
        maintodo[nmaintodo].clear();
        maintodo[nmaintodo].description=sl2.at(1);
        maintodo[nmaintodo].url=sl2.at(0);
        addToDo(&maintodo[nmaintodo]);
        nmaintodo++;
        }
      return;
      }
    maintodo[nmaintodo].clear();
    maintodo[nmaintodo].description=s1;
    addToDo(&maintodo[nmaintodo]);
    nmaintodo++;
    drawAllTasks();
    }
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
    addToDo(&maintodo[nmaintodo]);
    nmaintodo++;
    cout << item.url().toStdString() << endl;
    }
  event->acceptProposedAction();
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionAbout_triggered() {
  QMessageBox msgbox;

  msgbox.setWindowTitle("ZDoList");
  msgbox.setText("ZDoList version "+QString(SOFTWARE_VERSION)+"-"+QString(GIT_VERSION));
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
  ui->actionSort_by_context->setChecked(true);
  ui->actionSort_by_priority->setChecked(false);
  ui->actionSort_by_due_date->setChecked(false);
  sortorder=SORTORDER::DEFAULT;
  drawAllTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionSort_by_priority_triggered() {
  ui->actionSort_by_context->setChecked(false);
  ui->actionSort_by_priority->setChecked(true);
  ui->actionSort_by_due_date->setChecked(false);
  sortorder=SORTORDER::PRIORITY;
  drawAllTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::on_actionSort_by_due_date_triggered() {
  ui->actionSort_by_context->setChecked(false);
  ui->actionSort_by_priority->setChecked(false);
  ui->actionSort_by_due_date->setChecked(true);
  sortorder=SORTORDER::DUEDATE;
  drawAllTasks();
  }
//------------------------------------------------------------------------------
// User defined functions
//------------------------------------------------------------------------------
QString MainWindow::setTextColor(QString text,QString htmlfontcolor) {
 return "<font color=\""+htmlfontcolor+"\">"+text+"</font>";
  }
//------------------------------------------------------------------------------
void MainWindow::preferences() {
  PreferencesDialog prefdialog;

  prefdialog.setFileName(TODO_FILENAME,todofilename);
  prefdialog.setSoftware(MAILSOFTWARE,mailsoftware);
  prefdialog.setDeleteDays(daysdeletecompleted);
  if (prefdialog.exec()==QDialog::Accepted) {
    todofilename=prefdialog.getFileName();
    mailsoftware=prefdialog.getSoftware();
    daysdeletecompleted=prefdialog.getDeleteDays();
    QSettings settings(QDir::currentPath()+INI_FILENAME, QSettings::IniFormat);
    settings.setValue("ToDoFile",todofilename);
    settings.setValue("MailSoftware",mailsoftware);
    settings.setValue("DeleteDays",daysdeletecompleted);
    }
  prefdialog.close();
  }
//------------------------------------------------------------------------------
void MainWindow::checkEmptyToDoFile() {
  QMessageBox msgbox;

  ifstream fp(todofilename.toStdString().c_str());
  if (!fp.good()) {
    msgbox.setText("Create Todo.txt file");
    msgbox.setInformativeText("File not found. Do you want to create a Task file?");
    msgbox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    if (msgbox.exec()==QMessageBox::Ok)
      ofstream output(todofilename.toStdString());
    else
      close();
    }
  }
//------------------------------------------------------------------------------
void MainWindow::loadTasks() {
  QSettings settings(QDir::currentPath()+INI_FILENAME, QSettings::IniFormat);
  todofilename=settings.value("ToDoFile","").toString();
  mailsoftware=settings.value("MailSoftware",0).toInt();
  daysdeletecompleted=settings.value("DeleteDays",0).toInt();
  if (todofilename.isEmpty())
    preferences();
  checkEmptyToDoFile();
  nmaintodo=maintodo->load(todofilename,&context,&project,daysdeletecompleted);
  drawAllTasks();
  }
//------------------------------------------------------------------------------
void MainWindow::saveTasks() {
  maintodo->save(todofilename,nmaintodo);
  }
//------------------------------------------------------------------------------
void MainWindow::addToDo(ToDo *todo) {
  QFrame *frame;
  QGridLayout *myLayout;
  QLabel *label;
  QCheckBox *checkbox;
  QPushButton *button;
  QString duecolor;
  int row;

  frame=new QFrame();
  frame->setFrameShape(QFrame::Box);
  myLayout=new QGridLayout();
  frame->setLayout(myLayout);
  taskLayout->addWidget(frame);
  frame->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(frame,&QFrame::customContextMenuRequested,[=](const QPoint &pos) {
      ShowContextMenu(pos,todo);
      }
    );
  row=0;
  checkbox=new QCheckBox();
  checkbox->setFont(QFont("Ubuntu",9));
  if (todo->completed) {
    checkbox->setChecked(true);
    frame->setStyleSheet("color: black; background-color: lightgreen");
    }
  else
    frame->setStyleSheet("color: black; background-color: "+PRIORITY_COLOR.at(todo->priority));
  checkbox->setText(todo->description);
  checkbox->setMaximumWidth(frame->width()-MAXLABELWIDTHDIFF);
  connect(checkbox,&QCheckBox::stateChanged,[=] () {
      setCompleted(checkbox,todo);
      }
    );
  myLayout->addWidget(checkbox,row,0,1,3);
  row++;
  if (todo->context!=nullptr) {
    label=new QLabel();
    label->setFont(QFont("Ubuntu",9));
    label->setText(setTextColor("@"+todo->context->description,"darkmagenta"));
    label->setMaximumWidth(frame->width()-MAXLABELWIDTHDIFF);
    myLayout->addWidget(label,row,0,1,1);
    }
  if (todo->project!=nullptr) {
    label=new QLabel();
    label->setFont(QFont("Ubuntu",9));
    label->setText(setTextColor("+"+todo->project->description,"darkblue"));
    label->setMaximumWidth(frame->width()-MAXLABELWIDTHDIFF);
    myLayout->addWidget(label,row,1,1,2);
    }
  if (todo->context!=nullptr || todo->project!=nullptr)
    row++;
  if (todo->due.isValid()) {
    label=new QLabel();
    label->setFont(QFont("Ubuntu",9));
    if (todo->due<QDate::currentDate())
      duecolor="red";
    else
      duecolor="black";
    label->setText(setTextColor(todo->due.toString("yyyy-MM-dd"),duecolor));
    label->setMaximumWidth(frame->width()-MAXLABELWIDTHDIFF);
    myLayout->addWidget(label,row,0,1,1);
    }
  if (todo->priority>0) {
    label=new QLabel();
    label->setFont(QFont("Ubuntu",9));
    label->setMaximumWidth(frame->width()-MAXLABELWIDTHDIFF);
    label->setText(setTextColor(QString(char(todo->priority+64)),"black"));
    myLayout->addWidget(label,row,2,1,1);
    }
  if (!todo->url.isEmpty()) {
    button=new QPushButton();
    button->setFont(QFont("Ubuntu",9));
    button->setMaximumWidth(frame->width()-MAXLABELWIDTHDIFF);
    button->setText("Mail");
    myLayout->addWidget(button,row,1,1,1);
    connect(button,&QPushButton::clicked,[=] () {
        gotoMail(todo);
        }
      );
    }
  }
//------------------------------------------------------------------------------
void MainWindow::drawAllTasks() {
  while (QLayoutItem *item=taskLayout->takeAt(0)) {
    Q_ASSERT(!item->layout());
    delete item->widget();
    delete item;
    }
  switch (sortorder) {
    case SORTORDER::DEFAULT:
      qsort(maintodo,static_cast<size_t>(nmaintodo),sizeof(ToDo),ToDo::compareTasks);
      break;
    case SORTORDER::PRIORITY:
      qsort(maintodo,static_cast<size_t>(nmaintodo),sizeof(ToDo),ToDo::compareTasksPriority);
      break;
    case SORTORDER::DUEDATE:
      qsort(maintodo,static_cast<size_t>(nmaintodo),sizeof(ToDo),ToDo::compareTasksDueDate);
      break;
    }
  for (int i1=0; i1<nmaintodo; i1++)
    addToDo(&maintodo[i1]);
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
  datedialog.close();
  }
//------------------------------------------------------------------------------
void MainWindow::setCompleted(QCheckBox *checkbox,ToDo *todo) {
  todo->completed=checkbox->isChecked();
  todo->completion=QDate::currentDate();
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
      memcpy(&maintodo[i1],&maintodo[i1+1],sizeof(ToDo));
    }
  nmaintodo--;
  drawAllTasks();
  }
//------------------------------------------------------------------------------

