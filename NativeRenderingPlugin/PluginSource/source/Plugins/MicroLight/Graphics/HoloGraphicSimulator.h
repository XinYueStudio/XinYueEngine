#pragma once
#include "Resources.h"

enum SimulatorMode
{
	SingleWindow,
	MultiWindow,
};

enum SimulatorSpace
{
	Left0,
	Right0,
	Left1,
	Right1,
};

struct SimulatorRenderData
{
	SimulatorSpace space;
	HANDLE texturehandle;
	Vector2 texturesize;
};

class HoloGraphicSimulator
{
public:
	HoloGraphicSimulator();
	~HoloGraphicSimulator();

	//Set Simulator Window Position
	bool SetSimulatorPosition(Vector2 position);
	//Set Simulator Window Size, x is Width,y is Height
	bool SetSimulatorSize(Vector2 size);
	//Set Simulator Window Parent Window's Handle
	bool SetSimulatorParent(HWND parent);

	//Get Simulator Window Position
	Vector2 GetSimulatorPosition();
	//Get Simulator Window Size, x is Width,y is Height
	Vector2 GetSimulatorSize();
	//Get Simulator Window Parent Window's Handle
	HWND GetSimulatorParent();

 
	//Set Simulator display Mode 
	bool SetSimulatorMode(SimulatorMode mode);

	bool SetSimulatorRendeTexture(SimulatorSpace space, HANDLE texturehandle,Vector2 texturesize);
	
	
	bool StartSimulator();

	bool UpdateSimulator();

	bool StopSimulator();

	 

	HWND  m_ParenthWnd;
	HWND  m_CurrenthWnd;

	SimulatorMode m_SimulatorMode;
	SimulatorRenderData m_FrontRenderData;
	SimulatorRenderData m_FloorRenderData;

	HoloGraphicRenderAPI* m_HoloGraphicRender;

};

