#ifndef HMapEditor
#define HMapEditor

#include "HMapLoader.h"
#include "HWOWKlient.h"

#define ID_GUI_ADD_BUTTON 100
#define SIZE_GUI_ADD_BUTTON_HEIGHT 20

#define ID_GUI_LISTBOX_MODELS 110

#define ID_GUI_IMAGE_PREVIEW 120
#define SIZE_GUI_IMAGE_PREVIEW_WIDTH 256
#define SIZE_GUI_IMAGE_PREVIEW_HEIGHT 143

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