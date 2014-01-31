// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 2007 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <shade/FileResolver.h>
#include <cstdlib>
#include <UnitTest++.h>

SUITE(FileResolver)
{
  TEST(NoPathAdded)
  {
    shade::FileResolver resolver;
    CHECK_THROW(resolver.load("aword.txt"), shade::FileNotFound);
  }

  TEST(LocalLoading)
  {
    shade::FileResolver resolver;
    char* testdir = std::getenv("SHADE_TEST_DATA");
    CHECK(testdir);
    if (testdir)
    {
      resolver.append_path(testdir);
      std::string contents = resolver.load("aword.txt");

      CHECK_EQUAL("bingo\n", contents);
    }
  }

  TEST(LocalLoadingWithoutSlash)
  {
    shade::FileResolver resolver;
    char* testdir = std::getenv("SHADE_TEST_DATA");
    CHECK(testdir);
    if (testdir)
    {
      std::string dir(testdir);
      resolver.append_path(dir.substr(0, dir.size()-1));
      std::string contents = resolver.load("aword.txt");

      CHECK_EQUAL("bingo\n", contents);
    }
  }

  TEST(LocalLoadingWithMultiplePathes)
  {
    shade::FileResolver resolver;
    char* testdir = std::getenv("SHADE_TEST_DATA");
    char* testdir2 = std::getenv("SHADE_TEST_DATA2");
    CHECK(testdir);
    CHECK(testdir2);
    if (testdir && testdir2)
    {
      resolver.append_path(testdir2);
      resolver.append_path(testdir);

      std::string contents = resolver.load("aword.txt");
      CHECK_EQUAL("bingo\n", contents);
    }
  }

  TEST(PathLoading)
  {
    shade::FileResolver resolver;
    char* testpath = std::getenv("SHADE_TEST_PATH");
    CHECK(testpath);
    if (testpath)
    {
      shade::append_env_path(resolver, testpath);

      std::string contents = resolver.load("aword.txt");
      CHECK_EQUAL("bingo\n", contents);
    }
  }

  TEST(LocalLoadingWithMultiplePrependedPathes)
  {
    shade::FileResolver resolver;
    char* testdir = std::getenv("SHADE_TEST_DATA");
    char* testdir2 = std::getenv("SHADE_TEST_DATA2");
    CHECK(testdir);
    CHECK(testdir2);
    if (testdir && testdir2)
    {
      resolver.prepend_path(testdir2);
      resolver.prepend_path(testdir);

      std::string contents = resolver.load("doubleentry.txt");
      CHECK_EQUAL("in testdata/\n", contents);
    }
  }
}
