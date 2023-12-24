#include <UI/UIImage.h>
#include <UI/Panels/UIRoot.h>
#include <Texture/Texture2D.h>

UIImage::UIImage() {
	VisualElement = true;
}

UIImage::~UIImage() {}

void UIImage::Render(Bounds2Di Bounds, Vector2i Screensize) {
	struct Vertex {
		Vector3 position;
		Vector2 UV;
	};

	if (IsDirty()) {
		Vertex* verts = new Vertex[4];
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
				for (int i = 0; i < 4; ++i) {
					verts[i].UV.x = static_cast<float>(i % 2);
					verts[i].UV.y = static_cast<float>(i / 2);
					int x, y;
					x = (i % 2) ? (UsedBounds.position.x + UsedBounds.size.x) : UsedBounds.position.x;
					y = (i / 2) ? UsedBounds.position.y : (UsedBounds.position.y + UsedBounds.size.y);
					verts[i].position = ScreenPosToScreenSpace(Vector2i(x, y), Screensize);
				}
				break;

			case ScaleMode::Crop: {
				Vector2i ImageSize = material->Textures[0]->GetSize();

				for (int i = 0; i < 4; ++i) {
					verts[i].UV.x = std::min(static_cast<float>(i % 2), static_cast<float>(UsedBounds.size.x) / static_cast<float>(ImageSize.x));
					verts[i].UV.y = std::min(static_cast<float>(i / 2), static_cast<float>(UsedBounds.size.y) / static_cast<float>(ImageSize.y));
					int x, y;
					x = (i % 2) ? std::min(UsedBounds.position.x + UsedBounds.size.x, ImageSize.x) : UsedBounds.position.x;
					y = (i / 2) ? UsedBounds.position.y : std::min(UsedBounds.position.y + UsedBounds.size.y, ImageSize.y);
					verts[i].position = ScreenPosToScreenSpace(Vector2i(x, y), Screensize);
				}
				break;
			}

			case ScaleMode::Repeat: {
				Vector2i ImageSize = material->Textures[0]->GetSize();

				for (int i = 0; i < 4; ++i) {
					verts[i].UV.x = static_cast<float>(i % 2) * (UsedBounds.size.x / ImageSize.x);
					verts[i].UV.y = static_cast<float>(i / 2) * (UsedBounds.size.y / ImageSize.y);
					int x, y;
					x = (i % 2) ? (UsedBounds.position.x + UsedBounds.size.x) : UsedBounds.position.x;
					y = (i / 2) ? UsedBounds.position.y : (UsedBounds.position.y + UsedBounds.size.y);
					verts[i].position = ScreenPosToScreenSpace(Vector2i(x, y), Screensize);
				}
				break;
			}
		}

		mesh->GetVertexBuffer().SetVertexData(verts, 4);

		VertexBuffer::IndexType* indices = new VertexBuffer::IndexType[6];
		indices[0] = 2;
		indices[1] = 1;
		indices[2] = 0;
		indices[3] = 1;
		indices[4] = 2;
		indices[5] = 3;
	
		mesh->GetVertexBuffer().SetIndices(indices, 6);

		delete[] verts;
		delete[] indices;

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
