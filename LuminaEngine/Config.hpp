#ifndef CONFIG_H
#define CONFIG_H

#define NS_BEGIN namespace Lumina {
#define NS_END }

#define DELETECOM(x) {if(x) {x->Release(); x = 0;} }

#define NUMDIRECTIONALLIGHTS 1
#define NUMPOINTLIGHTS		 3
#define NUMSPOTLIGHTS		 3

#define SHADOWMAPRESOLUTION 1024
#define ORTHOGRAPHICVIEWWIDTH  50.0f
#define ORTHOGRAPHICVIEWHEIGHT 50.0f

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>

using namespace DirectX;

#endif