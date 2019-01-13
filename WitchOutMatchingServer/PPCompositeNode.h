#pragma once
#include "PPGroup.h"

namespace PP {
	class PPCompositeNode : public PP::PPNode {
	public:
		std::map<int, PP::PPGroup*> listChildren;
	public:
		PPCompositeNode();
		virtual ~PPCompositeNode();
	public :
	};
}