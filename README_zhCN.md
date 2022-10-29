# DecoupleLogger

`DecoupleLogger`是一个只有一个头文件的C++11日志分离库。他可以将对日志库的引用与代码解耦，如此你可以更简单地将原有的代码提炼出来，并做相应的单元测试。

## 如何使用

> 强烈建议在阅读完`README`后查看[`example/helloworld.cpp`](example/helloworld.cpp)代码，这样能帮助你理解使用`DecoupleLogger`。

在你定义一个类时，你可以将类继承`LoggerBase`，然后在类内使用`callLog`成员函数来调用日志。

```c++
class Table : public LoggerBase {
public:
  void testLog() { callLog(LogLevel::INFO, "this is the %s's log", "Table"); }
};
```

当你实例化`Table`类后，你需要调用`setLoggerFunc`来为该类设定日志函数，

```c++
Table tb;
tb.setLoggerFunc([&](const LogLevel &log_level, const std::string &msg) {
    // 可以在这里引用日志库或者做一些自定义操作
    printf("%s\n", msg.c_str());
  });
```

否则会在调用日志的时候弹出：

```text
* WARN: LoggerBase class hasn't setLoggerFunc yet!
```

## 特性说明

### 日志函数的设定具备可传递性

当我们阅读了上述示例，我们可能会担心当类存在较多嵌套关系时，手动调用`setLoggerFunc`是否过于繁琐。不用担心，`DecoupleLogger`的`setLoggerFunc`是可以向下传递的。不过需要你手动指定同样继承了`LoggerBase`的成员变量。

在`example/helloworld.cpp`中，类`ClassRoom`包含了两个成员变量`tb`和`ch`，他们都继承了`LoggerBase`，于是，我在`ClassRoom`的构造函数里调用了：

```c++
ClassRoom() {
    registLoggerSubClass(&tb);
    registLoggerSubClass(&ch);
}
```

这样在使用的时候我们只需要调用`ClassRoom`的`setLoggerFunc`，就可以自动地为`tb`和`ch`设置同样的日志成员函数了。

### 支持text format

`callLog`可以像`printf`一样，接受`fmt`然后利用可变参来输出日志。需要注意的是，请留意一下项目中日志长度，并根据需要修改`LOG_MAX_LENGTH`，避免日志被`snprintf`截断。