#include "PSBase.h"
#include "TextureBuffer.h"
#include "Class.h"

#include <stdio.h>

void virtual(Bind)(void* self, struct VLine* gfx)
{
	account(self);
	gfx->PS->TextureBuffer[this->slot] = this->Texture;
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

	CHAR_INFO x[16];
	fread(map->map, sizeof(CHAR_INFO), width*height, Texture);
	return S_OK;
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	self->Bind = virtual(Bind);
	const char* Filename = va_arg(*ap, char*);
	if (FAILED(LoadTextureFromFileW(Filename, &this->Texture)))
	{
		delete(this);
		return nullptr;
	}
	this->slot = va_arg(*ap, unsigned);
	return self;
}
Destructor(selfptr)
{
	if(self->Texture.map) free(self->Texture.map);
	return self;
}
ENDCLASSDESC