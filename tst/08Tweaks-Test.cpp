#include "gtest/gtest.h"

#include "08Tweaks.h"

TEST(TweaksItem41, PassValueByReferenceOverload)
{
    Resource r { 42 };

    Controller c1, c2, c3;
    c1.addResourceByRef(r);
    c2.addResourceByRef(std::move(r));
    c3.addResourceByRef(Resource { 42 });

    EXPECT_EQ(1, c1.copyCount());
    EXPECT_EQ(0, c1.moveCount());

    EXPECT_EQ(0, c2.copyCount());
    EXPECT_EQ(1, c2.moveCount());

    EXPECT_EQ(0, c3.copyCount());
    EXPECT_EQ(1, c3.moveCount());
}

TEST(TweaksItem41, PassValueByUniversalRefTemplate)
{
    Resource r { 42 };

    Controller c1, c2, c3;
    c1.addResourceTemplate(r);
    c2.addResourceTemplate(std::move(r));
    c3.addResourceTemplate(Resource { 42 });

    EXPECT_EQ(1, c1.copyCount());
    EXPECT_EQ(0, c1.moveCount());

    EXPECT_EQ(0, c2.copyCount());
    EXPECT_EQ(1, c2.moveCount());

    EXPECT_EQ(0, c3.copyCount());
    EXPECT_EQ(1, c3.moveCount());
}

TEST(TweaksItem41, PassValueByValue)
{
    Resource r { 42 };

    Controller c1, c2, c3;
    c1.addResourceByValue(r);
    c2.addResourceByValue(std::move(r));
    c3.addResourceByValue(Resource { 42 });

    EXPECT_EQ(1, c1.copyCount());
    EXPECT_EQ(1, c1.moveCount());

    EXPECT_EQ(0, c2.copyCount());
    EXPECT_EQ(2, c2.moveCount());

    EXPECT_EQ(0, c3.copyCount());
    EXPECT_EQ(1, c3.moveCount());
}

TEST(TweaksItem41, AssignByRef)
{
    Container c(Resource(42));
    Resource newResource(5);
    c.changeToByRef(newResource);
    
    EXPECT_EQ(1, c.copyCount());
    EXPECT_EQ(0, c.moveCount());
}

TEST(TweaksItem41, AssignByValue)
{
    Container c(Resource(42));
    Resource newResource(5);
    c.changeToByValue(newResource);
    
    EXPECT_EQ(1, c.copyCount());
    EXPECT_EQ(1, c.moveCount());
}

TEST(TweaksItem42, ContainerInsertion)
{
    std::vector<Resource> resources;

    resources.push_back(Resource { 42 });

    EXPECT_EQ(0, resources[0].copyCount);
    EXPECT_EQ(1, resources[0].moveCount);
}

TEST(TweaksItem42, ContainerEmplacement)
{
    std::vector<Resource> resources;

    resources.emplace_back(42);

    EXPECT_EQ(0, resources[0].copyCount);
    EXPECT_EQ(0, resources[0].moveCount);
}