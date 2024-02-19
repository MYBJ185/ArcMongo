#pragma once
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/json.hpp>
#include <iostream>
#include <exception>
#include <string>
#include <Windows.h>
#include <chrono>
#include "Timer.h"
#include <thread>
#include <random>
#define PROFILING 1
#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif
// 将 GBK 编码的字符串转换为 UTF-8 编码
std::string gbk_to_utf8(const std::string& gbk_string);

// Print the contents of the given collection.
void printCollection(mongocxx::collection& collection);

// Find the document with given key-value pair.
void findDocument(mongocxx::collection& collection, const std::string& key, const std::string& value);

// Find the document with given key-value pair.
void findDocument(mongocxx::collection& collection, const std::string& key, int value);

// 获取最新的文档
bsoncxx::document::view getLatestDocument(mongocxx::collection& collection);

// 建立 MongoDB 连接
mongocxx::client establishConnection(const std::string& uri_string);

void insertTimeSeriesData(mongocxx::collection& collection, const bsoncxx::document::view& json_data);

void insertJsonData(mongocxx::collection& collection, const bsoncxx::document::view& json_data);

int generateRandomNumber(int min, int max);

// 插入数据到集合
void insertData(mongocxx::collection& collection);