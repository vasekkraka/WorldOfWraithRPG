#ifndef HMapEditor
#define HMapEditor

#include "HMapLoader.h"
#include "HWOWKlient.h"

#define ID_GUI_ADD_BUTTON 100
#define SIZE_GUI_ADD_BUTTON_HEIGHT 20
#define SIZE_GUI_ADD_BUTTON_WIDTH 80

#define ID_GUI_IMAGE_PREVIEW 120
#define SIZE_GUI_IMAGE_PREVIEW_WIDTH 256
#define SIZE_GUI_IMAGE_PREVIEW_HEIGHT 143

#define ID_GUI_LISTBOX_MODELS 110
#define SIZE_GUI_LISTBOX_MODELS_WIDTH 256
#define SIZE_GUI_LISTBOX_MODELS_HEIGHT -(SIZE_GUI_ADD_BUTTON_HEIGHT + SIZE_GUI_IMAGE_PREVIEW_HEIGHT)

#define ID_TOOL_WINDOW 140
#define SIZE_TOOL_WINDOW_WIDTH 272

#define ID_TAB_CONTROL 150
#define SIZE_TAB_CONTROL_WIDTH 268
#define SIZE_TAB_CONTROL_HEIGHT 20

#define SIZE_TAB_WIDTH 256
#define SIZE_TAB_HEIGHT SIZE_TAB_CONTROL_HEIGHT - 0


class MapEditor
{
public:
	MapEditor();
	MapEditor(WowKlient::Core::GameState * state);
	~MapEditor();

	void setGState(WowKlient::Core::GameState * state);
	void runMapEditor();
	bool saveMapXML();
	bool loadMapXML();

private:
	WowKlient::Core::GameState * gState;
};

#endif