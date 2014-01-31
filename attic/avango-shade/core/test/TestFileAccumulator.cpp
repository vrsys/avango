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

#include <shade/FileAccumulator.h>
#include <cstdlib>
#include <UnitTest++.h>

SUITE(FileAccumulator)
{
  struct SearchPathFixture
  {
    struct EnvNotSet {};

    SearchPathFixture(void) :
      acc(resolver)
    {
      char* testdir = std::getenv("SHADE_TEST_DATA");
      if (testdir == 0)
        throw EnvNotSet();

      resolver.append_path(testdir);
    }

    shade::FileResolver resolver;
    shade::FileAccumulator acc;
  };

  TEST_FIXTURE(SearchPathFixture, SingleFileLoad)
  {
    acc.add_file("aword.txt");

    std::string contents = acc.get_str();
    CHECK_EQUAL("bingo\n", contents);
  }

  TEST_FIXTURE(SearchPathFixture, MultipleFilesLoad)
  {
    acc.add_file("aword.txt");
    acc.add_file("anotherword.txt");

    std::string contents = acc.get_str();
    CHECK_EQUAL("bingo\nyes!\n", contents);
  }

  TEST_FIXTURE(SearchPathFixture, MultipleIdenticalFilesLoad)
  {
    acc.add_file("aword.txt");
    acc.add_file("anotherword.txt");
    acc.add_file("aword.txt");

    std::string contents = acc.get_str();
    CHECK_EQUAL("bingo\nyes!\n", contents);
  }

  TEST_FIXTURE(SearchPathFixture, GetVersion)
  {
    acc.add_file("version100.glsl");
    CHECK_EQUAL(100, acc.get_glsl_version());
  }

  TEST_FIXTURE(SearchPathFixture, GetMaxVersion)
  {
    acc.add_file("version110.glsl");
    acc.add_file("version100.glsl");
    CHECK_EQUAL(110, acc.get_glsl_version());
  }

  TEST_FIXTURE(SearchPathFixture, LoadEmptyFilename)
  {
    acc.add_file("");
    std::string contents = acc.get_str();
    CHECK_EQUAL("", contents);
  }

  TEST_FIXTURE(SearchPathFixture, GetGpuShader4Extension)
  {
    acc.add_file("gpu_shader_4_ext.glsl");
    CHECK(acc.requires_gpu_shader4_ext());
  }

}
