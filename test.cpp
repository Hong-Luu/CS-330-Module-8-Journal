// Uncomment the next line to use precompiled headers
#include "pch.h"
// uncomment the next line if you do not use precompiled headers
//#include "gtest/gtest.h"
//
// the global test environment setup and tear down
// you should not need to change anything here
class Environment : public ::testing::Environment
{
public:
    ~Environment() override {}

    // Override this to define how to set up the environment.
    void SetUp() override
    {
        //  initialize random seed
        srand(time(nullptr));
    }

    // Override this to define how to tear down the environment.
    void TearDown() override {}
};

// create our test class to house shared data between tests
// you should not need to change anything here
class CollectionTest : public ::testing::Test
{
protected:
    // create a smart point to hold our collection
    std::unique_ptr<std::vector<int>> collection;

    void SetUp() override
    { // create a new collection to be used in the test
        collection.reset(new std::vector<int>);
    }

    void TearDown() override
    { //  erase all elements in the collection, if any remain
        collection->clear();
        // free the pointer
        collection.reset(nullptr);
    }

    // helper function to add random values from 0 to 99 count times to the collection
    void add_entries(int count)
    {
        assert(count > 0);
        for (auto i = 0; i < count; ++i)
            collection->push_back(rand() % 100);
    }
};

// When should you use the EXPECT_xxx or ASSERT_xxx macros?
// Use ASSERT when failure should terminate processing, such as the reason for the test case.
// Use EXPECT when failure should notify, but processing should continue

// Test that a collection is empty when created.
// Prior to calling this (and all other TEST_F defined methods),
//  CollectionTest::StartUp is called.
// Following this method (and all other TEST_F defined methods),
//  CollectionTest::TearDown is called
TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
    // is the collection created
    ASSERT_TRUE(collection);

    // if empty, the size must be 0
    ASSERT_NE(collection.get(), nullptr);
}

// Test that a collection is empty when created.
TEST_F(CollectionTest, IsEmptyOnCreate)
{
    // is the collection empty?
    ASSERT_TRUE(collection->empty());

    // if empty, the size must be 0
    ASSERT_EQ(collection->size(), 0);
}

/* Comment this test out to prevent the test from running
 * Uncomment this test to see a failure in the test explorer */
TEST_F(CollectionTest, AlwaysFail)
{
    FAIL();
}

// TODO: Create a test to verify adding a single value to an empty collection
TEST_F(CollectionTest, CanAddToEmptyVector)
{
    // is the collection empty?
    ASSERT_TRUE(collection->empty());
    // if empty, the size must be 0
    ASSERT_EQ(collection->size(), 0);


    add_entries(1); // add a single value to the empty collection
    
    // is the collection still empty?
    ASSERT_FALSE(collection->empty());
    // if not empty, what must the size be?
    ASSERT_EQ(collection->size(), 1); // if not empty, the size must be 1
}

// TODO: Create a test to verify adding five values to collection
TEST_F(CollectionTest, CanAddFiveValuesToVector)
{
    add_entries(5);

    ASSERT_EQ(collection->size(), 5); // check if the size of the collection is 5
}

// TODO: Create a test to verify that max size is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, MaxSizeGreaterThanOrEqualToSize) {
    // Test for 0 entry
    collection->clear();
    ASSERT_GE(collection->max_size(), collection->size());

    // Test for 1 entry
    add_entries(1);
    ASSERT_GE(collection->max_size(), collection->size());

    // Test for 5 entries
    collection->clear();
    add_entries(5);
    ASSERT_GE(collection->max_size(), collection->size());

    // Test for 10 entries
    collection->clear();
    add_entries(10);
    ASSERT_GE(collection->max_size(), collection->size());
}

// TODO: Create a test to verify that capacity is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, CapacityGreaterThanOrEqualToSize)
{
    // Test for 0 entries
    collection->clear();
    ASSERT_GE(collection->capacity(), collection->size());

    // Test for 1 entry
    add_entries(1);
    ASSERT_GE(collection->capacity(), collection->size());

    // Test for 5 entries
    collection->clear();
    add_entries(5);
    ASSERT_GE(collection->capacity(), collection->size());

    // Test for 10 entries
    collection->clear();
    add_entries(10);
    ASSERT_GE(collection->capacity(), collection->size());
}

// TODO: Create a test to verify resizing increases the collection
TEST_F(CollectionTest, ResizeIncreasesCollection)
{
    size_t initialSize = collection->size();
    size_t newSize = initialSize + 5;

    collection->resize(newSize);

    ASSERT_EQ(collection->size(), newSize);
}

// TODO: Create a test to verify resizing decreases the collection
TEST_F(CollectionTest, ResizeDecreasesCollection)
{
    add_entries(10); // Add 10 elements to the collection
    size_t initialSize = collection->size();
    size_t newSize = initialSize - 5; // Decrease size by 5

    collection->resize(newSize);

    ASSERT_EQ(collection->size(), newSize);
}

// TODO: Create a test to verify resizing decreases the collection to zero
TEST_F(CollectionTest, ResizeDecreasesCollectionToZero)
{
    add_entries(5); // Add 5 elements to the collection
    size_t newSize = 0; // Set new size to zero

    collection->resize(newSize);

    ASSERT_EQ(collection->size(), newSize);
}

// TODO: Create a test to verify clear erases the collection
TEST_F(CollectionTest, ClearErasesCollection)
{
    add_entries(5); // Add 5 elements to the collection

    collection->clear();

    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);
}

// TODO: Create a test to verify erase(begin,end) erases the collection
TEST_F(CollectionTest, EraseBeginToEndErasesCollection)
{
    add_entries(5); // Add 5 elements to the collection

    collection->erase(collection->begin(), collection->end());

    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);
}

// TODO: Create a test to verify reserve increases the capacity but not the size of the collection
TEST_F(CollectionTest, ReserveIncreasesCapacityNotSize)
{
    size_t initialSize = collection->size();
    size_t initialCapacity = collection->capacity();
    size_t reserveSize = initialCapacity + 5; // Reserve additional 5 spaces

    collection->reserve(reserveSize);

    ASSERT_EQ(collection->size(), initialSize); // Size remains unchanged
    ASSERT_GE(collection->capacity(), reserveSize); // Capacity increases to at least reserveSize
}

// TODO: Create a test to verify the std::out_of_range exception is thrown when calling at() with an index out of bounds
// NOTE: This is a negative test
TEST_F(CollectionTest, OutOfRangeExceptionThrown)
{
    add_entries(5); // Add 5 elements to the collection

    // Try accessing an element out of bounds using at()
    ASSERT_THROW(collection->at(10), std::out_of_range);
}

// TODO: Create 2 unit tests of your own to test something on the collection - do 1 positive & 1 negative
TEST_F(CollectionTest, AddElementIncreasesSize)
{
    size_t initialSize = collection->size();

    collection->push_back(42); // Add an element to the collection

    ASSERT_EQ(collection->size(), initialSize + 1); // Size should increase by 1
}

// TODO: Create a negative unit test to verify that attempting to access an element using at() with an empty collection throws an exception
TEST_F(CollectionTest, AccessElementInEmptyCollectionThrowsException)
{
    // Ensure the collection is empty
    collection->clear();

    // Try accessing an element using at()
    ASSERT_THROW(collection->at(0), std::out_of_range);
}