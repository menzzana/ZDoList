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
#ifndef GLOBAL_H
#define GLOBAL_H
//------------------------------------------------------------------------------
#include <stdio.h>
#include <iostream>
#include <QString>
#include <QStringList>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
// Global constants
//------------------------------------------------------------------------------
const int MAXLABELWIDTHDIFF=10;
const int MAXMAILBUTTONWIDTH=100;
const int MAXPRIORITY=5;
const int DAYSPERPRIO=7;
const unsigned int MAXTASKS=1000;
const QString ZDOLIST_FOLDER="/.zdolist";
const QString TODO_FILENAME="/todo.txt";
const QString DONE_FILENAME="/done.txt";
const QString INI_FILENAME="/ZDoList.ini";
const QStringList PRIORITY_COLOR={
  "white","#ffafaf","#ffc0a0","#ffd180","#ffe299","#ffffb3"
  };
const QStringList PROJECT_COLOR={
  "Maroon","Olive","Teal","Navy","Purple","Silver","Aqua","Gray","Blue","DarkMagenta"
  };
const QStringList MAILSOFTWARE={
  "Thunderbird"
  };
enum MAILORDER {THUNDERBIRD};
enum SORTORDER {DEFAULT,PRIORITY,DUEDATE,PRIORITYDAYSLEFT};
//------------------------------------------------------------------------------
template<typename T> T *addEntry(T *first,QString description) {
  T *tl1,*tl2;

  for (tl1=tl2=first; tl1!=nullptr; tl2=tl1,tl1=tl1->Next)
    if (description.compare(tl1->description)==0)
      return tl1;
  tl1=new T();
  if (tl2!=nullptr)
    tl2->Next=tl1;
  tl1->description=description;
  return tl1;
  }
//------------------------------------------------------------------------------
template<typename T> T *getEntry(T *first,QString description) {
  T *tl1;

  for (tl1=first; tl1!=nullptr; tl1=tl1->Next)
    if (description.compare(tl1->description)==0)
      return tl1;
  return nullptr;
  }
//------------------------------------------------------------------------------
template<typename T> void shiftInsert(T *first,int src,int dest) {
  T tl;

  if (dest==src)
    return;
  tl=first[src];
  for (int i1=src-1; i1>=dest; i1--)
    first[i1+1]=first[i1];
  first[dest]=tl;
  }
//------------------------------------------------------------------------------
#endif // GLOBAL_H
