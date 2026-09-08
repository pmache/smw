#include "global.h"

namespace {
// Serialized strings in SMW are UI/config names and paths, not blobs. Refuse
// absurd declared lengths from corrupt/untrusted legacy files before allocating.
const int kMaxSerializedStringBytes = 1024 * 1024;
}

FILE * OpenFile(const char * filename, const char * options)
{
#ifdef _XBOX
	std::string optionsbin = std::string("D:\\") + std::string(filename);
#else
	#ifdef PREFIXPATH
		char * folder=getenv("HOME");
		if (!folder)
			folder = const_cast<char *>("");
		#ifdef __MACOSX__
			std::string optionsbin=std::string(folder) + std::string("/Library/Preferences/smw.") + std::string(filename);
		#else
			std::string optionsbin=std::string(folder) + std::string("/.smw.") + std::string(filename);
		#endif
	#else
		std::string optionsbin = std::string(filename);
	#endif
#endif

	return fopen(optionsbin.c_str(), options);
}

void WriteInt(int out, FILE * outFile)
{
	if (!outFile)
		return;
#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	int t = out;

    ((char*)&out)[0] = ((char*)&t)[3];
    ((char*)&out)[1] = ((char*)&t)[2];
    ((char*)&out)[2] = ((char*)&t)[1];
    ((char*)&out)[3] = ((char*)&t)[0];
#endif

	fwrite(&out, sizeof(Uint32), 1, outFile);
}

int ReadInt(FILE * inFile)
{
	int in = 0;
	if (!inFile || fread(&in, sizeof(Uint32), 1, inFile) != 1)
		return 0;

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	int t = in;

    ((char*)&in)[0] = ((char*)&t)[3];
    ((char*)&in)[1] = ((char*)&t)[2];
    ((char*)&in)[2] = ((char*)&t)[1];
    ((char*)&in)[3] = ((char*)&t)[0];
#endif

	return in;
}

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
void ReadIntChunk(int * mem, size_t iQuantity, FILE * inFile)
{
	if (!mem || !inFile)
		return;
	for(size_t i = 0; i < iQuantity; i++)
	{
		mem[i] = 0;
		if (fread(&mem[i], sizeof(Uint32), 1, inFile) != 1)
			return;

		int t = mem[i];

		((char*)&mem[i])[0] = ((char*)&t)[3];
		((char*)&mem[i])[1] = ((char*)&t)[2];
		((char*)&mem[i])[2] = ((char*)&t)[1];
		((char*)&mem[i])[3] = ((char*)&t)[0];
	}
}
#else
void ReadIntChunk(int * mem, size_t iQuantity, FILE * inFile)
{
	if (!mem || !inFile)
		return;
	const size_t read = fread(mem, sizeof(Uint32), iQuantity, inFile);
	for (size_t i = read; i < iQuantity; ++i)
		mem[i] = 0;
}
#endif

void WriteFloat(float out, FILE * outFile)
{
	if (!outFile)
		return;
#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	float t = out;

    ((char*)&out)[0] = ((char*)&t)[3];
    ((char*)&out)[1] = ((char*)&t)[2];
    ((char*)&out)[2] = ((char*)&t)[1];
    ((char*)&out)[3] = ((char*)&t)[0];
#endif

	fwrite(&out, sizeof(float), 1, outFile);
}

float ReadFloat(FILE * inFile)
{
	float in = 0.0f;
	if (!inFile || fread(&in, sizeof(float), 1, inFile) != 1)
		return 0.0f;

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	float t = in;

    ((char*)&in)[0] = ((char*)&t)[3];
    ((char*)&in)[1] = ((char*)&t)[2];
    ((char*)&in)[2] = ((char*)&t)[1];
    ((char*)&in)[3] = ((char*)&t)[0];
#endif

	return in;
}

void WriteString(char * szString, FILE * outFile)
{
	if (!szString || !outFile)
		return;
	int iLen = strlen(szString) + 1;
	WriteInt(iLen, outFile);
	fwrite(szString, sizeof(Uint8), iLen, outFile);
}

void ReadString(char * szString, short size, FILE * inFile)
{
	if (!szString || size <= 0)
		return;

	szString[0] = 0;
	if (!inFile)
		return;

	const int iLen = ReadInt(inFile);
	if (iLen <= 0)
		return;

	if (iLen > kMaxSerializedStringBytes)
	{
		// Fail closed on a corrupt length. Do not allocate attacker-controlled
		// memory. Best-effort skip keeps later reads from interpreting string bytes
		// as scalar fields when the underlying FILE supports seeking.
		fseek(inFile, iLen, SEEK_CUR);
		return;
	}

	const int copyLimit = size - 1;
	int copied = 0;
	for (int i = 0; i < iLen; ++i)
	{
		unsigned char ch = 0;
		if (fread(&ch, sizeof(Uint8), 1, inFile) != 1)
			break;
		if (copied < copyLimit && ch != 0)
			szString[copied++] = static_cast<char>(ch);
	}
	szString[copied] = 0;
}

void WriteByteFromShort(short out, FILE * outFile)
{
	if (!outFile)
		return;
	char b = (char)out;
	fwrite(&b, sizeof(Uint8), 1, outFile);
}

short ReadByteAsShort(FILE * inFile)
{
	char b = 0;
	if (!inFile || fread(&b, sizeof(Uint8), 1, inFile) != 1)
		return 0;

	return (short)b;
}

void WriteBool(bool out, FILE * outFile)
{
	if (!outFile)
		return;
	fwrite(&out, sizeof(Uint8), 1, outFile);
}

bool ReadBool(FILE * inFile)
{
	bool b = false;
	if (!inFile || fread(&b, sizeof(Uint8), 1, inFile) != 1)
		return false;

	return b;
}

void WriteByte(Uint8 out, FILE * outFile)
{
	if (!outFile)
		return;
	fwrite(&out, sizeof(Uint8), 1, outFile);
}

Uint8 ReadByte(FILE * inFile)
{
	Uint8 b = 0;
	if (!inFile || fread(&b, sizeof(Uint8), 1, inFile) != 1)
		return 0;

	return b;
}
