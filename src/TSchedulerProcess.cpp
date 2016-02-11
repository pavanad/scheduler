//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TSchedulerProcess.h"
#include "TScheduler.h"
#include "TProcessThread.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall SchedulerProcess::SchedulerProcess(TFScheduler * child, bool CreateSuspended)
	: TThread(CreateSuspended)
{

	m_child = child;

	m_PrioThread[ELow] = &m_ProcessLow;
	m_PrioThread[ENormal] = &m_ProcessNormal;
	m_PrioThread[EHigh] = &m_ProcessHigh;

	m_Mutex = new TMutex(false);

	m_FeedQueue = new TEvent(NULL,true,false,NULL,true);
}
//---------------------------------------------------------------------------
void __fastcall SchedulerProcess::Execute()
{
	FreeOnTerminate = true;

	while (!Terminated)
	{

		m_FeedQueue->WaitFor(INFINITE);
		m_FeedQueue->ResetEvent();

		int sz1, sz2, sz3;

		m_Mutex->Acquire();
		sz1 = m_ProcessHigh.size();
		sz2 = m_ProcessNormal.size();
		sz3 = m_ProcessLow.size();
		m_Mutex->Release();

		Synchronize(&UpdateTimeSlice);

		if (sz1 > 0)
		{
			for(int i = 0; i < sz2; i++) m_ProcessNormal[i]->m_Event->ResetEvent();
			for(int i = 0; i < sz3; i++) m_ProcessLow[i]->m_Event->ResetEvent();
			StaggerProcess(m_ProcessHigh,EHigh);
			continue;
		}
		if (sz2 > 0)
		{
			for(int i = 0; i < sz3; i++) m_ProcessLow[i]->m_Event->ResetEvent();
			StaggerProcess(m_ProcessNormal,ENormal);
			continue;
		}
		if (sz3 > 0)
		{
			StaggerProcess(m_ProcessLow,ELow);
			continue;
		}

	}
}
//---------------------------------------------------------------------------
void SchedulerProcess::SetProcessThread(int Prioridade, int TimeCPU, int CodeProcess,
										UnicodeString TypeProcess)
{
    Synchronize(&UpdateTimeSlice);

	ProcessThread * thread = new ProcessThread(Prioridade, TimeCPU,CodeProcess,m_TimeSlice,
											   TypeProcess,false, m_FeedQueue);

	m_Mutex->Acquire();
	switch(Prioridade)
	{
		case 1:
		{
			m_ProcessLow.push_back(thread);
			break;
		}
		case 2:
		{
			m_ProcessNormal.push_back(thread);
			break;
		}
		case 3:
		{
			m_ProcessHigh.push_back(thread);
			break;
		}
	}
	m_Mutex->Release();
	m_FeedQueue->SetEvent();
}
//---------------------------------------------------------------------------
void __fastcall SchedulerProcess::UpdateTimeSlice()
{
	int row = m_child->m_status->RowCount;
	m_TimeSlice = StrToInt(m_child->m_TimeSlice->Text);

	int average = row-1;
	int SumTimeSlice = 0;
	int TurnaRound = 0;

	for (int i = 1; i < row-1; i++)
	{
		if (m_child->m_status->Cells[2][i]!="Finalizado")
		{
			TurnaRound += m_TimeSlice;
			SumTimeSlice += TurnaRound;
		}
		else
		{
            average--;
        }
	}
	m_child->m_TurnaRound->Caption = IntToStr(SumTimeSlice/average) + " u.t.";
}
//---------------------------------------------------------------------------
void SchedulerProcess::StaggerProcess(std::vector<ProcessThread *> & Queue, EPriority prio)
{

	int sz = Queue.size();

	for (int i = 0; i < sz; i++)
	{

		int timeCPU = Queue[i]->GetTimeCPU();

		if (Queue[i]->GetCountTime() < timeCPU)
		{
			//envia status
			m_status = "Execução";
			m_code = Queue[i]->GetCodeProcess();
			m_priority = Queue[i]->GetPriority();
			Synchronize(&UpdateStatus);

			//envia sinal para iniciar o thread
			Queue[i]->m_Event->SetEvent();
			Sleep(m_TimeSlice*1000);

			//envia status
			m_status = "Pronto";
			m_code = Queue[i]->GetCodeProcess();
			m_priority = Queue[i]->GetPriority();
			Synchronize(&UpdateStatus);

			//aguarda o time slice e suspende o thread
			Queue[i]->m_Event->SetEvent();

			Sleep(200);
		}
		else
		{
			//envia status
			m_status = "Finalizado";
			m_code = Queue[i]->GetCodeProcess();
			m_priority = Queue[i]->GetPriority();
			Synchronize(&UpdateStatus);

			Queue[i]->Finished = true;
		}

		m_FeedQueue->SetEvent();

	}

	std::vector<ProcessThread *> nQueue;

	for(int i = 0; i < Queue.size(); i++)
	{
		if(!Queue[i]->Finished)
		{
			nQueue.push_back(Queue[i]);
		}
		else
		{
			Queue[i] = NULL;
		}
	}
	if(Queue.size() != nQueue.size())
	{
		Queue.resize(nQueue.size());
		for(int i = 0; i < nQueue.size(); i++)
		{
			Queue[i] = nQueue[i];
		}
	}

	m_FeedQueue->SetEvent();

}
//---------------------------------------------------------------------------
int SchedulerProcess::GetPosThread(int code, EPriority prio)
{
	std::map<EPriority,std::vector<ProcessThread*> *>::iterator it;
	it = m_PrioThread.find(prio);
	if (it!=m_PrioThread.end())
	{
		for (int i = 0; i < it->second->size(); i++)
		{
			if (it->second->operator [](i)->GetCodeProcess()==code)
			{
				return i;
			}
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
void __fastcall SchedulerProcess::UpdateStatus()
{
	int row = m_child->m_status->RowCount;

	for (int i = 1; i < row-1; i++)
	{
		if (m_child->m_status->Cells[0][i]==IntToStr(m_code))
		{
			m_child->m_status->Cells[2][i] = m_status;
		}
	}

	TListItem * list = m_child->m_ls->Items->Add();
	list->Caption = IntToStr(m_code);
	list->SubItems->Add("Estado do Processo alterado para - " + m_status);
	list->ImageIndex = m_priority;
	list->MakeVisible(true);
	list->Selected = true;

}
//---------------------------------------------------------------------------
