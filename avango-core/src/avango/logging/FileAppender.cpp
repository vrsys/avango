// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include "avango/logging/FileAppender.h"

#include <avango/logging/LoggingEvent.h>


av::logging::FileAppender::FileAppender(const std::string& filename)
 : mLogFile(filename.c_str(), std::ios_base::app)
{
  if (!mLogFile.is_open())
    throw std::ios::failure("Could not open file " + filename);
}


av::logging::FileAppender::~FileAppender()
{}


const std::string&
av::logging::FileAppender::getFilename() const
{
  return mFilename;
}

/* virtual */ void
av::logging::FileAppender::doAppend(LoggingEvent& event)
{
  mLogFile << event.getFormattedString() << std::endl;

  // probably inefficient, currently needed for correct serialization of log messages, see tests
  mLogFile.flush();
}
