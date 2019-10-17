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
#ifndef TODOTAG_H
#define TODOTAG_H
//------------------------------------------------------------------------------
#include <stdio.h>
#include <iostream>
#include <QString>
//------------------------------------------------------------------------------
class ToDoTag {
  public:
    QString description;
    class ToDoTag *Next;
    ToDoTag();
    ~ToDoTag();
  };
//------------------------------------------------------------------------------
#endif // TODOTAG_H
