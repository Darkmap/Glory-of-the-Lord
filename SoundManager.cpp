#include "SoundManager.h"


BOOL SoundDesc::LoadData(void)
{
	LPVOID lpPtr1, lpPtr2;
	DWORD dwLen1, dwLen2;
	HRESULT hResult;

TryLoad:
	hResult = lpDirectSoundBuffer->Lock(0, m_dwSize, &lpPtr1, &dwLen1, &lpPtr2, &dwLen2, 0);

	if (hResult == DS_OK)
	{
		memcpy(lpPtr1, m_pData, dwLen1);

		if (lpPtr2 == DS_OK)
			memcpy(lpPtr2, m_pData + dwLen1, dwLen2);

		lpDirectSoundBuffer->Unlock(lpPtr1, dwLen1, lpPtr2, dwLen2);
		return TRUE;
	}
	else 
	if (hResult == DSERR_BUFFERLOST)
	{
		hResult = lpDirectSoundBuffer->Restore();
		if (hResult == DS_OK)
			goto TryLoad;
	}
	return FALSE;
}



BOOL SoundDesc::LoadFile(char * Filename)
{

	FILE * file = fopen(Filename,"r");
	char RIFF_ID[4];
	int RIFF_Size;  
	read(fileno(file),&RIFF_ID,4);
	read(fileno(file),&RIFF_Size,4);
	DWORD dwSize = RIFF_Size+8;
	void * m_pMemory = malloc(dwSize); 
	fseek(file, 0, SEEK_SET);
	read(fileno(file),m_pMemory,dwSize);
	fclose(file);

	LPDWORD pdw，pdwEnd;
	DWORD dwRiff,dwType, dwLength;

	if (m_pFormat) //格式块指针，类型：LPWAVEFORMATEX
		m_pFormat = NULL;
	if (m_pData) //数据块指针，类型：LPBYTE
		m_pData = NULL;
	if (m_dwSize) //数据长度，类型：DWORD
		m_dwSize = 0;


	pdw = (DWORD *) m_pMemory;
	dwRiff = *pdw++;
	dwLength = *pdw++;
	dwType = *pdw++;

	if (dwRiff != mmioFOURCC ('R', 'I', 'F', 'F'))
		return FALSE;
	if (dwType != mmioFOURCC ('W', 'A', 'V', 'E'))
		return FALSE;

	//寻找格式块，数据块位置及数据长度
	pdwEnd = (DWORD *)((BYTE *) pdw + dwLength - 4);
	while (pdw < pdwEnd){
		dwType = *pdw++;
		dwLength = *pdw++;
		switch (dwType){
		case mmioFOURCC('f', 'm', 't', ' '):
			if (!m_pFormat){
				if (dwLength < sizeof (WAVEFORMAT))
					return FALSE;
				m_pFormat = (LPWAVEFORMATEX) pdw;
				if (m_pData && m_dwSize)
					return TRUE;
			}
			break;
		case mmioFOURCC('d', 'a', 't', 'a'):
			if (!m_pData || !m_dwSize){
				m_pData = (LPBYTE) pdw;
				m_dwSize = dwLength;
				if (m_pFormat)
					return TRUE;
			}
			break;
		}
		pdw = (DWORD *)((BYTE *) pdw + 
			((dwLength + 1) &~ 1));
	}
	//未找到，返回FALSE
	return FALSE;
}





