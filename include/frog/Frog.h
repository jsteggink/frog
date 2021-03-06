/* ex: set tabstop=8 expandtab: */
/*
  $Id$
  $URL$

  Copyright (c) 2006 - 2015
  Tilburg University

  This file is part of frog

  frog is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  frog is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  For questions and suggestions, see:
      http://ilk.uvt.nl/software.html
  or send mail to:
      timbl@uvt.nl
*/


#ifndef FROG_H
#define FROG_H

#include <set>
#include "ticcutils/LogStream.h"
#include "ticcutils/Configuration.h"
#include "ticcutils/Timer.h"
#include "libfolia/document.h"


//declared here and defined in Frog-util.cxx (bit messy)
std::string prefix( const std::string&, const std::string& );
std::string getTime();
bool existsDir( const std::string& );
void getFileNames( const std::string&, const std::string&, std::set<std::string>& );

class TimerBlock{
public:
  TiCC::Timer parseTimer;
  TiCC::Timer tokTimer;
  TiCC::Timer mblemTimer;
  TiCC::Timer mbmaTimer;
  TiCC::Timer mwuTimer;
  TiCC::Timer tagTimer;
  TiCC::Timer iobTimer;
  TiCC::Timer nerTimer;
  TiCC::Timer prepareTimer;
  TiCC::Timer pairsTimer;
  TiCC::Timer relsTimer;
  TiCC::Timer dirTimer;
  TiCC::Timer csiTimer;
  TiCC::Timer frogTimer;
};


#endif
