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
#include "todo.h"
//------------------------------------------------------------------------------
ToDo::ToDo() {
  clear();
  }
//------------------------------------------------------------------------------
void ToDo::clear() {
  completed=collapsed=false;
  priority=0;
  description="";
  context=nullptr;
  project=nullptr;
  url="";
  due.setDate(0,0,0);
  completion.setDate(0,0,0);
  creation=QDate::currentDate();
  }
//------------------------------------------------------------------------------
void ToDo::save(string filename,int length) {
  ofstream wout;
  string firstspace;

  wout.open(filename.c_str(),ofstream::out);
  for (int i1=0; i1<length; i1++) {
    firstspace="";
    if (this[i1].completed) {
      wout << "x";
      firstspace=" ";
      }
    if (this[i1].priority>0) {
      wout << firstspace << "(" << char(this[i1].priority+64) << ")";
      firstspace=" ";
      }
    if (this[i1].completed && this[i1].completion.isValid()) {
      wout << firstspace << this[i1].completion.toString("yyyy-MM-dd").toStdString();
      firstspace=" ";
      }
    wout << firstspace << this[i1].creation.toString("yyyy-MM-dd").toStdString();
    wout << " " << this[i1].description.toStdString();
    if (this[i1].context!=nullptr)
      wout << " @\'" << this[i1].context->description.toStdString() << "\'";
    if (this[i1].project!=nullptr)
      wout << " +\'" << this[i1].project->description.toStdString() << "\'";
    if (this[i1].due.isValid())
      wout << " due:" << this[i1].due.toString("yyyy-MM-dd").toStdString();
    if (!this[i1].url.isEmpty())
      wout << " url:\'" << this[i1].url.toStdString() << "\'";
    wout << endl;
    }
  wout.close();
  }
//------------------------------------------------------------------------------
QString ToDo::getTaskValue(string fstr,int start,char end) {
  return QString::fromStdString(fstr.substr(start,fstr.find_first_of(end,start)-start));
  }
//------------------------------------------------------------------------------
int ToDo::load(string filename,string donefile,ToDoTag **context,ToDoTag **project,bool archiving,int daysdeletecompleted,bool collapsed) {
  const int TODOTAGLENGTH=4;
  string TODOTAG[TODOTAGLENGTH]={
    " @'"," +'"," due:"," url:"
    };
  ToDoTag *tdt1;
  ifstream fin;
  ofstream wout;
  string fstr;
  QString s1;
  int spacechar,length,minposition,pos;
  QDate date1;

  length=0;
  fin.open(filename.c_str());
  wout.open(donefile.c_str(),ofstream::out|ofstream::app);
  while (getline(fin,fstr)) {
    if (fstr.length()==0)
      break;
    spacechar=0;
    this[length].collapsed=collapsed;
    if (fstr.substr(spacechar,2)=="x ") {
      this[length].completed=true;
      spacechar=2;
      }
    if (fstr.at(spacechar)=='(' && fstr.substr(spacechar+2,2)==") ") {
      this[length].priority=static_cast<int>(fstr.at(spacechar+1))-64;
      spacechar=spacechar+4;
      }
    s1=getTaskValue(fstr,spacechar,' ');
    date1=QDate::fromString(s1,"yyyy-MM-dd");
    if (date1.isValid()) {
      this[length].creation=date1;
      spacechar=spacechar+11;
      }
    s1=getTaskValue(fstr,spacechar,' ');
    date1=QDate::fromString(s1,"yyyy-MM-dd");
    if (date1.isValid() && this[length].completed) {
      this[length].completion=this[length].creation;
      this[length].creation=date1;
      spacechar=spacechar+11;
      if (this[length].completion.addDays(daysdeletecompleted)<QDate::currentDate() && daysdeletecompleted>0) {
        if (archiving)
          wout << fstr << endl;
        continue;
        }
      }
    minposition=fstr.length();
    for (int i1=0; i1<TODOTAGLENGTH; i1++) {
      pos=fstr.find(TODOTAG[i1],spacechar);
      if (pos==string::npos)
        continue;
      minposition=min(minposition,static_cast<int>(pos));
      }
    this[length].description=QString::fromStdString(fstr.substr(spacechar,minposition-spacechar));
    spacechar=minposition;
    if (fstr.substr(spacechar,2)==" @") {
      s1=getTaskValue(fstr,spacechar+3,'\'');
      tdt1=addEntry(*context,s1);
      if (*context==nullptr)
        *context=tdt1;
      tdt1->description=s1;
      this[length].context=tdt1;
      spacechar=spacechar+s1.length()+4;
      }
    if (fstr.substr(spacechar,2)==" +") {
      s1=getTaskValue(fstr,spacechar+3,'\'');
      tdt1=addEntry(*project,s1);
      if (*project==nullptr)
        *project=tdt1;
      tdt1->description=s1;
      this[length].project=tdt1;
      spacechar=spacechar+s1.length()+4;
      }
    if (fstr.substr(spacechar,5)==" due:") {
      s1=getTaskValue(fstr,spacechar+5,' ');
      this[length].due=QDate::fromString(s1,"yyyy-MM-dd");
      spacechar=spacechar+15;
      }
    if (fstr.substr(spacechar,5)==" url:") {
      s1=getTaskValue(fstr,spacechar+6,'\'');
      this[length].url=s1;
      }
    length++;
    }
  fin.close();
  wout.close();
  return length;
  }
