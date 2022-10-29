# DecoupleLogger

[[中文]](README_zhCN.md)

`DecoupleLogger` is a C++11 logging decoupling library with only one header file. He can decouple the reference to the logging library from the code, so you can more simply distill the original code and do the corresponding unit tests.

## How to use

> It is highly recommended to look at the [`example/helloworld.cpp`](example/helloworld.cpp) code after reading `README` as it will help you understand using `DecoupleLogger`.

When you first define a class, you can inherit the class from `LoggerBase`, and then use the `callLog` member function within the class to call the log.

```c++
class Table : public LoggerBase {
public:
  void testLog() { callLog(LogLevel::INFO, "this is the %s's log", "Table"); }
};
```

When you instantiate the `Table` class, you need to call `setLoggerFunc` in advance to set the logging function of the class, otherwise the following prompt will pop up when logging is called:

```text
* WARN: LoggerBase class hasn't setLoggerFunc yet!
```

## Feature Description

### Logging functions can be delivered automatically

When we read the above example, we may worry whether it is too tedious to call `setLoggerFunc` manually when the classes have more nested relationships. Don't worry, `setLoggerFunc` of `DecoupleLogger` is able to distribute logging functions downwards. But you need to manually specify the member variables that also inherit from `LoggerBase`.

In `example/helloworld.cpp`, class `ClassRoom` contains two member variables `tb` and `ch`, both of which inherit from `LoggerBase`, so I called `registLoggerSubClass` in `ClassRoom`'s constructor to register `tb ` and `ch`.

```c++
ClassRoom() {
    registLoggerSubClass(&tb);
    registLoggerSubClass(&ch);
}
```

This way when using it we just need to call `setLoggerFunc` of `ClassRoom` to automatically set the same logging member functions for `tb` and `ch`.
