#ifndef __GUIBUTTON__
#define __GUIBUTTON__

#include "j1Module.h"
#include"j1GUIelement.h"


class GUIButton : public j1GUIelement
{
public:

	GUIButton();
	~GUIButton();


	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();



	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };

	void Dragging();
	void MovingIt(float dt);
	void OnRelease();

private:

	j1GUIelement* label = nullptr;
	iPoint accuratedDrag = { 0,0 };

	bool dragging;
};






#endif // !__GUIBUTTON__
