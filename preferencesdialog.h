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
#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H
//------------------------------------------------------------------------------
#include <QDialog>
#include <qfiledialog.h>
//------------------------------------------------------------------------------
namespace Ui {
  class PreferencesDialog;
  }
//------------------------------------------------------------------------------
class PreferencesDialog : public QDialog {
  Q_OBJECT

  public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog();
    void setFilePath(QString filepath);
    void setHideCompleted(bool hidecompleted);
    void setSoftware(QStringList softwarelist,int softwareindex);
    void setCompletedTasks(int completedtasks);
    void setDeleteDays(int days);
    void setCollapsed(bool collapsed);
    QString getFilePath();
    bool getHideCompleted();
    int getSoftware();
    int getCompletedTasks();
    int getDeleteDays();
    bool getCollapsed();

    private:
    Ui::PreferencesDialog *ui;
  };
//------------------------------------------------------------------------------
#endif // PREFERENCESDIALOG_H
