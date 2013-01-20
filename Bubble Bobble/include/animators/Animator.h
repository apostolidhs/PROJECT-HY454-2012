#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "MemoryManage.h"
#include "utilities.h"

typedef unsigned long timestamp_t;

enum animatorstate_t {
	ANIMATOR_FINISHED = 0,
	ANIMATOR_RUNNING  = 1,
	ANIMATOR_STOPPED  = 2
};

class Animator : public LatelyDestroyable{
public:
	typedef void (*FinishCallback)(Animator*,void*);

protected:

	timestamp_t	lastTime;
	animatorstate_t	state;
	FinishCallback	onFinish;
	void* finishClosure;
	void NotifyStopped (void);

public:
	Animator (void); 
	virtual ~Animator();
	void Stop (void);
	bool HasFinished (void) const;
	virtual void TimeShift (timestamp_t offset);
	virtual void Progress (timestamp_t currTime)=0;
	virtual void Display(Bitmap at)=0;
	void SetOnFinish (FinishCallback f, void* c); 
};

#endif