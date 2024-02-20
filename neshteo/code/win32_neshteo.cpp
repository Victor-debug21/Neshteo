#include<windows.h>
#include<stdint.h>
#include<xinput.h>
#include<dsound.h>

#define internal static
#define local_persist static
#define global_variable static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#include "neshteo.h"
#include "neshteo.cpp"

struct win32_offscreen_buffer
{
  BITMAPINFO Info;
  int Width;
  int Height;
  void *Memory;
  int Pitch;
};

struct win32_window_dimension
{
  int Width;
  int Height;
};


#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
    return(0);
}
global_variable x_input_get_state *XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_


#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub)
{
    return(0);
}
global_variable x_input_set_state *XInputSetState_ = XInputSetStateStub;
#define XInputSetState XInputSetState_

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);


global_variable bool GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackBuffer;
global_variable LPDIRECTSOUNDBUFFER GlobalSecondaryBuffer;

internal void
Win32LoadXInput(void)    
{
    HMODULE XInputLibrary = LoadLibraryA("xinput1_3.dll");
    if(XInputLibrary)
    {
        XInputGetState = (x_input_get_state *)GetProcAddress(XInputLibrary, "XInputGetState");
        if(!XInputGetState) {XInputGetState = XInputGetStateStub;}

        XInputSetState = (x_input_set_state *)GetProcAddress(XInputLibrary, "XInputSetState");
        if(!XInputSetState) {XInputSetState = XInputSetStateStub;}
    }
}
internal void
Win32InitDSound(HWND Window, int32 SamplesPerSecond, int32 BufferSize)
{
    HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");
    if(DSoundLibrary)
    {
        direct_sound_create *DirectSoundCreate = (direct_sound_create *)
            GetProcAddress(DSoundLibrary, "DirectSoundCreate");

        LPDIRECTSOUND DirectSound;
        if(DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0)))
        {
            WAVEFORMATEX WaveFormat = {};
            WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
            WaveFormat.nChannels = 2;
            WaveFormat.nSamplesPerSec = SamplesPerSecond;
            WaveFormat.wBitsPerSample = 16;
            WaveFormat.nBlockAlign = (WaveFormat.nChannels*WaveFormat.wBitsPerSample) / 8;
            WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec*WaveFormat.nBlockAlign;
            WaveFormat.cbSize = 0;

            if(SUCCEEDED(DirectSound->SetCooperativeLevel(Window, DSSCL_PRIORITY)))
            {
                DSBUFFERDESC BufferDescription = {};
                BufferDescription.dwSize = sizeof(BufferDescription);
                BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;


                LPDIRECTSOUNDBUFFER PrimaryBuffer;
                if(SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &PrimaryBuffer, 0)))
                {
                    HRESULT Error = PrimaryBuffer->SetFormat(&WaveFormat);
                    if(SUCCEEDED(Error))
                    {

                        OutputDebugStringA("Primary buffer format was set.\n");
                    }
                    else
                    {

                    }
                }
                else
                {

                }
            }
            else
            {

            }


            DSBUFFERDESC BufferDescription = {};
            BufferDescription.dwSize = sizeof(BufferDescription);
            BufferDescription.dwFlags = 0;
            BufferDescription.dwBufferBytes = BufferSize;
            BufferDescription.lpwfxFormat = &WaveFormat;
            HRESULT Error = DirectSound->CreateSoundBuffer(&BufferDescription, &GlobalSecondaryBuffer, 0);
            if(SUCCEEDED(Error))
            {
                OutputDebugStringA("Secondary buffer created successfully.\n");
            }
        }
        else
        {

        }
    }
    else
    {

    }
}

internal win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
  win32_window_dimension Result;

  RECT ClientRect;
  GetClientRect(Window, &ClientRect);
  Result.Width = ClientRect.right - ClientRect.left;
  Result.Height = ClientRect.bottom - ClientRect.top;

  return(Result);
}

internal void
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
  if(Buffer->Memory)
    {
      VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

  Buffer->Width = Width;
  Buffer->Height = Height;

  Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
  Buffer->Info.bmiHeader.biWidth = Buffer->Width;
  Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
  Buffer->Info.bmiHeader.biPlanes = 1;
  Buffer->Info.bmiHeader.biBitCount = 32;
  Buffer->Info.bmiHeader.biCompression = BI_RGB;

  int BytesPerPixel = 4;
  int BitmapMemorySize = (Buffer->Width*Buffer->Height)*BytesPerPixel;
  Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
  Buffer->Pitch = Width*BytesPerPixel;
}

