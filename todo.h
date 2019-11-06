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
#ifndef TODO_H
#define TODO_H
//------------------------------------------------------------------------------
#include "global.h"
#include "todotag.h"
#include <iostream>
#include <fstream>
#include <string>
#include <QString>
#include <QDate>
#include <QUrl>
//------------------------------------------------------------------------------
class ToDo {
  public:
    bool completed;
    int priority;
    QString description;
    QUrl url;
    ToDoTag *context;
    ToDoTag *project;
    QDate due,completion,creation;

    ToDo();
    void clear();
    void save(string filename,int length);
    QString getTaskValue(string fstr,int start,char end);
    int load(string filename,string donefile,ToDoTag **context,ToDoTag **project,bool archiving,int daysdeletecompleted);
    static int compareTasks(const void *a,const void *b);
    static int compareTasksPriority(const void *a,const void *b);
    static int compareTasksDueDate(const void *a,const void *b);
  };
//------------------------------------------------------------------------------
#endif // TODO_H
