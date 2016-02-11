//---------------------------------------------------------------------------

#ifndef TProcessThreadH
#define TProcessThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class ProcessThread : public TThread
{
private:

	int m_prio;
	int m_timeCPU;
	int m_codeProcess;
	UnicodeString m_type;
	int m_UsedCPU, m_TimeSlice;

	TEvent * m_QueueEvent;

protected:

	void __fastcall Execute();

public:

	__fastcall ProcessThread(int prioridade, int timeCPU, int codeProcess, int TimeSlice,
													 UnicodeString type, bool CreateSuspended, TEvent * QE);

	bool Finished;
	int GetTimeCPU();
	int GetCountTime();
	int GetCodeProcess();
	int GetPriority();
	UnicodeString GetType();
	void SetTimeSlice(int TimeSlice);

	TEvent * m_Event;
};
//---------------------------------------------------------------------------
#endif