CWaveFile::CWaveFile(char * filepath)
{
	FILE * file = fopen(filepath,"r");

	read(fileno(file),&(wavefileheader.RIFF_ID),sizeof(wavefileheader.RIFF_ID));
	read(fileno(file),&(wavefileheader.RIFF_Size),sizeof(wavefileheader.RIFF_Size));
	read(fileno(file),&(wavefileheader.RIFF_Type),sizeof(wavefileheader.RIFF_Type));
	read(fileno(file),&(wavefileheader.FORMAT_ID),sizeof(wavefileheader.FORMAT_ID));
	read(fileno(file),&(wavefileheader.FORMAT_Size),sizeof(wavefileheader.FORMAT_Size));
	read(fileno(file),&(wavefileheader.wf.wFormatTag),sizeof(wavefileheader.wf.wFormatTag));
	read(fileno(file),&(wavefileheader.wf.wChannels),sizeof(wavefileheader.wf.wChannels));
	read(fileno(file),&(wavefileheader.wf.dwSamplesPerSec),sizeof(wavefileheader.wf.dwSamplesPerSec));
	read(fileno(file),&(wavefileheader.wf.dwAvgBytesPerSec),sizeof(wavefileheader.wf.dwAvgBytesPerSec));
	read(fileno(file),&(wavefileheader.wf.wBlockAlign),sizeof(wavefileheader.wf.wBlockAlign));
	read(fileno(file),&(wavefileheader.wf.wBitsPerSample),sizeof(wavefileheader.wf.wBitsPerSample));
	read(fileno(file),&(wavefileheader.DATA_ID),sizeof(wavefileheader.DATA_ID));
	read(fileno(file),&(wavefileheader.DATA_Size),sizeof(wavefileheader.DATA_Size));
	//read(fileno(file),data,wavefileheader.DATA_Size);
	data = new char[wavefileheader.DATA_Size];
	read(fileno(file),data,wavefileheader.DATA_Size);
	fclose(file);
}



SoundManager::SoundManager(void)
{
	/// 创建一个设备对象
	if( DirectSoundCreate8(NULL, & lpDirectSound, NULL) != DS_OK )
		MessageBoxA(0,"DirectSoundCreate8 Failed!","aa",0);

	HWND hWnd = CreateWindow("BUTTON", "aaa", 0, 0,0,0,0, 0, 0, 0, 0);
	///  设置协作水平
	if( lpDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY) != DS_OK )
		MessageBoxA(0,"SetCooperativeLevel Failed!","aa",0);

	play_sound = false;
	attenuation = 0;
}


void SoundManager::open(char * filepath,string sound_name)
{	
	SoundDesc sounddesc;	
	
	sounddesc.LoadFile(filepath);
	
	////////////////////////////////////////////////////////
	DSBUFFERDESC BufferDesc;
	memset (&BufferDesc, 0, sizeof (BufferDesc));
	BufferDesc.lpwfxFormat = (LPWAVEFORMATEX) (sounddesc.m_pFormat);
	BufferDesc.dwSize = sizeof (DSBUFFERDESC);
	BufferDesc.dwBufferBytes = sounddesc.m_dwSize;
	BufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;

	////////////////////////////////////////////////////////
	
	sound.push_back(sounddesc);

	lpDirectSound->CreateSoundBuffer(&BufferDesc,&(sound[sound.size()-1].lpDirectSoundBuffer),NULL);
	
	sound[sound.size()-1].sound_name = sound_name;

}



/// play_type = 1 ,则只播放一次
///play_type = 0,则循环播放，直到调用stop方法来停止
void SoundManager::play(string sound_name,int play_type)
{
	if(play_sound)
	{
		for(int i = 0; i<sound.size(); i++)
		{
			if( sound[i].sound_name.compare(sound_name) == 0 )
			{
				/// 播放
				sound[i].LoadData();
				sound[i].lpDirectSoundBuffer->SetCurrentPosition(0);

				if(play_type==1)
					sound[i].lpDirectSoundBuffer->Play(0,0,0);
				else
				if(play_type==0)
					sound[i].lpDirectSoundBuffer->Play(0,0,DSBPLAY_LOOPING);

				break;
			}
		}
	}
	
}

void SoundManager::stop(string sound_name)
{
	for(int i = 0; i<sound.size(); i++)
	{
		if( sound[i].sound_name.compare(sound_name) == 0 )
		{
			sound[i].lpDirectSoundBuffer->Stop();
			break;
		}
	}
}

void SoundManager::stopAll()
{
	for(int i = 0; i<sound.size(); i++)
	{
		sound[i].lpDirectSoundBuffer->Stop();
	}
}

void SoundManager::setVolume()
{
	HRESULT result;
	for(int i = 0; i<sound.size(); i++)
	{
		result = sound[i].lpDirectSoundBuffer->SetVolume(attenuation);
		if(result==DSERR_INVALIDPARAM)
			MessageBoxA(0,"DSERR_INVALIDPARAM","DSERR_INVALIDPARAM",0);
	}
	
}


SoundManager::~SoundManager(void)
{
}
