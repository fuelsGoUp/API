#pragma once

#include "../entities/Task.hpp"

#include <vector>

class ITaskRepository {
public:

    virtual void save(const Task& task) = 0;

    virtual std::vector<Task> findAll() = 0;

    virtual Task findById(int id) = 0;

    virtual void remove(int id) = 0;

    virtual ~ITaskRepository() = default;
};
