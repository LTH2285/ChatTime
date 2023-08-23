# 项目概要

## 项目名称

即时通讯软件Linpop

## 项目目标

1. 体会从需求理解出发，到软件整体设计，详细设计，开发，测试，发布的整体流程。熟悉软件开发整体过程

2. 了解基于Linux操作系统下的应用程序开发方法  

3. 掌握Linux系统下开发工具gdb、gcc、vim、makefile以及Qt Creator的使用

4. 培养快速学习新知识，并在项目中使用的能力及独立解决问题的能力

5. 掌握开源软件技术Qt图形库的使用，提高初级ui设计能力

6. 掌握Linux系统下程序并发的开发方法包括多进程(process)编程及多线程(pthread)编程。

7. 掌握socket通信编程方法

## 软件概要

- 开发一款运行于Linux操作系统平台的IM(即时通信)软件，可以实现局域网内任意电脑之间的信息接收与发送，也可作为支持200人中小型规模的企业，企业信息平台中的一个组件及功能存在。
- 需要针对企业情况，考虑项目的实现框架结构及相关程序的服务支撑度及稳定性，尤其需要对程序的容错有一定考虑。
    
## 功能描述

包含以下基本功能:

1、接受局域网上其他电脑的数据;

2、发送数据给局域网上其他电脑; 

3、可以随意设置发送ip地址;

4、程序界面及相关功能按钮

5、发送文件

## 开发环境

运行系统:虚拟机VM ware16、Ubuntu 10及其以上版本

开发工具:VIM+GCC+GDB+CMAKE或Qt Creator5.12及其以上版本

## 关键技术

Qt图形设计

    程序的界面由Qt图形库实现
   
Socket通信

    网络通信由Socket编程实现其功能
   
多线程、多进程编程

    程序的主框架应该是一个多进程、多线程结构。
   
# 软件详细需求

## 接口

界面风格为类似于x-windows应用程序界面。

### 发送信息

将指定信息发送给指定的ip地址(或用户名)的电脑，通过界面的文本框进行信息输入，点击发送按钮进行发送。

### 接受信息
 
从指定ip地址(或用户名)的电脑接受发送过来的信息，对方发送过来的信息自动显示在界面接受文本框中。

### 设置ip地址

设定可通信电脑的ip地址，通过界面的文本框输入或者使用用户名进行通讯人员的管理

### 文件传输

实现双方文件传输的功能

## 错误处理

需设计完整的错误处理机制

## 模块要求

**图形界面gui:**

- 登录窗口(如需要):包括相关按钮及控件，Qt实现

- 通信主窗口:包括相关按钮及控件，Qt实现

- 传输文件窗口:包括相关按钮及控件，Qt实现

**网络通信层:**

- 通信功能:单聊

**程序并发:** 

- 多进程接收发送信息及文件或者多线程接收或发送信息及文件