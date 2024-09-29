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
#ifndef PARAMDIALOG_H
#define PARAMDIALOG_H
//------------------------------------------------------------------------------
#include <QDialog>
#include "todotag.h"
#include "todo.h"
//------------------------------------------------------------------------------
namespace Ui {
  class ParamDialog;
  }
//------------------------------------------------------------------------------
class ParamDialog : public QDialog {
  Q_OBJECT

  public:
  explicit ParamDialog(QWidget *parent = nullptr);
    ~ParamDialog();
    void setText(QString text);
    void setTags(ToDoTag *paramtag,ToDo *todo);
    QString getToDoText();

  private:
  Ui::ParamDialog *ui;
  bool comboEditStart;
  };
//------------------------------------------------------------------------------
#endif // PARAMDIALOG_H
