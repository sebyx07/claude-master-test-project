#include "todolist/todo_item.h"
#include <sstream>
#include <iomanip>

namespace todolist {

TodoItem::TodoItem()
    : id_(0)
    , title_("")
    , description_("")
    , completed_(false)
    , created_at_(std::chrono::system_clock::now())
{
}

TodoItem::TodoItem(int id, std::string title, std::string description,
                   bool completed, TimePoint created_at)
    : id_(id)
    , title_(std::move(title))
    , description_(std::move(description))
    , completed_(completed)
    , created_at_(created_at)
{
}

TodoItem::TodoItem(std::string title, std::string description)
    : id_(0)
    , title_(std::move(title))
    , description_(std::move(description))
    , completed_(false)
    , created_at_(std::chrono::system_clock::now())
{
}

std::time_t TodoItem::getCreatedAtUnix() const {
    return std::chrono::system_clock::to_time_t(created_at_);
}

TodoItem::TimePoint TodoItem::fromUnixTime(std::time_t unix_time) {
    return std::chrono::system_clock::from_time_t(unix_time);
}

std::string TodoItem::getFormattedCreatedAt() const {
    std::time_t time = getCreatedAtUnix();
    std::tm tm_time;

    // Use localtime_r for thread safety (POSIX)
    #ifdef _WIN32
        localtime_s(&tm_time, &time);
    #else
        localtime_r(&time, &tm_time);
    #endif

    std::ostringstream oss;
    oss << std::put_time(&tm_time, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

} // namespace todolist
