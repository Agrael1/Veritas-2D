#define TB_IMPL
#include "VLine.h"
#include "Class.h"
#include "TextureBuffer.h"
#include <stdio.h>

void virtual(Bind)(selfptr, struct VLine* gfx)
{
	gfx->PS.TextureBuffer = self->Texture;
}
HRESULT LoadTextureFromFileW(const char* const wFile, ColorMap* map)
{
	FILE* Texture = fopen(wFile, "rb");
	if (!Texture) return E_INVALIDARG;

	DWORD paletteSize;
	fread(&paletteSize, sizeof(DWORD), 1u, Texture);		// check for pallete size
	if (paletteSize > 16) return E_NOT_VALID_STATE;

	char PaletteName[64];
	fscanf(Texture, "%s\n", PaletteName);	// do nothing and go past the table of colors
	fseek(Texture, 16 * sizeof(COLORREF) + sizeof((char)'\n'), SEEK_CUR);

	DWORD width, height;
	fread(&width, sizeof(DWORD), 1u, Texture);
	map->Width = (float)width++;
	fread(&height, sizeof(DWORD), 1u, Texture);
	map->Height = (float)height++;
	map->Stride = width;
	fseek(Texture, sizeof((char)'\n'), SEEK_CUR);

	map->map = malloc(sizeof(CHAR_INFO)*width*height);	// allocate and try buffer
	if (!map->map) return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);

	fread(map->map, sizeof(CHAR_INFO), width*height, Texture);
	return S_OK;
}

VirtualTable{
	.Bind = virtual(Bind)
};
Constructor(selfptr, va_list *ap)
{
	InitializeVtable(self);
	const char* Filename = va_arg(*ap, char*);
	if (FAILED(LoadTextureFromFileW(Filename, &self->Texture)))
	{
		delete(self);
	}
}
Destructor(selfptr)
{
	if(self->Texture.map) free(self->Texture.map);
}
ENDCLASSDESC

extern inline String virtual(GenerateUID)(const char* fileName);
extern inline shared_ptr(Bindable)* virtual(Resolve)(const char* fileName);