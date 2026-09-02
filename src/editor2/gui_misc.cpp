#include <editor/editor.h>
#include <editor/gui_misc.h>

TabResultTag TopBar() {
	TabResultTag result = None;
	if (BeginMenuBar()) {
		if (MenuItem("Save"))result= Save;
		if (MenuItem("Load"))result= Load;
		EndMenuBar();
	}
	return result;
}