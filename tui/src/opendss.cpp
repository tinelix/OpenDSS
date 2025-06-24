#include <opendss.h>

#include <utils/tuiman.h>

TUIManager* gTuiMan;

int main() {
	
	gTuiMan = new TUIManager();

	gTuiMan->showTopBanner();
	gTuiMan->listenKbd();

	return 0;
}
