#ifndef __BANK_H__
#define __BANK_H__

#include<queue>


struct Event{
	int _occur_time;
	int _type;
	friend bool operator <(const Event& a, const Event& b){
		return a._occur_time > b._occur_time;
	}
};


struct QueueNode{
	int _arrival_time;
	int _duration_time;


};
class Bank{
public:
	explicit Bank(int window_number = 4, int close_time = 8 * 3600);
		~Bank();
		void Simulation();
private:
	int _queue_number; //队列数
	int _close_time; //关门时间
	int _total_time; //客户总逗留时间
	int _customer_number; //客户总数

	std::priority_queue<Event> _event_queue; //事件队列
	std::queue<QueueNode> *_work_queue;

	void OpenForDay();
	void CustomerArrived(Event *event);
	void CustomerDeparture(Event *event);
	int FindShortestQueue();
//	void SortEventList();


};


#endif