#pragma once

#include <vector>
#include <utility>

/*
 * Item 41: Consider pass by value for copyable parameters that are cheap to move and always copied
 */

class Resource
{
    int data;

public:
    std::size_t copyCount = 0;
    std::size_t moveCount = 0;

    Resource(int data) : data { data } {}
    
    Resource(const Resource& other) : data { other.data }, 
                                      copyCount { other.copyCount + 1 }, 
                                      moveCount { other.moveCount } {}
                                      
    Resource(Resource&& other) noexcept : data { other.data },
                                          copyCount { other.copyCount }, 
                                          moveCount { other.moveCount + 1 } {}
                                          
    Resource& operator=(const Resource& other) noexcept
    {
        data = other.data;
        copyCount = other.copyCount + 1;
        moveCount = other.moveCount;

        return *this;
    }
    Resource& operator=(Resource&& other) noexcept
    {
        data = other.data;
        copyCount = other.copyCount;
        moveCount = other.moveCount + 1;

        return *this;
    }
};

class Controller
{
    std::vector<Resource> resources;

public:
    void addResourceByRef(const Resource& resource) noexcept
    {
        resources.push_back(resource);
    }
    void addResourceByRef(Resource&& resource) noexcept
    {
        resources.push_back(std::move(resource));
    }

    template<typename T, typename = std::enable_if_t<std::is_same_v<std::decay_t<T>, Resource>>>
    void addResourceTemplate(T&& resource) noexcept
    {
        resources.push_back(std::forward<T>(resource));
    }

    void addResourceByValue(Resource resource) noexcept
    {
        resources.push_back(std::move(resource));
    }

    std::size_t copyCount() const
    {
        return resources[0].copyCount;
    }
    std::size_t moveCount() const
    {
        return resources[0].moveCount;
    }
};

class Container 
{
    Resource resource;

public:
    explicit Container(Resource r) : resource { std::move(r) } {} 
    void changeToByValue(Resource r) noexcept
    {
        resource = std::move(r); 
    } 
    void changeToByRef(const Resource& r) noexcept
    {
        resource = std::move(r); 
    }

    std::size_t copyCount() const noexcept
    {
        return resource.copyCount;
    }
    std::size_t moveCount() const noexcept
    {
        return resource.moveCount;
    }
};