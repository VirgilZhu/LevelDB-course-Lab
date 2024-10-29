<<<<<<< HEAD
=======
#include <chrono>

>>>>>>> virgil
#include "gtest/gtest.h"
#include "leveldb/env.h"
#include "leveldb/db.h"

using namespace leveldb;

constexpr int value_size = 2048;
constexpr int data_size = 128 << 20;

Status OpenDB(std::string dbName, DB **db) {
  Options options;
  options.create_if_missing = true;
  return DB::Open(options, dbName, db);
}

void InsertData(DB *db, uint64_t ttl/* second */) {
  WriteOptions writeOptions;
  int key_num = data_size / value_size;
<<<<<<< HEAD
  srand(0);
=======
  srand(42);
>>>>>>> virgil

  for (int i = 0; i < key_num; i++) {
    int key_ = rand() % key_num+1;
    std::string key = std::to_string(key_);
    std::string value(value_size, 'a');
    db->ttl = ttl;
    db->Put(writeOptions, key, value, ttl);
  }
}

void GetData(DB *db, int size = (1 << 30)) {
  ReadOptions readOptions;
  int key_num = data_size / value_size;
  
  // 点查
<<<<<<< HEAD
  srand(0);
=======
  srand(42);
>>>>>>> virgil
  for (int i = 0; i < 100; i++) {
    int key_ = rand() % key_num+1;
    std::string key = std::to_string(key_);
    std::string value;
    db->Get(readOptions, key, &value);
  }
}

TEST(TestTTL, ReadTTL) {
    DB *db;
    if(OpenDB("testdb_ReadTTL", &db).ok() == false) {
        std::cerr << "open db failed" << std::endl;
        abort();
    }

    uint64_t ttl = 20;

    InsertData(db, ttl);

    ReadOptions readOptions;
    Status status;
    int key_num = data_size / value_size;
    srand(42);
    for (int i = 0; i < 100; i++) {
        int key_ = rand() % key_num+1;
        std::string key = std::to_string(key_);
        std::string value;
        status = db->Get(readOptions, key, &value);
        uint64_t now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        std::cout << status.ToString() << "    key: " << key << "    value: ******" << value.substr(value.find("_ts_")) << "    now: " << now << std::endl;
        ASSERT_TRUE(status.ok());
    }

    Env::Default()->SleepForMicroseconds(ttl * 1000000);

    for (int i = 0; i < 100; i++) {
        int key_ = rand() % key_num+1;
        std::string key = std::to_string(key_);
        std::string value;
        status = db->Get(readOptions, key, &value);
        uint64_t now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        std::cout << status.ToString() << "    key: " << key << "    value: ******" << value.substr(value.find("_ts_")) << "    now: " << now << std::endl;
        ASSERT_FALSE(status.ok());
    }
}

TEST(TestTTL, CompactionTTL) {
    DB *db;

    if(OpenDB("testdb_CompactionTTL", &db).ok() == false) {
        std::cerr << "open db failed" << OpenDB("testdb_CompactionTTL", &db).ToString() << std::endl;
        abort();
    }

    uint64_t ttl = 20;
    InsertData(db, ttl);

    leveldb::Range ranges[1];
    ranges[0] = leveldb::Range("-", "A");
    uint64_t sizes[1];
    db->GetApproximateSizes(ranges, 1, sizes);
    std::cout << "ApproximateSizes before TTL: " << sizes[0] << std::endl;
    ASSERT_GT(sizes[0], 0);

    Env::Default()->SleepForMicroseconds(ttl * 1000000);

    db->CompactRange(nullptr, nullptr);

    // leveldb::Range ranges[1];
    ranges[0] = leveldb::Range("-", "A");
    // uint64_t sizes[1];
    db->GetApproximateSizes(ranges, 1, sizes);
    std::cout << "ApproximateSizes after TTL: " << sizes[0] << std::endl;
    ASSERT_EQ(sizes[0], 0);
}


int main(int argc, char** argv) {
  // All tests currently run with the same read-only file limits.
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}