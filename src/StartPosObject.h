#ifndef __STARTPOSOBJECT_H__
#define __STARTPOSOBJECT_H__

#include <gd.h>

namespace gd {
	class StartPosObject {
	public:

		float getAudioScale() {
			return from<float>(this, 0x1AC);
		}
	};
}

#endif
