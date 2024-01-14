#include <UI/UIImage.h>
#include <UI/Panels/UIRoot.h>
#include <Texture/Texture2D.h>

UIImage::UIImage() {
	VisualElement = true;
}

UIImage::~UIImage() {}

void UIImage::Render(Bounds2Di Bounds, Vector2i Screensize) {
	if (IsDirty()) {
		Bounds2Di UsedBounds = Bounds;

		switch(Scale) {
			case ScaleMode::MaintainAspect: {
				float BoundsAspect = static_cast<float>(Bounds.size.x) / static_cast<float>(Bounds.size.y);
				float ImageAspect = static_cast<float>(material->Textures[0]->GetWidth()) / static_cast<float>(material->Textures[0]->GetHeight());
				if (BoundsAspect > ImageAspect) {
					UsedBounds.size.x = UsedBounds.size.y * ImageAspect;
				} else {
					UsedBounds.size.y = UsedBounds.size.x * (1.f / ImageAspect);
				}
			}

			[[fallthrough]];
			case ScaleMode::Stretch:
				SetBoxRender(UsedBounds, Screensize, { 0.f, 0.f }, { 1.f, 1.f }, mesh->GetVertexBuffer());
				break;

			case ScaleMode::Crop: {
				Vector2i ImageSize = material->Textures[0]->GetSize();

				UsedBounds.size.x = std::min(ImageSize.x, UsedBounds.size.x);
				UsedBounds.size.y = std::min(ImageSize.y, UsedBounds.size.y);
				Vector2 ToUV = { static_cast<float>(UsedBounds.size.x) / static_cast<float>(ImageSize.x), static_cast<float>(UsedBounds.size.y) / static_cast<float>(ImageSize.y) };
				SetBoxRender(UsedBounds, Screensize, { 0.f, 0.f }, ToUV, mesh->GetVertexBuffer());
				break;
			}
			
			case ScaleMode::Repeat: {
				// need to offset UV coords so that image tiles from top-left since standard UI arranges from top-left instead of bottom left
				Vector2i ImageSize = material->Textures[0]->GetSize();
				float xRepeats = static_cast<float>(UsedBounds.size.x) / static_cast<float>(ImageSize.x);
				float yRepeats = static_cast<float>(UsedBounds.size.y) / static_cast<float>(ImageSize.y);
				float xRemainder = std::fmodf(xRepeats, 1.f);
				float yRemainder = std::fmodf(yRepeats, 1.f);
				SetBoxRender(UsedBounds, Screensize, { -xRemainder, -yRemainder }, { xRepeats - xRemainder, yRepeats - yRemainder }, mesh->GetVertexBuffer());
				break;
			}

			case ScaleMode::Sliced: {
				SetSlicedBoxRender(UsedBounds, Screensize, Slices, material->Textures[0]->GetSize(), mesh->GetVertexBuffer());
				break; 
			}
		}

		ClearDirty();
	}

	mesh->Render(Matrix4x4::Identity(), material);
}

void UIImage::Initialize() {
	UIElement::Initialize();

	mesh = GetRoot()->GetRenderPipeline().CreateMesh();
	material = GetRoot()->GetRenderPipeline().CreateMaterial();

	SFSharedRef<Shader> VertShader = GetRoot()->GetRenderPipeline().GetShader("Shaders/UI/Vert.hlsl", ShaderType::Vertex);
	SFSharedRef<Shader> FragShader = GetRoot()->GetRenderPipeline().GetShader("Shaders/UI/Frag.hlsl", ShaderType::Fragment);
	SFSharedRef<Texture2D> Texture = GetRoot()->GetRenderPipeline().GetTexture2D("C:\\Users\\Rune_\\Downloads\\neon_landscape_ultrawide_wallpaper_by_ultrawidewalls_dfzydb3-fullview.jpg");

	material->BindShaders({ VertShader, FragShader });
	material->Textures.push_back(Texture);

	mesh->GetVertexBuffer().SetDescriptors({
		VertexBuffer::VertexDescriptor::Position,
		VertexBuffer::VertexDescriptor::UV_Coordinate
	});
}

Vector2i UIImage::GetDesiredSize() const {
	return material->Textures[0]->GetSize();
}

void UIImage::OnMouseOver() {
	UIElement::OnMouseOver();

	SF_LOG(LogTest, Log, "Mouse over event")
}