#include <formatio.h>
#include <advanlys.h>
#include <cvirte.h>		
#include <userint.h>
#include "experiment 5.h"
#include <ansi_c.h>
#include "tktds2xx.h"
static float final_file[300000];
static int panelHandle, ctrlarray;
static int plothandle;
static int plothandle2;
static ViInt16 arrayreadbuffer[10800];
static ViInt16 numbytesread[256];
static ViChar preamble[512];
static ViInt32 bytesread;
static ViChar readbuffer[10800];
static ViSession instrumenthandle;
static int data[2500];
static int m;
static double copy_data[2500];
static int index_array[2500];
static int x_array[2500];
static int y_array[2500];
static char final[300];

static int panelHandle,i;


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "experiment 5.uir", PANEL)) < 0)
		return -1;
		tktds2xx_init ("GPIB::1", VI_ON, VI_OFF, 9600, 0, 20000, &instrumenthandle);
	// ctrlarray = GetCtrlArrayFromResourceID (panelHandle, CTRLARRAY);
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK bye (int panel, int control, int event,
					 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK datafromscope (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			float d1 = 0.0;
			float d2 = 0.0;
			int count = 4;
			tktds2xx_readWfmArray (instrumenthandle, 0, 1, 2500, preamble, arrayreadbuffer, numbytesread);
			x_array[0] = 0;
			for (i=0;i<2500;i++)
			{
			     data[i]= arrayreadbuffer[i]*500/256; // change from VIint into int by a scale 500/256, but the scale is not matter
				 //x_array[i+1] = x_array[i] + 50/1000/2500;
			}
			//printf("%dd\n",arrayreadbuffer[i]);   }
			if (plothandle){DeleteGraphPlot (panelHandle, PANEL_GRAPH, plothandle,VAL_IMMEDIATE_DRAW);  }
			plothandle = PlotWaveform (panelHandle, PANEL_GRAPH, data, 2500, VAL_INTEGER, 1.0, 0.0, 0.0, 1.0, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			for( i = 0 ; i< 2500; i++)
			{
				if (i+1 < 2500 && i-1 >=0)
				{
					d1 = (data[i+1]-data[i]);
					d2 = (data[i]-data[i-1]);
					if(d1*d2 < 0 && data[i] >= data[i+1] && data[i]>=data[i-1])
					{
						
						//printf("%d\t",i);
						//printf("%d\n",data[i]);
					}
						
				}		
			}		
			
			
			break;
	}
	return 0;
}



int CVICALLBACK savetofile (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			for( i = 0 ; i < 2500 ; i++)
			{
				final_file[i] = i;
				final_file[i+2500] = data[i];
			}
			FileSelectPopupEx ("", "*.txt*", "*.txt", "", VAL_SAVE_BUTTON, 0, 0, final);
			ArrayToFile (final, final_file, VAL_FLOAT, 5000, 2, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 10, VAL_ASCII, VAL_TRUNCATE);
			
	}
	return 0; 
 	//printf("Peak numbers are %d",peaks);
}

