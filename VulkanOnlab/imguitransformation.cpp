#include "imguitransformation.h"
#include "imguiobject.h"

int ImGuiTransformation::rotatingId = 0;

void ImGuiTransformation::drawDelete()
{
	if (ImGui::Button("Delete")) {
		parent->remove(this);
	}
}
