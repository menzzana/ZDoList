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
#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
//------------------------------------------------------------------------------
PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent),ui(new Ui::PreferencesDialog) {
  ui->setupUi(this);
  }
//------------------------------------------------------------------------------
PreferencesDialog::~PreferencesDialog() {
  delete ui;
  }
//------------------------------------------------------------------------------
void PreferencesDialog::on_pushButton_clicked() {
  QString s1;
  QFileDialog filedialog;

  s1=filedialog.getExistingDirectory(this,tr("Choose a directory"),"/home",
                                               QFileDialog::ShowDirsOnly | QFileDialog::ReadOnly);
  if (s1.isEmpty())
    return;
  ui->lineEdit->setText(s1+filename);
  }
//------------------------------------------------------------------------------
void PreferencesDialog::setFileName(QString filename,QString fullfilename) {
  this->filename=filename;
  ui->lineEdit->setText(fullfilename);
  }
//------------------------------------------------------------------------------
void PreferencesDialog::setSoftware(QStringList softwarelist,int softwareindex) {
  ui->comboBox->addItems(softwarelist);
  ui->comboBox->setCurrentIndex(softwareindex);
  }
//------------------------------------------------------------------------------
QString PreferencesDialog::getFileName() {
  return ui->lineEdit->text();
  }
//------------------------------------------------------------------------------
int PreferencesDialog::getSoftware() {
  return ui->comboBox->currentIndex();
  }
//------------------------------------------------------------------------------
