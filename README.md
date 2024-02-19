# MongoDB_Connector_Read

## 简介

这个项目是一个用于测试 MongoDB 连接性能的工具，同时也提供了一个泛用的性能分析工具。你可以使用这个工具来测量代码块的执行时间，并将结果写入 JSON 文件。你可以使用 Perfetto （https://ui.perfetto.dev/）来查看这个 JSON 文件。

## 使用方法

### MongoDB 连接性能测试

首先，你需要建立到 MongoDB 的连接。你可以使用 `establishConnection` 函数来建立连接。
`mongocxx::client client = establishConnection("mongodb://localhost:27017");`

然后，你可以使用 insertData 函数将数据插入到给定的集合中。这个函数会生成一些随机数据，并插入到集合中。

```
mongocxx::collection collection = client["testdb"]["testcollection"];
insertData(collection);
```

你还可以使用 printCollection 函数来打印集合的内容，或者使用 findDocument 函数来查找具有给定键值对的文档。

性能分析工具
你可以使用 PROFILE_SCOPE(name) 来测量一个命名的代码块，或者使用 PROFILE_FUNCTION() 来测量一个函数。这两个宏都会创建一个 InstrumentationTimer 实例，当代码块或函数执行完毕时，析构 InstrumentationTimer 并写入性能分析结果。

```
Instrumentor::Get().BeginSession("Session Name"); // 开始性能分析会话
{
    PROFILE_FUNCTION(); // 测量这个函数的执行时间

    for (int i = 0; i < 5; ++i)
    {
        PROFILE_SCOPE("Loop"); // 测量这个循环的执行时间
        // ...
    }
}
```
Instrumentor::Get().EndSession(); // 结束性能分析会话

然后，你可以在 Perfetto 上查看性能分析的结果。只需要将生成的 JSON 文件上传到 Perfetto，就可以看到一个详细的时间线视图，显示你的代码的执行情况。

宏设置
你可以通过定义 PROFILING 宏来开启或关闭性能分析。如果定义为 1，性能分析开启；如果定义为 0 或者没有定义，性能分析关闭。

```
#define PROFILING 1
#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif
```