internal void
Win32GetBufferInWindow(win32_offscreen_buffer *Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{
  StretchDIBits(DeviceContext,
		0, 0, WindowWidth, WindowHeight,
		0, 0, Buffer->Width, Buffer->Height,
		Buffer->Memory,
		&Buffer->Info,
		DIB_RGB_COLORS,
		SRCCOPY);

}

LRESULT CALLBACK
Win32MainWindowCallBack(HWND Window,
			UINT Message,
			WPARAM WParam,
			LPARAM LParam)
{
  LRESULT Result = 0;
  switch(Message)
    {
    case WM_CLOSE:
      {
	GlobalRunning = false;
      }break;
    case WM_ACTIVATEAPP:
      {
	OutputDebugStringA("WM_ACTIVATEAPP");
      }break;
    case WM_DESTROY:
      {
	GlobalRunning = false;
      }break;
    case WM_SYSKEYUP:
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    case WM_KEYUP:
      {
	uint8 VKCode = WParam;
	bool IsDown = ((LParam & (1 << 30)) == 0);
	bool WasDown = ((LParam & (1 << 31)) != 0);
	if(WasDown != IsDown)
	  {
	    if(VKCode == 'W')
	      {
	      }
	    else if(VKCode == 'S')
	      {
	      }
	    else if(VKCode == 'A')
	      {
	      }
	    else if(VKCode == 'D')
	      {
	      }
	    else if(VKCode == 'Q')
	      {
	      }
	    else if(VKCode == 'E')
	      {
	      }
	    else if(VKCode == VK_SPACE)
	      {
	      }
	    else if(VKCode == VK_ESCAPE)
	      {
		OutputDebugStringA("Escape:");
		if(IsDown)
		  {
		    OutputDebugStringA("IsDown");
		  }
		if(WasDown)
		  {
		    OutputDebugStringA("WasDown");
		  }
		OutputDebugStringA("\n");
	      }
	  }

	int32 AltKeyWasDown = LParam & (1 << 29) != 0;
	if(VKCode == VK_F4 && AltKeyWasDown)
	  {
	    OutputDebugStringA("\t\nAltKeyWasDown");
	    GlobalRunning = false;
	  }
      }break;
    case WM_PAINT:
      {
	PAINTSTRUCT Paint;
	HDC DeviceContext = BeginPaint(Window, &Paint);
	win32_window_dimension Dimension = Win32GetWindowDimension(Window);
	Win32GetBufferInWindow(&GlobalBackBuffer, DeviceContext, Dimension.Width, Dimension.Height);
	EndPaint(Window, &Paint);
      }break;
    default:
      {
	Result = DefWindowProc(Window, Message, WParam, LParam);
      }break;
    }
  return(Result);
}

struct SoundBuffer
{
  int ToneHz;
  int16 ToneVolume;
  uint32 RunningSampleIndex;
  int BytesPerSample;
  int SquareWavePeriod;
  int HalfSquareWavePeriod;
  int SamplesPerSecond;
  int SecondaryBufferSize;
};

		      

int CALLBACK
WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CommandLine,
	int ShowCode)
{
  WNDCLASSA WindowClass = {};
  Win32ResizeDIBSection(&GlobalBackBuffer, 1280, 720);
  WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
  WindowClass.lpfnWndProc = Win32MainWindowCallBack;
  WindowClass.hInstance = Instance;
  WindowClass.lpszClassName = "??????";

  if(RegisterClassA(&WindowClass))
    {
      HWND Window = CreateWindowExA(0,
				    WindowClass.lpszClassName,
				    "????",
				    WS_OVERLAPPEDWINDOW|WS_VISIBLE,
				    CW_USEDEFAULT,
				    CW_USEDEFAULT,
				    CW_USEDEFAULT,
				    CW_USEDEFAULT,
				    0,
				    0,
				    Instance,
				    0);

      if(Window)
	{
	  int XOffset = 0;
	  int YOffset = 0;
	  HDC DeviceContext = GetDC(Window);

	  SoundBuffer SoundBuffer = {};
	  SoundBuffer.BytesPerSample = sizeof(int16)*2;
	  SoundBuffer.RunningSampleIndex = 0;
	  SoundBuffer.ToneVolume = 1000;
	  SoundBuffer.ToneHz = 256;
	  SoundBuffer.SamplesPerSecond = 48000;
	  SoundBuffer.SquareWavePeriod = SoundBuffer.SamplesPerSecond/SoundBuffer.ToneHz;
	  SoundBuffer.HalfSquareWavePeriod = SoundBuffer.SquareWavePeriod/2;
	  SoundBuffer.SecondaryBufferSize = SoundBuffer.SamplesPerSecond*SoundBuffer.BytesPerSample;
	  Win32InitDSound(Window, SoundBuffer.SamplesPerSecond, SoundBuffer.SecondaryBufferSize);
	  GlobalSecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);

	    
	  GlobalRunning = true;
	  while(GlobalRunning)
	    {
	      MSG Message;
	      while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
		  if(Message.message == WM_QUIT)
		    {
		      GlobalRunning = false;
		    }
		  TranslateMessage(&Message);
		  DispatchMessageA(&Message);
		}
	      DWORD PlayCursor;
	      DWORD WriteCursor;
	      if(SUCCEEDED(GlobalSecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor)))
		{
		  DWORD ByteToLock = SoundBuffer.RunningSampleIndex*SoundBuffer.BytesPerSample % SoundBuffer.SecondaryBufferSize;
		  DWORD BytesToWrite;
		  if(ByteToLock == PlayCursor)
                    {
		      BytesToWrite = 0;
                    }
		  else if(ByteToLock > PlayCursor)
                    {
                        BytesToWrite = (SoundBuffer.SecondaryBufferSize - ByteToLock);
                        BytesToWrite += PlayCursor;
                    }
                    else
                    {
                        BytesToWrite = PlayCursor - ByteToLock;
                    }

                    VOID *Region1;
                    DWORD Region1Size;
                    VOID *Region2;
                    DWORD Region2Size;
                    if(SUCCEEDED(GlobalSecondaryBuffer->Lock(ByteToLock, BytesToWrite,
                                                             &Region1, &Region1Size,
                                                             &Region2, &Region2Size,
                                                             0)))
                    {

                        DWORD Region1SampleCount = Region1Size/SoundBuffer.BytesPerSample;
                        int16 *SampleOut = (int16 *)Region1;
                        for(DWORD SampleIndex = 0;
                            SampleIndex < Region1SampleCount;
                            ++SampleIndex)
                        {
                            int16 SampleValue = ((SoundBuffer.RunningSampleIndex++ / SoundBuffer.HalfSquareWavePeriod) % 2) ? SoundBuffer.ToneVolume : -SoundBuffer.ToneVolume;
                            *SampleOut++ = SampleValue;
                            *SampleOut++ = SampleValue;
                        }

                        DWORD Region2SampleCount = Region2Size/SoundBuffer.BytesPerSample;
                        SampleOut = (int16 *)Region2;
                        for(DWORD SampleIndex = 0;
                            SampleIndex < Region2SampleCount;
                            ++SampleIndex)
                        {
                            int16 SampleValue = ((SoundBuffer.RunningSampleIndex++ / SoundBuffer.HalfSquareWavePeriod) % 2) ? SoundBuffer.ToneVolume : -SoundBuffer.ToneVolume;
                            *SampleOut++ = SampleValue;
                            *SampleOut++ = SampleValue;
                        }

                        GlobalSecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);
                    }
                }


	      for (DWORD ControllerIndex = 0;
		   ControllerIndex < XUSER_MAX_COUNT;
		   ++ControllerIndex)
                {
		  XINPUT_STATE ControllerState;
		  if(XInputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
                    {
		      
		      XINPUT_GAMEPAD *Pad = &ControllerState.Gamepad;
		      
		      bool Up = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
		      bool Down = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
		      bool Left = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
		      bool Right = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
		      bool Start = (Pad->wButtons & XINPUT_GAMEPAD_START);
		      bool Back = (Pad->wButtons & XINPUT_GAMEPAD_BACK);
		      bool LeftShoulder = (Pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
		      bool RightShoulder = (Pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
		      bool AButton = (Pad->wButtons & XINPUT_GAMEPAD_A);
		      bool BButton = (Pad->wButtons & XINPUT_GAMEPAD_B);
		      bool XButton = (Pad->wButtons & XINPUT_GAMEPAD_X);
		      bool YButton = (Pad->wButtons & XINPUT_GAMEPAD_Y);
		      
                      
		      int16 StickX = Pad->sThumbLX;
		      int16 StickY = Pad->sThumbLY;
		      
		      XOffset += StickX >> 12;
		      YOffset += StickY >> 12;
                    }
		  else
                    {
              
                    }
                }
	      
	      game_offscreen_buffer Buffer = {};
	      Buffer.Memory = GlobalBackBuffer.Memory;
	      Buffer.Width = GlobalBackBuffer.Width;
	      Buffer.Height = GlobalBackBuffer.Height;
	      Buffer.Pitch = GlobalBackBuffer.Pitch;
	      GameUpdateAndRender(&Buffer, XOffset, YOffset);
	      
	      win32_window_dimension Dimension = Win32GetWindowDimension(Window);
	      Win32GetBufferInWindow(&GlobalBackBuffer, DeviceContext, Dimension.Width, Dimension.Height);

	      XOffset++;
	    }
	}
      else
	{
	}
    }
  else
    {
      
    }


  return(0);
}
