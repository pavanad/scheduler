//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TScheduler.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFScheduler *FScheduler;
//---------------------------------------------------------------------------
__fastcall TFScheduler::TFScheduler(TComponent* Owner)
	: TForm(Owner)
{
	m_status->Cols[0]->Text = "PID";
	m_status->Cols[1]->Text = "Prioridade";
	m_status->Cols[2]->Text = "Estado";
	m_status->Cols[3]->Text = "Tempo UCP";

	myScheduler = new SchedulerProcess(this,true);
	myScheduler->Start();
}
//---------------------------------------------------------------------------
void __fastcall TFScheduler::m_CreateProcessClick(TObject *Sender)
{

	int typeProcess = m_TypeProcess->ItemIndex;

	if (typeProcess<0)
	{
		MessageBox(NULL,"Por favor selecione o tipo de processo","Escalonador",
				   MB_OK | MB_ICONINFORMATION);
		return;
	}
	else
	{
		if (m_prioridade->Text.IsEmpty() || StrToInt(m_prioridade->Text)>3)
		{
			MessageBox(NULL,"Por favor defina uma prioridade válida para o processo","Escalonador",
				   MB_OK | MB_ICONINFORMATION);
			m_prioridade->SetFocus();
				   return;
		}
		if (m_timeCPU->Text.IsEmpty())
		{
			MessageBox(NULL,"Por favor defina o tempo de processador","Escalonador",
				   MB_OK | MB_ICONINFORMATION);
			m_timeCPU->SetFocus();
				   return;
		}

		int priority = StrToInt(m_prioridade->Text);
		int timeCPU = StrToInt(m_timeCPU->Text);
		int codeProcess = RandomCodeProcess();
		UnicodeString TypeProcess = m_TypeProcess->Text;

		myScheduler->SetProcessThread(priority,timeCPU,codeProcess,TypeProcess);
		SetStatus(priority,timeCPU,codeProcess);

	}
}
//---------------------------------------------------------------------------
int TFScheduler::RandomCodeProcess()
{
	UnicodeString count = IntToStr(rand()*6);

	while (count.Length()!=4)
	{
		count = IntToStr(rand()*6);
	}

	return StrToInt(count);
}
//---------------------------------------------------------------------------
void TFScheduler::SetStatus(int prio, int time, int code)
{
	int row = m_status->RowCount;

	m_status->Cells[0][row-1] = code;
	m_status->Cells[1][row-1] = prio;
	m_status->Cells[2][row-1] = "Pronto";
	m_status->Cells[3][row-1] = time;

	m_status->RowCount++;

	TListItem * list = m_ls->Items->Add();
	list->Caption = IntToStr(code);
	list->SubItems->Add("Processo em estado de criação");
	list->ImageIndex = prio;
	list->MakeVisible(true);
	list->Selected = true;
}
//---------------------------------------------------------------------------
void __fastcall TFScheduler::m_ToolClearClick(TObject *Sender)
{
	int count = 0;
	int row = m_status->RowCount;

	for (int i = 1; i < row; i++)
	{
		if (m_status->Cells[2][i]=="Finalizado") count++;
	}

	if (count==row-2)
	{
		for (int i = 1; i < m_status->RowCount; i++)
		{
        	m_status->Rows[i]->Clear();
		}
		m_status->RowCount = 2;
		m_ls->Clear();

		MessageBox(NULL,"Processos limpos com sucesso.","Escalonador",MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(NULL,"Aguarde a finalização dos processos.","Escalonador",MB_OK | MB_ICONEXCLAMATION);
		return;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFScheduler::m_ToolSaveClick(TObject *Sender)
{
	UnicodeString filename;
	int count = m_ls->Items->Count;

	if (count!=0)
	{
		TSaveDialog * save = new TSaveDialog(NULL);
		save->Filter = "Arquivos de texto (*.txt)|*.txt";
		save->FilterIndex = 0;
		save->DefaultExt = "txt";
		save->Options << ofOverwritePrompt;

		if (save->Execute())
		{
			filename = save->FileName;
		}

		if (!filename.IsEmpty())
		{
			FILE * fileStatus;

			fileStatus = fopen(filename.t_str(),"w");
			fprintf(fileStatus,"\nArquivo de Log do Escalonador\n");
			fprintf(fileStatus,"Fila Circular com Prioridades Estáticas\n");
			fprintf(fileStatus,"---------------------------------------\n\n");

			fprintf(fileStatus,"Gerência de Processos:\n\n");

			UnicodeString item;
			int rows = m_status->RowCount;

			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					UnicodeString tab = "\t";
					if (i==0 && j==2) tab="\t\t";
					if (i>=1 && j==1) tab="\t\t";

					item += m_status->Cells[j][i] + tab;
				}

				item += "\n";
				fprintf(fileStatus,item.t_str());
				item = "";
			}

			fprintf(fileStatus,"\nStatus de Processos:\n\n");

			int rowList = m_ls->Items->Count;

			UnicodeString titleItem = m_ls->Column[0]->Caption + "\t" +
									  m_ls->Column[1]->Caption + "\n";
			fprintf(fileStatus,titleItem.t_str());

			for (int i = 0; i < rowList; i++)
			{
				UnicodeString itemList = m_ls->Items->Item[i]->Caption;
				UnicodeString subitemList = m_ls->Items->Item[i]->SubItems->Text;
				UnicodeString lineItem = itemList + "\t\t" + subitemList;
				fprintf(fileStatus,lineItem.t_str());
			}

			fclose(fileStatus);

			MessageBox(NULL,"Arquivo gerado com sucesso.","Escalonador",MB_OK|MB_ICONINFORMATION);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFScheduler::m_ToolAboutClick(TObject *Sender)
{
	TAbout * about = new TAbout(this);
	about->ShowModal();
}
//---------------------------------------------------------------------------

