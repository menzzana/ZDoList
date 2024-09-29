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
#include "paramdialog.h"
#include "qlineedit.h"
#include "ui_paramdialog.h"
//------------------------------------------------------------------------------
ParamDialog::ParamDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ParamDialog) {
  ui->setupUi(this);
  }
//------------------------------------------------------------------------------
ParamDialog::~ParamDialog() {
  delete ui;
  }
//------------------------------------------------------------------------------
void ParamDialog::setText(QString text) {
  ui->label->setText(text);
  }
 //------------------------------------------------------------------------------
void ParamDialog::setTags(ToDoTag *paramtag,ToDo *todo) {
  ToDoTag *pt1;
  QStringList stringlist;
  int idx,currentidx;
  QFont comboFont;

  currentidx=0;
  for (pt1=paramtag,idx=1; pt1!=nullptr; pt1=pt1->Next,idx++) {
    stringlist << QString(pt1->description);
    if (pt1==todo->context)
      currentidx=idx;
    }
  stringlist << QString("New entry");
  ui->comboBox->addItems(stringlist);
  if (currentidx>0)
    ui->comboBox->setCurrentIndex(currentidx);
  idx=ui->comboBox->count()-1;
  comboEditStart=false;
  comboFont.setItalic(true);
  ui->comboBox->setItemData(ui->comboBox->count() - 1, comboFont, Qt::FontRole);
  if (ui->comboBox->count()==1) {
    ui->comboBox->setEditable(true);
    ui->comboBox->setEditText("");
    }
  connect(ui->comboBox, &QComboBox::currentIndexChanged, this, [=] {
    ui->comboBox->setEditable(ui->comboBox->currentIndex()==ui->comboBox->count()-1);
    if (ui->comboBox->currentIndex()==ui->comboBox->count()-1)
      ui->comboBox->setEditText("");
    });
  }
//------------------------------------------------------------------------------
QString ParamDialog::getToDoText() {
  return ui->comboBox->currentText();
  }
//------------------------------------------------------------------------------
