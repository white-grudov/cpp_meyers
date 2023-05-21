#pragma once

#include <vector>
#include <utility>

/*
 * Item 41: Consider pass by value for copyable parameters that are cheap to move and always copied
 */

class Resource
{
    int data_;

public:
    std::size_t copyCount_ = 0;
    std::size_t moveCount_ = 0;

    Resource(int data) : data_(data) {}
    
    Resource(const Resource& other) : data_(other.data_), 
                                      copyCount_(other.copyCount_ + 1), 
                                      moveCount_(other.moveCount_) {}
                                      
    Resource(Resource&& other) noexcept : data_(other.data_),
                                          copyCount_(other.copyCount_), 
                                          moveCount_(other.moveCount_ + 1) {}
                                          
    Resource& operator=(const Resource& other)
    {
        data_ = other.data_;
        copyCount_ = other.copyCount_ + 1;
        moveCount_ = other.moveCount_;

        return *this;
    }
    Resource& operator=(Resource&& other) noexcept
    {
        data_ = other.data_;
        copyCount_ = other.copyCount_;
        moveCount_ = other.moveCount_ + 1;

        return *this;
    }
};

class Controller
{
    std::vector<Resource> resources_;

public:
    void addResourceByRef(const Resource& resource)
    {
        resources_.push_back(resource);
    }
    void addResourceByRef(Resource&& resource)
    {
        resources_.push_back(std::move(resource));
    }

    template<typename T, typename = std::enable_if_t<std::is_same_v<std::decay_t<T>, Resource>>>
    void addResourceTemplate(T&& resource)
    {
        resources_.push_back(std::forward<T>(resource));
    }

    void addResourseByValue(Resource resource)
    {
        resources_.push_back(std::move(resource));
    }

    std::size_t copyCount() const
    {
        return resources_[0].copyCount_;
    }
    std::size_t moveCount() const
    {
        return resources_[0].moveCount_;
    }
};

class Container 
{
    Resource resource_;

public:
    explicit Container(Resource r) : resource_(std::move(r)) {} 
    void changeToByValue(Resource r) 
    {
        resource_ = std::move(r); 
    } 
    void changeToByRef(const Resource& r) 
    {
        resource_ = std::move(r); 
    }

    std::size_t copyCount() const
    {
        return resource_.copyCount_;
    }
    std::size_t moveCount() const
    {
        return resource_.moveCount_;
    }
};