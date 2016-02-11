//---------------------------------------------------------------------------

#ifndef TSchedulerH
#define TSchedulerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include "TProcessThread.h"
#include "TSchedulerProcess.h"
#include "TAbout.h"
#include <Mask.hpp>
#include <ToolWin.hpp>
#include <Graphics.hpp>
#include <vector>
#include <stdio.h>



//---------------------------------------------------------------------------
class TFScheduler : public TForm
{
__published:	// IDE-managed Components
	TImageList *ImageList1;
	TGroupBox *m_GroupProcess;
	TLabel *m_lblPrio;
	TLabel *m_lblType;
	TEdit *m_prioridade;
	TUpDown *m_UpPrio;
	TComboBox *m_TypeProcess;
	TButton *m_CreateProcess;
	TStringGrid *m_status;
	TGroupBox *m_GroupStatusProcess;
	TListView *m_ls;
	TLabel *Label1;
	TEdit *m_timeCPU;
	TUpDown *m_UpTime;
	TLabel *m_lblTimeSlice;
	TEdit *m_TimeSlice;
	TUpDown *m_UpTimeSlice;
	TLabel *Label2;
	TToolBar *ToolBar1;
	TStatusBar *m_StatusBar;
	TImage *m_ImageLow;
	TLabel *m_LabelLow;
	TImage *m_ImageNormal;
	TLabel *m_LabelNormal;
	TImage *m_ImageHigh;
	TLabel *m_LabelHigh;
	TLabel *m_LabelTurnaRound;
	TLabel *m_TurnaRound;
	TToolButton *m_ToolClear;
	TToolButton *m_ToolSave;
	TToolButton *ToolButton1;
	TToolButton *m_ToolAbout;
	void __fastcall m_CreateProcessClick(TObject *Sender);
	void __fastcall m_ToolClearClick(TObject *Sender);
	void __fastcall m_ToolSaveClick(TObject *Sender);
	void __fastcall m_ToolAboutClick(TObject *Sender);
private:	// User declarations

	int RandomCodeProcess();
	SchedulerProcess * myScheduler;

	void SetStatus(int prio, int time, int code);


public:		// User declarations
	__fastcall TFScheduler(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFScheduler *FScheduler;
//---------------------------------------------------------------------------
#endif
