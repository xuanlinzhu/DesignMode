![封面](./Source/pic/main-cover.png)

# 🚀 嵌入式视角的 GoF 设计模式实践

> 基于《Design Patterns: Elements of Reusable Object-Oriented Software》
>  从嵌入式系统架构出发，系统讲解 GoF 23 种设计模式，并提供工程级代码示例。

------

## 📌 项目简介

本仓库围绕 **GoF 设计模式原书结构**展开讲解，结合嵌入式系统开发场景进行重构与落地实现。

不同于面向 Web / 业务系统的讲解方式，本项目强调：

- 面向嵌入式系统的架构设计思维
- 资源受限环境下的模式取舍
- 模块解耦与可扩展性
- 产品线与平台演进能力
- 工程化可落地的代码结构

适合：

- 嵌入式开发工程师

- C 工程师

- 系统架构设计人员

- 希望系统掌握 GoF 理论与工程实践的开发者

  <p align="center">   <a href="https://www.bilibili.com/video/BV1vJHLz3ENX/">     <img src="./Source/pic/bilibili-cover.png" width="700">   </a> </p> <p align="center">   👉 <a href="https://www.bilibili.com/video/BV1vJHLz3ENX/"><strong>点击进入 B 站视频合集</strong></a> </p>

------

# 📚 GoF 23 种设计模式总览

## 一、创建型模式（Creational Patterns · 5）

| 英文名称         | 中文名称                 |
| ---------------- | ------------------------ |
| Abstract Factory | 抽象工厂模式             |
| Builder          | 生成器模式（建造者模式） |
| Factory Method   | 工厂方法模式             |
| Prototype        | 原型模式                 |
| Singleton        | 单例模式                 |

------

## 二、结构型模式（Structural Patterns · 7）

| 英文名称  | 中文名称             |
| --------- | -------------------- |
| Adapter   | 适配器模式           |
| Bridge    | 桥接模式             |
| Composite | 组合模式             |
| Decorator | 装饰模式             |
| Facade    | 外观模式（门面模式） |
| Flyweight | 享元模式             |
| Proxy     | 代理模式             |

------

## 三、行为型模式（Behavioral Patterns · 11）

| 英文名称                | 中文名称     |
| ----------------------- | ------------ |
| Chain of Responsibility | 责任链模式   |
| Command                 | 命令模式     |
| Interpreter             | 解释器模式   |
| Iterator                | 迭代器模式   |
| Mediator                | 中介者模式   |
| Memento                 | 备忘录模式   |
| Observer                | 观察者模式   |
| State                   | 状态模式     |
| Strategy                | 策略模式     |
| Template Method         | 模板方法模式 |
| Visitor                 | 访问者模式   |

# 🗂 仓库结构

```
Code/        设计模式代码实现
PDF/         教程 PDF 文档版本
Source/      原始讲义（Markdown / PPT）
Reference/   参考资料（含原书）
```

------

# 🧠 内容组织方式

每个设计模式通常包含：

- 原书理论背景解析
- UML 结构图拆解
- 问题场景分析
- 嵌入式场景映射
- 可运行代码示例
- 架构层面的演进讨论

------

# 🎯 为什么嵌入式更需要设计模式？

嵌入式系统通常具有：

- 生命周期长
- 平台迁移频繁
- 硬件耦合严重
- 可维护性要求高

设计模式在嵌入式中的核心价值：

- 抽象硬件差异
- 降低驱动与业务耦合
- 支持产品线扩展
- 提升系统可测试性
- 改善架构演进能力

在以下场景尤为重要：

- BSP 分层设计
- 驱动抽象层
- 中间件架构
- 多平台产品架构

------

# 📖 使用方式

建议学习路径：

1. 阅读 PDF / Markdown 讲义
2. 对照 UML 理解结构关系
3. 调试 Code 目录中的实现
4. 思考如何迁移到实际项目中

------

# ⚠ 版权说明

- `Reference/` 目录下内容禁止商业用途或转载
- 其余内容允许转载，请注明来源

------

# 🔎 关键词

 设计模式
 GoF
 嵌入式设计模式
 嵌入式架构
 C 设计模式
 软件架构
 Embedded System
 Embedded Architecture