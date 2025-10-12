#pragma once

class IObjectEnterWorld
{
	friend class CWorld;
	virtual void EnterWorld(class CWorld& InWorld, class CObject& InEnterObject) = 0;
};

class IObjectExitWorld
{
	friend class CWorld;
	virtual void ExitWorld(class CWorld& InWorld, class CObject& InExitObject) = 0;
};