#include "wx\wxprec.h"
#include "wx\wx.h"
#include <iostream>
#include <string>
#include "snake.h"
#include "food.h"
#include "wx\timer.h"
#include "wx/button.h"


using namespace std;

#ifndef WX_PRECOMP
#include "include\wx\wx.h"
#include <wx/msgdlg.h>
#endif
#define TIMER_ID 1000
#define BUTTON_IDup 1500
#define BUTTON_IDdown 1501
#define BUTTON_IDleft 1502
#define BUTTON_IDright 1503

class MyApp : public wxApp
{
public: virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	void OnTimer(wxTimerEvent& event);
	void up(wxCommandEvent& event)
	{
		head->turnUp();
	}
	void down(wxCommandEvent& event)
	{
		head->turnDown();
	}
	void left(wxCommandEvent& event)
	{
		head->turnLeft();
	}
	void right(wxCommandEvent& event)
	{
		head->turnRight();
	}
	wxTimer mytimer;
private:
	wxDECLARE_EVENT_TABLE();
	std::shared_ptr<SnakeHead> head;
	void showthem();
	food *afood;
	wxButton* buttup;
	wxButton* buttd;
	wxButton* buttl;
	wxButton* buttr;
};



wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_TIMER(TIMER_ID, MyFrame::OnTimer)
EVT_BUTTON(BUTTON_IDup,MyFrame::up)
EVT_BUTTON(BUTTON_IDdown, MyFrame::down)
EVT_BUTTON(BUTTON_IDleft, MyFrame::left)
EVT_BUTTON(BUTTON_IDright, MyFrame::right)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);


bool MyApp::OnInit()
{
	MyFrame* frame = new MyFrame("贪吃蛇", wxPoint(100, 100), wxSize(800, 800));
	frame->Show(true);
	return true;
}
//静态数据成员不能在类中初始化
std::shared_ptr<SnakeHead> SnakeHead::Ainstance = nullptr;
std::mutex SnakeHead::MyMutex;

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size), mytimer(this, TIMER_ID)
{
	wxMenu* menuFile = new wxMenu;
	wxMenuBar* menuBar = new wxMenuBar;
	SetMenuBar(menuBar);
	head = SnakeHead::getIntance();
	CreateStatusBar();
	mytimer.Start(200);
	SnakeBody firstbody(true, 9, 10);
	SnakeBody secondbody(false, 8, 10);
	snbodys.push_back(firstbody);
	snbodys.push_back(secondbody);
	srand(time(0));
	afood = new  food(rand() % 49, rand() % 49);
	const wxPoint btup(600, 150);
	const wxSize szup(40, 40);
	const wxPoint btup1(600, 300);
	const wxSize szup1(40, 40);
	const wxPoint btup2(550, 225);
	const wxSize szup2(40, 40);
	const wxPoint btup3(650, 225);
	const wxSize szup3(40, 40);
	buttup = new wxButton(this, BUTTON_IDup, "上", btup, szup, wxBU_TOP);
	buttd = new wxButton(this, BUTTON_IDdown, "下", btup1, szup1, wxBU_TOP);
	buttl = new wxButton(this, BUTTON_IDleft, "左", btup2, szup2, wxBU_TOP);
	buttr = new wxButton(this, BUTTON_IDright, "右", btup3, szup3, wxBU_TOP);
}

void MyFrame::OnTimer(wxTimerEvent& event)//检查碰撞,更新界面
{

	//eat	
	//wxMessageBox(wxT("Hello World!"));
	if (head != nullptr)
	{
		if (afood->getx() == head->getx() && afood->gety() == head->gety())
		{
			delete afood;
			srand(time(0));
			int a, b;
			while (1)
			{
				a = rand() % 49, b = rand() % 49;
				list<SnakeBody>::iterator thisbody1 = find_if(snbodys.begin(), snbodys.end(),
					[a, b](SnakeBody& item)-> bool { return (a == item.getx() && b == item.gety()); });
				if (thisbody1 == snbodys.end()) break;
			}
			afood = new food(a, b);
			//create body
			list<SnakeBody>::iterator thisbody = snbodys.end();
			thisbody--;
			SnakeBody newbody(false, thisbody->getlx(thisbody), thisbody->getly(thisbody));
			snbodys.push_back(newbody);
		}
		showthem();
	}
	
}

void MyFrame::showthem()
{
	//move and paint
	wxClientDC* dc = new wxClientDC(this);
	dc->Clear();
	for (int i = 0; i < 51; i++)
	{
		dc->DrawRectangle(i*10, 500, 10, 10);
		dc->DrawRectangle(500, i*10, 10, 10);
	}
	//move
	list<SnakeBody>::iterator thisbody = snbodys.begin();
	for (thisbody; thisbody != snbodys.end(); thisbody++)
	{
		thisbody->follow();
		dc->DrawRectangle(10 * thisbody->getx(), 10 * thisbody->gety(), 10, 10);
	}
	head->toNext();
	//paint
	dc->DrawRectangle(10 * afood->getx(), 10 * afood->gety(), 10, 10);
	dc->DrawRectangle(10*head->getx(), 10*head->gety(), 10, 10);
	head->iscrash();

}



