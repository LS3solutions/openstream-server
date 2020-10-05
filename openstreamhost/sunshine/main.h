//
// Created by loki on 12/22/19.
//

#ifndef SUNSHINE_ENTRY_H
#define SUNSHINE_ENTRY_H

#include <boost/log/common.hpp>
#include <string>
#include "thread_pool.h"

extern util::ThreadPool task_pool;
extern bool display_cursor;

extern boost::log::sources::severity_logger<int> verbose;
extern boost::log::sources::severity_logger<int> debug;
extern boost::log::sources::severity_logger<int> info;
extern boost::log::sources::severity_logger<int> warning;
extern boost::log::sources::severity_logger<int> error;
extern boost::log::sources::severity_logger<int> fatal;

void log_flush();
void log_and_flush(std::basic_string_view<char> msg,
                   boost::log::sources::severity_logger<int> lvl);
#endif //SUNSHINE_ENTRY_H
