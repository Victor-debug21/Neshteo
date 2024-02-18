#include "neshteo.h"

internal void
RenderGradient(game_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
{
  uint8 *Row = (uint8 *)Buffer->Memory;
  for(int Y = 0; Y < Buffer->Height; ++Y)
    {
      uint32 *Pixel = (uint32 *)Row;
      for(int X = 0; X < Buffer->Width; ++X)
	{
	  uint8 Blue = (X + BlueOffset);
	  uint8 Green = (Y + GreenOffset);

	  *Pixel++ = ((Green << 8) | Blue);
	  
	}
      Row += Buffer->Pitch;
    }
}

internal void
GameUpdateAndRender(game_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
{
  RenderGradient(Buffer, BlueOffset, GreenOffset);
}
