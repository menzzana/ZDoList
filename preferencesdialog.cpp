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
  ui->comboBox_2->addItem("No action");
  ui->comboBox_2->addItem("Delete");
  ui->comboBox_2->addItem("Archive");
  ui->comboBox_3->addItem("Context");
  ui->comboBox_3->addItem("Priority");
  ui->comboBox_3->addItem("Due date");
  ui->comboBox_3->addItem("Priority/days left");
  }
//------------------------------------------------------------------------------
PreferencesDialog::~PreferencesDialog() {
  delete ui;
  }
//------------------------------------------------------------------------------
void PreferencesDialog::setHideCompleted(bool hidecompleted) {
  ui->checkBox->setChecked(hidecompleted);
  }
//------------------------------------------------------------------------------
void PreferencesDialog::setSoftware(QStringList softwarelist,int softwareindex) {
  ui->comboBox->addItems(softwarelist);
  ui->comboBox->setCurrentIndex(softwareindex);
  }
//------------------------------------------------------------------------------
void PreferencesDialog::setCompletedTasks(int completedtasks) {
  ui->comboBox_2->setCurrentIndex(completedtasks);
  }
//------------------------------------------------------------------------------
void PreferencesDialog::setDeleteDays(int days) {
  ui->lineEdit_2->setText(QString::number(days));
  }
//------------------------------------------------------------------------------
void PreferencesDialog::setCollapsed(bool collapsed) {
  ui->checkBox_3->setChecked(collapsed);
  }
//------------------------------------------------------------------------------
void PreferencesDialog::setSortOrder(int sorttype) {
  ui->comboBox_3->setCurrentIndex(sorttype);
  }
//------------------------------------------------------------------------------
bool PreferencesDialog::getHideCompleted() {
  return ui->checkBox->checkState();
  }
//------------------------------------------------------------------------------
int PreferencesDialog::getSoftware() {
  return ui->comboBox->currentIndex();
  }
//------------------------------------------------------------------------------
int PreferencesDialog::getCompletedTasks() {
  return ui->comboBox_2->currentIndex();
  }
//------------------------------------------------------------------------------
int PreferencesDialog::getDeleteDays() {
  return ui->lineEdit_2->text().toInt();
  }
//------------------------------------------------------------------------------
int PreferencesDialog::getSortOrder() {
  return ui->comboBox_3->currentIndex();
  }
//------------------------------------------------------------------------------
bool PreferencesDialog::getCollapsed() {
  return ui->checkBox_3->checkState();
  }
//------------------------------------------------------------------------------
void PreferencesDialog::on_checkBox_stateChanged(int arg1) {
  ui->groupBox->setEnabled(arg1);
  }
//------------------------------------------------------------------------------

