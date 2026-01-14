#ifndef TODOLIST_TODO_ITEM_H
#define TODOLIST_TODO_ITEM_H

#include <string>
#include <chrono>
#include <ctime>

namespace todolist {

/**
 * @brief Represents a single todo item
 *
 * This class encapsulates all the properties of a todo item including
 * its unique identifier, title, description, completion status, and
 * creation timestamp.
 */
class TodoItem {
public:
    using TimePoint = std::chrono::system_clock::time_point;

    /**
     * @brief Default constructor
     * Creates an empty todo item with id 0
     */
    TodoItem();

    /**
     * @brief Constructor with all fields
     * @param id Unique identifier
     * @param title Todo item title
     * @param description Optional description
     * @param completed Whether the item is completed
     * @param created_at Creation timestamp
     */
    TodoItem(int id, std::string title, std::string description,
             bool completed, TimePoint created_at);

    /**
     * @brief Constructor for new items (without id and timestamp)
     * @param title Todo item title
     * @param description Optional description
     *
     * Sets id to 0 and created_at to current time
     */
    TodoItem(std::string title, std::string description);

    // Getters
    int getId() const { return id_; }
    const std::string& getTitle() const { return title_; }
    const std::string& getDescription() const { return description_; }
    bool isCompleted() const { return completed_; }
    TimePoint getCreatedAt() const { return created_at_; }

    // Setters
    void setId(int id) { id_ = id; }
    void setTitle(const std::string& title) { title_ = title; }
    void setDescription(const std::string& description) { description_ = description; }
    void setCompleted(bool completed) { completed_ = completed; }
    void setCreatedAt(TimePoint created_at) { created_at_ = created_at; }

    /**
     * @brief Convert timestamp to Unix epoch (seconds since 1970-01-01)
     * @return Unix timestamp in seconds
     */
    std::time_t getCreatedAtUnix() const;

    /**
     * @brief Create a TodoItem from Unix timestamp
     * @param unix_time Unix timestamp in seconds
     * @return TimePoint
     */
    static TimePoint fromUnixTime(std::time_t unix_time);

    /**
     * @brief Format the created_at timestamp as a readable string
     * @return Formatted timestamp string (YYYY-MM-DD HH:MM:SS)
     */
    std::string getFormattedCreatedAt() const;

private:
    int id_;
    std::string title_;
    std::string description_;
    bool completed_;
    TimePoint created_at_;
};

} // namespace todolist

#endif // TODOLIST_TODO_ITEM_H
