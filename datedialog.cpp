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
#include "datedialog.h"
#include "ui_datedialog.h"
//------------------------------------------------------------------------------
DateDialog::DateDialog(QWidget *parent) : QDialog(parent), ui(new Ui::DateDialog) {
  ui->setupUi(this);
  }
//------------------------------------------------------------------------------
DateDialog::~DateDialog() {
  delete ui;
  }
//------------------------------------------------------------------------------
void DateDialog::setDueDate(QDate date) {
  if (date.isValid())
    ui->calendarWidget->setSelectedDate(date);
  }
//------------------------------------------------------------------------------
QDate DateDialog::getDueDate() {
  return ui->calendarWidget->selectedDate();
  }
//------------------------------------------------------------------------------
