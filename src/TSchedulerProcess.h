//---------------------------------------------------------------------------

#ifndef TSchedulerProcessH
#define TSchedulerProcessH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <vector>
#include <map>
//---------------------------------------------------------------------------

class TFScheduler;
class ProcessThread;

enum EPriority
{
	ELow,
	ENormal,
	EHigh
};

class SchedulerProcess : public TThread
{
private:

	TFScheduler * m_child;
	UnicodeString m_status;
	int m_TimeSlice, m_code, m_priority;

	std::vector<ProcessThread *> m_ProcessLow;
	std::vector<ProcessThread *> m_ProcessNormal;
	std::vector<ProcessThread *> m_ProcessHigh;
	std::map<EPriority,std::vector<ProcessThread*> *> m_PrioThread;

	int GetPosThread(int code, EPriority prio);
	void StaggerProcess(std::vector<ProcessThread *> & Queue, EPriority prio);

	TEvent * m_FeedQueue;

protected:
	void __fastcall Execute();
public:
	__fastcall SchedulerProcess(TFScheduler * child, bool CreateSuspended);
	void __fastcall UpdateTimeSlice();
	void __fastcall UpdateStatus();

	void SetProcessThread(int Prioridade, int TimeCPU, int CodeProcess,UnicodeString TypeProcess);

	TMutex * m_Mutex;
};
//---------------------------------------------------------------------------
#endif
