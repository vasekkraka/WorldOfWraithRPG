#ifndef HMapEditorEventReciever
#define HMapEditorEventReciever

#include "HMapLoader.h"
#include "HWOWKlient.h"

struct MapSceneContext
{
	IrrlichtDevice *device;
	s32             counter;
	IGUIListBox * listBoxModels;
	IGUIButton * buttonAdd;
};

class MapEditorEventReceiver : public IEventReceiver
{
public:
	MapEditorEventReceiver(MapSceneContext & context) : SceneContext(context) { }

	virtual bool OnEvent(const SEvent& event)
	{
		return false;
	}

private:
	MapSceneContext & SceneContext;
};

#endif