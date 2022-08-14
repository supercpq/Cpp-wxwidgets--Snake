#pragma once
#include <iostream>
#include <memory>
#include <list>
#include <algorithm>
#include <mutex>


class SnakeHead//蛇头只有一个，用双检锁单例模式
{
    //蛇头负责碰撞检测和方向控制（可能不太符合单一职责原则？）
private:
    int dx, dy;
    int direction;//1234代表四个方向（其实这里更好的写法是用状态模式，可以更好地解耦）
    SnakeHead(int a, int b,int c):dx(a),dy(b),direction(c) { }
    SnakeHead(const SnakeHead& s);
    const SnakeHead& operator=(const SnakeHead& s) { return *this; }
    bool isend()
    {
        if (dx == 50 || dx == 0 || dy == 50 || dy == 0)
        {
            exit(0);
        }
        return true;
    }
    static std::shared_ptr<SnakeHead> Ainstance;
    static std::mutex MyMutex;
public:
    int getx();
    int gety();
    ~SnakeHead() { dx = 10; dy = 10; }
    static std::shared_ptr<SnakeHead> getIntance()
    {
        if (Ainstance == nullptr)
        {
            std::lock_guard<std::mutex> _lock(MyMutex);
            Ainstance = std::shared_ptr<SnakeHead>(new SnakeHead(10,10,4));
        }
        return Ainstance;
    }
    void iscrash();
    void toNext();
    void turnUp();
    void turnDown();
    void turnLeft();
    void turnRight();
};
class SnakeBody
{
//蛇身只负责跟着就行
private:
    int dx, dy,lastx,lasty;
    bool ifhead;
    std::shared_ptr<SnakeHead> head;
   
    void setl(std::list<SnakeBody>::iterator prebody)
    {
        prebody->lastx = prebody->dx;
        prebody->lasty = prebody->dy;
    }
public:
    int getx(std::list<SnakeBody>::iterator prebody);
    int gety(std::list<SnakeBody>::iterator prebody);
    int getx()
    {
        return dx;
    }
    int gety()
    {
        return dy;
    }
    int getlx(std::list<SnakeBody>::iterator prebody)
    {
        int a = prebody->lastx;
        //prebody->lastx = dx;
        return a;
    }
    int getly(std::list<SnakeBody>::iterator prebody)
    {
        int a = prebody->lasty;
        //prebody->lasty = dy;
        return a;
    }
    SnakeBody(bool ifheadd, int a, int b) :ifhead(ifheadd), dx(a), dy(b) { head = SnakeHead::getIntance(); lastx = a; lasty = b; }
    void follow();
};
extern std::list<SnakeBody> snbodys;