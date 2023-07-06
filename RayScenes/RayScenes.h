// RayScenes.h : Include file for standard system include files,
// or project specific include files.
#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include "json_struct.h"

#define STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION


#ifndef __MAT__
#define __MAT__
#include "Material.h"
#endif


#ifndef __IMAGE__
#define __IMAGE__
#include "Image.h"
#endif


#ifndef __MATH__
#define __MATH__
#include "Maths.cpp"
#endif





#include "Entity.h"
JS_OBJ_EXT(CameraParser, position,angle,backgroundColor);
JS_OBJ_EXT(EntityParser,type, position,angle,idMaterial,meshPath);
JS_OBJ_EXT(MaterialParser,ambiante, diffuse,specular,si,colorMapPath,normalMapPath);
JS_OBJ_EXT(LightParser,direction,color,specular);
JS_OBJ_EXT(SceneParser, camera, materials,entities,lights);

