#ifndef HMapEditorEventReciever
#define HMapEditorEventReciever

#include "HMapLoader.h"
#include "HWOWKlient.h"
#include "HMapEditor.h"


class MapEditorEventReceiver : public IEventReceiver
{
public:
	MapEditorEventReceiver(mapSceneContext * context) : SceneContext(context) { }

	virtual bool OnEvent(const SEvent& event);

	bool parseMouse(const SEvent& event);
	bool parseKeyboard(const SEvent& event);
	bool parseGUI(const SEvent& event);

	bool isRotatingPreview()
	{
		return rotating;
	}

	vector2d<s32> getLastMousePos()
	{
		return mousePos;
	}

private:
	mapSceneContext * SceneContext;
	bool rotating = false;
	vector2d<s32> mousePos;
	vector3df metaRotationOld;
};

#endif