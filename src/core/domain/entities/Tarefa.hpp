#pragma once

#include <string>

class Task {
public:

    int id;
    std::string title;
    std::string description;
    std::string status;
    int assignedUserId;

    Task() = default;

    Task(
        const std::string& title,
        const std::string& description,
        int assignedUserId
    ) {
        this->title = title;
        this->description = description;
        this->assignedUserId = assignedUserId;
        this->status = "pending";
    }
};
