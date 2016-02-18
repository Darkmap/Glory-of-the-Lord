#pragma once

//#include "windows.h"

//#include <afx.h>
#include "windows.h"
#include "Mmreg.h"
#include "dsound.h"

#include "stdio.h"
#include <io.h> 
#include <fcntl.h> 
#include <process.h> 
#include <sys\stat.h>
#include <vector>
#include <string>
using namespace std;

struct _WAVEFORMAT  
{   
	short wFormatTag;   //// may be changed
	short wChannels; 
	int dwSamplesPerSec;   
	int dwAvgBytesPerSec;   
	short wBlockAlign;   
	short wBitsPerSample;  
};

struct WAVEFILEHEADER 
{   
	char RIFF_ID[4];   // there is no '/0' to the end
	int RIFF_Size;  
	char RIFF_Type[4];   // there is no '/0' to the end
	char FORMAT_ID[4];   // there is no '/0' to the end
	int FORMAT_Size;  
	_WAVEFORMAT wf; 
	char DATA_ID[4];    // there is no '/0' to the end
	int DATA_Size; 
};


class CWaveFile
{
public:
	CWaveFile(){};
	CWaveFile(char * filepath);
	WAVEFILEHEADER wavefileheader;
	char * data;
};


//struct SoundDesc
//{
//	CWaveFile * wavefile;
//	LPDIRECTSOUNDBUFFER lpDirectSoundBuffer;
//	string sound_name;
//};

struct SoundDesc
{
	DWORD m_dwSize;
	LPWAVEFORMATEX m_pFormat;
	LPBYTE m_pData;
	DWORD * pdwEnd;
	DWORD * pdw;
	DWORD dwRiff;
	DWORD dwLength;
	DWORD dwType;
	LPDIRECTSOUNDBUFFER lpDirectSoundBuffer;
	string sound_name;


	BOOL LoadData (void);
	BOOL LoadFile(char * Filename);
};


class SoundManager
{
public:
	SoundManager(void);
	~SoundManager(void);
	void open(char * filepath,string sound_name);
	void play(string sound_name,int play_type);
	void stop(string sound_name);
	void stopAll();
	void setVolume();
	std::vector<SoundDesc> sound;
	LPDIRECTSOUND8 lpDirectSound;
	bool play_sound;
	long attenuation;
};

