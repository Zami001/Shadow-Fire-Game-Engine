#include <Editor.h>

#include <Application.h>
#include <EditorGame.h>

int main(int argc, char** argv) {
	EditorGame editor;

	Application::Init(argc, argv);

	Application::Run(editor);

	Application::Terminate();
}