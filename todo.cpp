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
  completed=false;
  priority=0;
  description="";
  context=nullptr;
  project=nullptr;
  url="";
  due.setDate(0,0,0);
  }
//------------------------------------------------------------------------------
void ToDo::save(QString filename,int length) {
  ofstream wout;

  wout.open(filename.toStdString().c_str(),ofstream::out);
  for (int i1=0; i1<length; i1++) {
    if (this[i1].completed)
      wout << "x ";
    if (this[i1].priority>0)
      wout << "(" << char(this[i1].priority+64) << ") ";
    wout << this[i1].description.toStdString();
    if (this[i1].context!=nullptr)
      wout << " @\'" << this[i1].context->description.toStdString() << "\'";
    if (this[i1].project!=nullptr)
      wout << " +\'" << this[i1].project->description.toStdString() << "\'";
    if (this[i1].due.isValid())
      wout << " due:" << this[i1].due.toString("yyyy-MM-dd").toStdString();
    if (!this[i1].url.isEmpty())
      wout << " url:\'" << this[i1].url.toString().toStdString() << "\'";
    wout << endl;
    }
  wout.close();
  }
//------------------------------------------------------------------------------
QString ToDo::getTaskValue(string fstr,int start,char end) {
  return QString::fromStdString(fstr.substr(start,fstr.find_first_of(end,start)-start));
  }
//------------------------------------------------------------------------------
int ToDo::load(QString filename,ToDoTag **context,ToDoTag **project) {
  const int TODOTAGLENGTH=4;
  string TODOTAG[TODOTAGLENGTH]={
    " @'"," +'"," due:"," url:"
    };
  ToDoTag *tdt1;
  ifstream fin;
  string fstr;
  QString s1;
  int spacechar,length,minposition,pos;

  length=0;
  fin.open(filename.toStdString().c_str());
  while (getline(fin,fstr)) {
    if (fstr.length()==0)
      break;
    spacechar=0;
    if (fstr.substr(spacechar,2)=="x ") {
      this[length].completed=true;
      spacechar=2;
      }
    if (fstr.at(spacechar)=='(' && fstr.substr(spacechar+2,2)==") ") {
      this[length].priority=static_cast<int>(fstr.at(spacechar+1))-64;
      spacechar=spacechar+4;
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
  return length;
  }
//------------------------------------------------------------------------------
int ToDo::compareTasks(const void *a,const void *b) {
  ToDo *todoa=(ToDo *)a;
  ToDo *todob=(ToDo *)b;
  int comp_result;

  if (todoa->completed!=todob->completed)
    return todoa->completed?1:-1;
  if ((todoa->context==nullptr)^(todob->context==nullptr))
    return todoa->context==nullptr?1:-1;
  if (todoa->context!=nullptr && todob->context!=nullptr) {
    comp_result=(todoa->context->description.compare(todob->context->description));
    if (comp_result!=0)
      return comp_result;
    }
  if ((todoa->project==nullptr)^(todob->project==nullptr))
    return todoa->project==nullptr?1:-1;
  if (todoa->project!=nullptr && todob->project!=nullptr) {
    comp_result=(todoa->project->description.compare(todob->project->description));
    if (comp_result!=0)
      return comp_result;
    }
  if (todoa->priority!=todob->priority)
    return todoa->priority==0?1:todob->priority==0?-1:todoa->priority-todob->priority;
  if (todoa->due.isValid() && todob->due.isValid())
    return (todoa->due<todob->due?-1:1);
  if (todoa->due.isValid() || todob->due.isValid())
    return todoa->due.isValid()?-1:1;
  return 0;
  }
//------------------------------------------------------------------------------
int ToDo::compareTasksPriority(const void *a,const void *b) {
  ToDo *todoa=(ToDo *)a;
  ToDo *todob=(ToDo *)b;
  int comp_result;

  if (todoa->completed!=todob->completed)
    return todoa->completed?1:-1;
  if (todoa->priority!=todob->priority)
    return todoa->priority==0?1:todob->priority==0?-1:todoa->priority-todob->priority;
  if (todoa->due.isValid() && todob->due.isValid())
    return (todoa->due<todob->due?-1:1);
  if (todoa->due.isValid() || todob->due.isValid())
    return todoa->due.isValid()?-1:1;
  return 0;
  }
//------------------------------------------------------------------------------
int ToDo::compareTasksDueDate(const void *a,const void *b) {
  ToDo *todoa=(ToDo *)a;
  ToDo *todob=(ToDo *)b;
  int comp_result;

  if (todoa->completed!=todob->completed)
    return todoa->completed?1:-1;
  if (todoa->due.isValid() && todob->due.isValid())
    return (todoa->due<todob->due?-1:1);
  if (todoa->due.isValid() || todob->due.isValid())
    return todoa->due.isValid()?-1:1;
  return 0;
  }
//------------------------------------------------------------------------------
