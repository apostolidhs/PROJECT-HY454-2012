#ifndef FRAMERANGEANIMATION_H
#define FRAMERANGEANIMATION_H

#include "MovingAnimation.h"

typedef unsigned char frame_t;

class FrameRangeAnimation : public MovingAnimation{
	frame_t start, end;

public:
	FrameRangeAnimation (frame_t s, frame_t e, offset_t dx, offset_t dy, delay_t d, bool c, animid_t id);
	~FrameRangeAnimation(void);
    frame_t	GetStartFrame (void) const;
    void SetStartFrame (frame_t v);
    frame_t	GetEndFrame (void) const;
    void SetEndFrame (frame_t v);
    Animation* Clone (animid_t newId) const;
};

#endif