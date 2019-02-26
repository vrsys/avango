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

// include i/f header

// includes, system

#include <cstdlib>
#include <fstream>
#include <sstream>

// includes, project

#include <avango/logging/ConsoleAppender.h>
#include <avango/logging/FileAppender.h>
#include <avango/logging/Logger.h>
#include <avango/logging/StreamAppender.h>

#include <avango/UnitTest++/UnitTest++.h>

// variables, exported

// internal unnamed namespace

namespace
{
const char* build_filename(void)
{
    static const char* fallback = "log-file.txt";
    const char* envname = std::getenv("AVANGO_TEST_LOG");
    if(envname != 0)
        return envname;
    else
        return fallback;
}
std::string filename(build_filename());

void clearFile()
{
    std::ofstream output(filename.c_str(), std::ios_base::trunc);
    output.close();
}

TEST(useLogger)
{
    clearFile();
    av::Logger& z = av::Logger::getLogger("useLogger::Group");
    av::Logger& y = av::Logger::getLogger("useLogger::Node");
    av::Logger& x = av::Logger::getLogger("useLogger");

    x.info("log message using log() method");
    y.info("log message using log() method");
    z.info("useLogger: log message using log() method");

    boost::shared_ptr<av::logging::Appender> file_app(new av::logging::FileAppender(filename));
    x.addAppender(file_app);

    x.info("log message using log() method");
    x.info() << "streamed log message";
    x.info() << "before -- %s %x %.1f -- after", "1", 42, 5.45;

    x.removeAllAppenders();

    std::ifstream input(filename.c_str());

    CHECK(input.is_open());

    std::string line;
    int count = 0;

    while(!input.eof())
    {
        getline(input, line);
        count++;

        if(count == 1)
        {
            CHECK((std::string::npos != line.find("log message using log() method", 0)));
        }
        else if(count == 2)
        {
            CHECK((std::string::npos != line.find("streamed log message", 0)));
        }
        else if(count == 3)
        {
            CHECK((std::string::npos != line.find("before -- 1 2a 5.5 -- after", 0)));
        }
    }

    input.close();
}

TEST(useLoggerHierarchy)
{
    clearFile();

    av::Logger& z = av::Logger::getLogger("useLoggerHierarchy::Group");
    av::Logger& y = av::Logger::getLogger("useLoggerHierarchy::Node");
    av::Logger& x = av::Logger::getLogger("useLoggerHierarchy");

    boost::shared_ptr<av::logging::Appender> file_app(new av::logging::FileAppender(filename));

    x.addAppender(file_app);
    y.addAppender(file_app);
    z.addAppender(file_app);

    x.setLevel(av::logging::ERROR);
    y.setLevel(av::logging::FATAL);
    z.setLevel(av::logging::ERROR);

    x.error() << "error message that should appear once";
    y.info() << "info message that should NOT appear";
    z.error() << "error message that should appear two times";

    std::ifstream input(filename.c_str());

    CHECK(input.is_open());

    std::string line;
    int count = 0;
    while(!input.eof())
    {
        getline(input, line);
        count++;

        if(count == 1)
        {
            CHECK((std::string::npos != line.find("error message that should appear once", 0)));
        }
        else if(count == 2 || count == 3)
        {
            CHECK((std::string::npos != line.find("error message that should appear two times", 0)));
        }
    }

    input.close();

    x.removeAllAppenders();
    y.removeAllAppenders();
    z.removeAllAppenders();
}

TEST(useLoggerStream)
{
    av::Logger& logger = av::Logger::getLogger("useLoggerStream");

    std::ostringstream os;
    boost::shared_ptr<av::logging::Appender> stream_app(new av::logging::StreamAppender(os));

    logger.addAppender(stream_app);

    av::Logger::Logger::Stream info(logger, av::logging::INFO);
    info << "test message";
    info.flush();

    logger.removeAppender(stream_app);

    CHECK(os.str().find("test message") != std::string::npos);
}

TEST(addConsoleAppender)
{
    av::Logger& logger = av::Logger::getLogger("consoleLogger");
    av::Logger::getRootLogger().addConsoleAppender();
    av::Logger::getRootLogger().addConsoleAppender();
    logger.info() << "consoleLogger: added console appender two times";
    CHECK(av::Logger::getRootLogger().getAppenders().size() == 1);
}

TEST(isActive)
{
    av::Logger& child = av::Logger::getLogger("useLoggerHierarchy::Node");
    av::Logger& parent = av::Logger::getLogger("useLoggerHierarchy");

    boost::shared_ptr<av::logging::Appender> file_app(new av::logging::FileAppender(filename));
    parent.addAppender(file_app);

    parent.setLevel(av::logging::ERROR);
    child.setLevel(av::logging::FATAL);

    CHECK(child.isActive(av::logging::ERROR));
}

TEST(LOGMacroSuccessfulLogging)
{
    av::Logger& logger = av::Logger::getLogger("useLoggerHierarchy");
    std::ostringstream os;
    boost::shared_ptr<av::logging::Appender> stream_app(new av::logging::StreamAppender(os));
    logger.addAppender(stream_app);
    logger.setLevel(av::logging::ERROR);

    AVANGO_LOG(logger, av::logging::ERROR, "Test");

    CHECK(!os.str().empty());
}

TEST(LOGMacroUnsuccessfulLogging)
{
    av::Logger& logger = av::Logger::getLogger("useLoggerHierarchy");
    std::ostringstream os;
    boost::shared_ptr<av::logging::Appender> stream_app(new av::logging::StreamAppender(os));
    logger.addAppender(stream_app);
    logger.setLevel(av::logging::ERROR);

    AVANGO_LOG(logger, av::logging::WARN, "Test");

    CHECK(os.str().empty());
}

} // namespace

// functions, exported

int main() { return UnitTest::RunAllTests(); }
