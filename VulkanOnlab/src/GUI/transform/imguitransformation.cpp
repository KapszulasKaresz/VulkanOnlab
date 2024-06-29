#include "GUI/transform/imguitransformation.h"
#include "GUI/object/imguiobject.h"

int ImGuiTransformation::rotatingId = 0;

void ImGuiTransformation::drawDelete()
{
	if (ImGui::Button("Delete")) {
		parent->remove(this);
	}
}
