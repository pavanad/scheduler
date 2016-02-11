//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TProcessThread.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall ProcessThread::ProcessThread(int prioridade, int timeCPU, int codeProcess, int TimeSlice,
										UnicodeString type, bool CreateSuspended, TEvent * QE)
	: TThread(CreateSuspended), m_QueueEvent(QE)
{
	m_type = type;
	m_timeCPU = timeCPU;
	m_prio = prioridade;
	m_codeProcess = codeProcess;
	m_TimeSlice = TimeSlice;

	m_UsedCPU = 0;

	m_Event = new TEvent(false);
	Finished = false;
}
//---------------------------------------------------------------------------
void __fastcall ProcessThread::Execute()
{
	FreeOnTerminate = true;

	while(!Terminated)
	{

		m_Event->WaitFor(INFINITE);
		m_Event->ResetEvent();

		Sleep(1000);

		if (m_type=="CPU Bound") m_TimeSlice++;
		m_UsedCPU += m_TimeSlice;

		m_QueueEvent->SetEvent();
		m_QueueEvent->ResetEvent();

	}
}
//---------------------------------------------------------------------------
int ProcessThread::GetTimeCPU()
{
	return m_timeCPU;
}
//---------------------------------------------------------------------------
UnicodeString ProcessThread::GetType()
{
	return m_type;
}
//---------------------------------------------------------------------------
int ProcessThread::GetCodeProcess()
{
	return m_codeProcess;
}
//---------------------------------------------------------------------------
int ProcessThread::GetCountTime()
{
	return m_UsedCPU;
}
//---------------------------------------------------------------------------
int ProcessThread::GetPriority()
{
	return m_prio;
}
//---------------------------------------------------------------------------
void ProcessThread::SetTimeSlice(int TimeSlice)
{
	m_TimeSlice = TimeSlice;
}
//---------------------------------------------------------------------------