//------------------------------------------------------------------------------
int ToDo::compareTasks(const void *a,const void *b) {
  ToDo *todoa=(ToDo *)a;
  ToDo *todob=(ToDo *)b;
  int comp_result;

  if (todoa->checkContext(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkProject(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkCompleted(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkPriority(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkDueDate(todob,&comp_result)!=0)
    return comp_result;
  return todoa->description.compare(todob->description);
  }
//------------------------------------------------------------------------------
int ToDo::compareTasksPriority(const void *a,const void *b) {
  ToDo *todoa=(ToDo *)a;
  ToDo *todob=(ToDo *)b;
  int comp_result;

  if (todoa->checkCompleted(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkPriority(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkDueDate(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkContext(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkProject(todob,&comp_result)!=0)
    return comp_result;
  return todoa->description.compare(todob->description);
  }
//------------------------------------------------------------------------------
int ToDo::compareTasksDueDate(const void *a,const void *b) {
  ToDo *todoa=(ToDo *)a;
  ToDo *todob=(ToDo *)b;
  int comp_result;

  if (todoa->checkCompleted(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkDueDate(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkPriority(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkContext(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkProject(todob,&comp_result)!=0)
    return comp_result;
  return todoa->description.compare(todob->description);
  }
//------------------------------------------------------------------------------
int ToDo::compareTasksPriorityDaysLeft(const void *a,const void *b) {
  ToDo *todoa=(ToDo *)a;
  ToDo *todob=(ToDo *)b;
  int comp_result;

  if (todoa->checkCompleted(todob,&comp_result)!=0)
    return comp_result;
  comp_result=(todoa->priority==0?MAXPRIORITY:todoa->priority)
         +todoa->WeightedDaysLeft()
         -(todob->priority==0?MAXPRIORITY:todob->priority)
         -todob->WeightedDaysLeft();
  if (comp_result!=0)
    return comp_result;
  if (todoa->checkPriority(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkDueDate(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkContext(todob,&comp_result)!=0)
    return comp_result;
  if (todoa->checkProject(todob,&comp_result)!=0)
    return comp_result;
  return todoa->description.compare(todob->description);
  }
//------------------------------------------------------------------------------
int ToDo::WeightedDaysLeft() {
  int daysleft;

  if (!this->due.isValid())
    return MAXPRIORITY;
  daysleft=static_cast<int>((this->due.toJulianDay()-QDate::currentDate().toJulianDay())/DAYSPERPRIO);
  return min(max(daysleft,0),MAXPRIORITY);
  }
//------------------------------------------------------------------------------
int ToDo::checkCompleted(ToDo *todo,int *result) {
  return *result=(this->completed==todo->completed?0:this->completed?1:-1);
  }
//------------------------------------------------------------------------------
int ToDo::checkPriority(ToDo *todo,int *result) {
  return *result=(this->priority==todo->priority?0:
                 this->priority==0?1:todo->priority==0?-1:
                 this->priority-todo->priority);
  }
//------------------------------------------------------------------------------
int ToDo::checkContext(ToDo *todo,int *result) {
  if (this->context==nullptr || todo->context==nullptr)
    return *result=(this->context==todo->context?0:
                   this->context==nullptr?1:-1);
  return *result=(this->context->description.compare(todo->context->description));
  }
//------------------------------------------------------------------------------
int ToDo::checkProject(ToDo *todo,int *result) {
  return *result=(this->getProjectName().compare(todo->getProjectName()));
  }
//------------------------------------------------------------------------------
int ToDo::checkDueDate(ToDo *todo,int *result) {
  if (!this->due.isValid() || !todo->due.isValid())
    return *result=(this->due.isValid()==todo->due.isValid()?
                   0:this->due.isValid()?-1:1);
  return *result=(this->due==todo->due?0:this->due<todo->due?-1:1);
  }
//------------------------------------------------------------------------------
QString ToDo::getProjectName() {
  if (project==nullptr)
    return "";
  else
    return project->description;
  }
//------------------------------------------------------------------------------
