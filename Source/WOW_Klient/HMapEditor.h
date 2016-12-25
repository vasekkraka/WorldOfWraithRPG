#ifndef HMapEditor
#define HMapEditor

#include "HMapLoader.h"
#include "HWOWKlient.h"

#define ID_TOOL_WINDOW 140
#define SIZE_TOOL_WINDOW_WIDTH 268
#define SIZE_TOOL_WINDOW_HEIGHT 720

#define ID_TAB_CONTROL 150
#define SIZE_TAB_CONTROL_WIDTH 264
#define SIZE_TAB_CONTROL_LEFT 4
#define SIZE_TAB_CONTROL_TOP 25
#define SIZE_TAB_CONTROL_HEIGHT SIZE_TOOL_WINDOW_HEIGHT - 5

#define SIZE_TAB_WIDTH SIZE_TAB_CONTROL_WIDTH - 0
#define SIZE_TAB_LEFT 0
#define SIZE_TAB_RIGHT SIZE_TAB_WIDTH - 0
#define SIZE_TAB_HEIGHT SIZE_TAB_CONTROL_HEIGHT - 0

#define ID_GUI_LISTBOX_MODELS 110
#define SIZE_GUI_LISTBOX_MODELS_WIDTH 256
#define SIZE_GUI_LISTBOX_MODELS_HEIGHT 450

#define ID_GUI_ADD_BUTTON 100
#define SIZE_GUI_ADD_BUTTON_HEIGHT 20
#define SIZE_GUI_ADD_BUTTON_WIDTH 80
#define SIZE_GUI_ADD_BUTTON_TOP SIZE_TAB_CONTROL_TOP + SIZE_GUI_LISTBOX_MODELS_HEIGHT

#define ID_GUI_IMAGE_PREVIEW 120
#define SIZE_GUI_IMAGE_PREVIEW_WIDTH 256
#define SIZE_GUI_IMAGE_PREVIEW_HEIGHT 143
#define SIZE_GUI_IMAGE_PREVIEW_TOP SIZE_TAB_CONTROL_TOP + SIZE_GUI_LISTBOX_MODELS_HEIGHT + SIZE_GUI_ADD_BUTTON_HEIGHT

#define ID_GUI_TOOLBAR 160
#define ID_GUI_TOOLBAR_BUTTON_OPEN 210
#define ID_GUI_TOOLBAR_BUTTON_NEW 200
#define ID_GUI_TOOLBAR_BUTTON_WMO 220
#define ID_GUI_TOOLBAR_BUTTON_NPC 230
#define ID_GUI_TOOLBAR_BUTTON_SAVE 240
#define ID_GUI_TOOLBAR_BUTTON_ROUTE 250

struct nodeProperties
{
	s32 nodeId;
	bool isTerrain;
	bool isTargetable;
};

struct mapNode
{
	IMeshSceneNode * node;
	nodeProperties * properties;
};

struct mapSceneContext
{
	IrrlichtDevice *device;
	s32             counter;
	WowKlient::Core::GameState * gState;
	IGUIWindow * toolBox;
	IGUIListBox * listBoxModels;
	IGUIButton * buttonAdd;
	IAnimatedMeshSceneNode * previewNode;
	IMeshSceneNode * metaNode;
	IGUIImage * previewImage;
	ICameraSceneNode * cameraNodeMaya;
	ICameraSceneNode * cameraNodeModelPreview;
	irr::core::list<mapNode*> mapNodes;
};


class MapEditor
{
public:
	MapEditor();
	MapEditor(WowKlient::Core::GameState * state);
	~MapEditor();

	void setGState(WowKlient::Core::GameState * state);
	
	void runMapEditor();
	IGUIToolBar * createToolbar(mapSceneContext * sceneContext);
	void prepairModelsList(mapSceneContext * sceneContext);	
	bool saveMapXML();
	bool loadMapXML();
	bool saveServerData();
	bool loadServerData();
	
	
private:
	WowKlient::Core::GameState * gState;
};

void initializeCamera(mapSceneContext * sceneContext);

#endif