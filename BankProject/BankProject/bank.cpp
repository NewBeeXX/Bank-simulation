#include"bank.h"
#include<iostream>
#include<clocale>
#include<chrono>
#include<cstdlib>

Bank::Bank(int window, int close_time) :_queue_number(window), _close_time(close_time),
_total_time(0),_customer_number(0){
	_work_queue = new std::queue<QueueNode>[window];
	srand(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
};

Bank::~Bank(){
	delete[] _work_queue;
}

void Bank::OpenForDay(){
	_event_queue.push({0,0});
}

void Bank::CustomerArrived(Event *event){//到达事件是由上一个客户的到达事件带动产生的
	++_customer_number;
	int duration_time, inter_time;
	duration_time = rand() % 1800 + 1;
	inter_time = rand() % 600 + 1;
	int t = event->_occur_time + inter_time;
	if (t < _close_time){
		_event_queue.push({t,0});

	}
	int i = FindShortestQueue();
	_work_queue[i].push({event->_occur_time,duration_time});

	if (_work_queue[i].size() == 1){  //如果他这条队队头则直接产生一个离开事件
		_event_queue.push({ event->_occur_time + duration_time, i + 1 });
		
	}


}

void Bank::CustomerDeparture(Event *event){//让离开的客户出队，累加停留时间，并让下一个客户开始办理
	int i = event->_type - 1;
	QueueNode customer = _work_queue[i].front();
	_work_queue[i].pop();
	_total_time += event->_occur_time - customer._arrival_time;

	if (!_work_queue[i].empty()){ //该条队下一个客户开始办理，将他的离开事件加入链表
		customer = _work_queue[i].front();
		_event_queue.push({
			customer._duration_time+event->_occur_time,i+1
		});
	}
}

int Bank::FindShortestQueue(){
	int res = 0;
	for (int i = 1; i < _queue_number; ++i){
		if (_work_queue[res].size() > _work_queue[i].size()){
			res = i;
		}
	}
	return res;
}



void Bank::Simulation(){
	OpenForDay();
	Event event;
	while (!_event_queue.empty()){
		event = _event_queue.top();
		_event_queue.pop();
		if (event._type == 0){
			CustomerArrived(&event);
		}
		else {
			CustomerDeparture(&event);
		}
	}
	std::wcout << L"客户数:" << _customer_number << std::endl
		<< L"总逗留时间(小时):" << (double)_total_time / 3600.0
		<< std::endl
		<< L"平均逗留时间(分钟):"
		<< (double)_total_time / (double)(_customer_number * 60)
		<< std::endl;
}

int wmain(int argc, wchar_t *argv[], wchar_t *env[]){
	_wsetlocale(LC_ALL,L"");
	Bank bank;
	bank.Simulation();
	return 0;
}


