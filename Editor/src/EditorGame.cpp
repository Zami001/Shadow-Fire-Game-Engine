#include <EditorGame.h>
#include <Application.h>
#include <windows.h>

#include <Components/StaticMeshComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Assets/SerializedAsset.h>
#include <MeshImporter.h>

#include <Math/Constants.h>
#include <Math/Quaternion.h>
#include <Profiling/ScopedProfiler.h>

void EditorGame::Initialize() {
	Game::Initialize();

	if (!Project::HasFileAssociation()) {
		Project::SetupFileAssociations();
	}

	//ActiveProject.LoadProject("C:\\Users\\Rune_\\Desktop\\Sample SF Project\\Test Project.SFPROJECT");

	//auto options = Application::GetLaunchOptions();
	//if (options.size() > 0) {
	//	if (std::string(options[0]).find(".SFPROJECT") != ~0) {
	//		ActiveProject.LoadProject(options[0]);
	//	}
	//}
	//
	//if (!ActiveProject.GetProjectLoaded()) {
	//	ActiveProject.CreateProject();
	//}
}

void EditorGame::Serialize(SerializedAsset& asset) {
	Game::Serialize(asset);

	asset.SetAssetType<EditorGame>();
}

RenderPipeline& EditorGame::GetRenderer() {
	return pipeline;
}

void EditorGame::CreateInitialScene() {
	SFWeakPtr<Scene> scene = Game::CreateEmptyScene();

	// todo: initialize the editor environment
	SFSharedPtr<GameObject> obj = scene->SpawnGameObject();

	auto meshcomp = obj->AddComponent<SkeletalMeshComponent>();

	SFSharedPtr<Mesh> debugMesh = GetGameInstance()->GetRenderer().CreateMesh();
	SFSharedPtr<Material> mat = GetGameInstance()->GetRenderer().GetDefaultMaterial();

	ImportedMeshAsset ImportedMesh;
	{
		ScopedProfiler Profiler("Mesh loading");
		MeshImporter importer(GetGameInstance()->GetRenderer());
		
		//importer.ImportFile("C:\\Shadow Fire Game Engine\\Third Party\\assimp-5.2.5\\test\\models\\FBX\\animation_with_skeleton.fbx" , *mesh); //
		importer.ImportFile("C:\\Users\\Rune_\\Desktop\\Sample SF Project\\Content\\Bug\\Basic Bug_4.fbx", ImportedMesh);
		//importer.ImportFile("C:\\Users\\Rune_\\Desktop\\Sample SF Project\\Content\\teapot.fbx", mesh->GetVertexBuffer());
	}

	//mesh->GetSkeleton()->GenerateAnimatedDebugMesh(debugMesh->GetVertexBuffer(), 0);

	SFSharedRef<Texture2D> diffuseTex = GetGameInstance()->GetRenderer().CreateTexture2D();
	diffuseTex->LoadFile("C:\\Users\\Rune_\\Desktop\\Sample SF Project\\Content\\Bug\\BBug_Texture.png");

	mat->Textures.push_back(diffuseTex);

	meshcomp->SetMaterial(mat);
	meshcomp->SetMesh(ImportedMesh.Meshes[0]);
	meshcomp->SetAnimation(ImportedMesh.Animations[7]);
	
	//meshcomp->mesh = debugMesh;
	float scaler = 6.f;
	meshcomp->transform.Scale({ scaler, scaler, scaler });
	meshcomp->transform.Translate({ 0, -25, 100 });
	//meshcomp->transform.Rotate({ 90 * Math::Deg2Rad, 0 * Math::Deg2Rad, 0 * Math::Deg2Rad });
	meshcomp->transform.Rotate(Quaternion(Vector3(0, 35 * Math::Deg2Rad, 0)));


	//GetGameInstance()->GetTickManager().RegisterTick(obj.Get(), [meshcomp, debugMesh, ImportedMesh, animIndex](float deltatime) mutable {
		//InputManager& inputManager = instance->GetWindows()[0]->GetInputManager();
		//if (inputManager.GetButtonDown(Keycode::RightArrow)) {
		//	meshcomp->transform.Translate({ deltatime, 0, 0 });
		//}
		//
		//if (inputManager.GetButtonDown(Keycode::LeftArrow)) {
		//	meshcomp->transform.Translate({ -deltatime, 0, 0 });
		//}
		//meshcomp->transform.Translate({ deltatime, 0, 0 });
		
		
		//meshcomp->transform.Rotate(Quaternion(Vector3(0, deltatime, 0)));

		//target.Meshes[0]->GetSkeleton()->GenerateDebugMesh(debugMesh->GetVertexBuffer());
		//target.Meshes[0]->GetSkeleton()->GenerateAnimatedDebugMesh(debugMesh->GetVertexBuffer(), timer);
		//meshcomp->transform.Rotate({ 0, Math::TAU * 100, 0 });
	//});

	//TimedProfiler fpsTracker("FPS");

	{
		ScopedProfiler serializeprofiler("Serializer");
		SerializedAsset testasset;
		scene->Serialize(testasset);
		testasset.WriteToFile("Test Scene.SFASSET");
	}
}