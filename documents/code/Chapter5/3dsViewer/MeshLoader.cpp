#include "MeshLoader.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
 
namespace MeshLoader {
	bool Load3DS(const std::string& filename, std::vector<C3dsMesh*>& meshes, std::vector<Material*>& materials) {
		ifstream infile(filename, std::ios::in|std::ios::binary);
	
		if(infile.bad())
			return false;

		long beg=0,end=0;
		infile.seekg(0, std::ios::beg); 
		beg = (long)infile.tellg();     
		infile.seekg(0, std::ios::end);
		end = (long)infile.tellg();
		infile.seekg(0, std::ios::beg); 
		long fileSize = (end - beg);

		unsigned short chunk_id;
		unsigned int chunk_length;
		 
		Material* pMaterial=0;
		TextureMap* pCurrentTextureMap=0;
		float* pColorChannel=0;
		float* pPercent=0;
		C3dsMesh* pMesh = 0;

		while(infile.tellg() < fileSize) {
			infile.read(reinterpret_cast<char*>(&chunk_id), 2);
			infile.read(reinterpret_cast<char*>(&chunk_length), 4);  

			switch(chunk_id) {
				case 0x4d4d: break;
				case 0x3d3d: break;
				case 0x4000: {
					std::string name = "";
					char c = ' ';
					while(c!='\0') {
						infile.read(&c,1);
						name.push_back(c);
					} 
					pMesh =new C3dsMesh(name);
					pMesh->pMaterial = pMaterial;
					//pMesh->name = name;
					meshes.push_back(pMesh);
				} break;
				case 0x4100:break;

				case 0x4110: {
					unsigned short total_vertices=0;
					infile.read(reinterpret_cast<char*>(&total_vertices), 2);
					pMesh->vertices.resize(total_vertices);
					infile.read(reinterpret_cast<char*>(&pMesh->vertices[0].x), sizeof(glm::vec3)*total_vertices); 
				}break;

				case 0x4120: {
					unsigned short total_tris=0;
					infile.read(reinterpret_cast<char*>(&total_tris), 2);
					pMesh->faces.resize(total_tris);
					infile.read(reinterpret_cast<char*>(&pMesh->faces[0].a), sizeof(Face)*total_tris);
				}break;

				case 0x4130: {
					std::string name = "";
					char c = ' ';
					while(c!='\0') {
						infile.read(&c,1);
						name.push_back(c);
					} 
					unsigned short total_enteries=0;
					infile.read(reinterpret_cast<char*>(&total_enteries), 2);
				
					//find the material in the materials list
					Material* pMat = 0;
					for(size_t i=0;i<materials.size();i++) {
						if(name.compare(materials[i]->name)==0) {
							pMat = materials[i];
							break;
						}
					}			 

					unsigned short face_id;
					for(size_t i=0;i<total_enteries;i++) {
						infile.read(reinterpret_cast<char*>(&face_id), 2);
						pMat->face_ids.push_back(face_id); 
					}
				} break;

				case 0x4140: {
					unsigned short total_uvs=0;
					infile.read(reinterpret_cast<char*>(&total_uvs), 2);
					pMesh->uvs.resize(total_uvs);
					infile.read(reinterpret_cast<char*>(&pMesh->uvs[0].x), sizeof(glm::vec2)*total_uvs);
				}break;
			
				case 0x4160: {
					float transform[4][3]={0};  
					infile.read(reinterpret_cast<char*>(&transform[0][0]), 12*sizeof(float)); 
					  
					glm::mat4 C = glm::mat4(1,0,0,0,
											0,0,1,0,
											0,-1,0,0,
											0,0,0,1); 
					pMesh->transform = glm::mat4(transform[0][0],transform[0][1],transform[0][2],0,
											     transform[1][0],transform[1][1],transform[1][2],0,
											     transform[2][0],transform[2][1],transform[2][2],0,
												 transform[3][0],transform[3][1],transform[3][2],1)  ;
					  
					for (int j=0;j<4;j++) {
						for (int i=0;i<3;i++)
							cout<<transform[j][i]<<" ";
						cout<<endl;
					}
					cout<<"------------------------"<<endl;
					 

				} break;

				case 0xa200://Texture map 1 
				case 0xa33a://Texture map 2
				case 0xa210://Opacity map
				case 0xa230://Bump map
				case 0xa33c://Shininess map
				case 0xa204://Specular map
				case 0xa33d://Self illum. map
				case 0xa220://Reflection map
				case 0xa33E://Mask for texture map 1
				case 0xa340://Mask for texture map 2
				case 0xa342://Mask for opacity map
				case 0xa344://Mask for bump map
				case 0xa346://Mask for shininess map
				case 0xa348://Mask for specular map
				case 0xa34A://Mask for self illum. map			
				case 0xa34C://Mask for reflection map	
				{
					pMaterial->textureMaps.push_back(new TextureMap());
					pCurrentTextureMap = pMaterial->textureMaps[pMaterial->textureMaps.size()-1]; 
				}
				break; 

				case 0xa300://Mapping filename
				{
					std::string name = "";
					char c = ' ';
					while(c!='\0') {
						infile.read(&c,1);
						name.push_back(c);
					} 
					pCurrentTextureMap->filename = name; 
				}
				break;

				case 0xa351://Mapping parameters 
					infile.seekg(chunk_length-6, std::ios::cur);
				break;

				case 0xa353://Blur percent 
					infile.read(reinterpret_cast<char*>(& pCurrentTextureMap->blur_percent ), sizeof(float)); 
				break;

				case 0xa354://V scale	
					infile.read(reinterpret_cast<char*>(& pCurrentTextureMap->UVscale[1] ), sizeof(float)); 
				break;

				case 0xa356://U scale
					infile.read(reinterpret_cast<char*>(& pCurrentTextureMap->UVscale[0] ), sizeof(float)); 
				break;

				case 0xa358://U offset
					infile.read(reinterpret_cast<char*>(& pCurrentTextureMap->UVoffset[0] ), sizeof(float)); 
				break;

				case 0xa35A://V offset
					infile.read(reinterpret_cast<char*>(& pCurrentTextureMap->UVoffset[1] ), sizeof(float)); 
				break;

				case 0xa35C://Rotation angle
					infile.read(reinterpret_cast<char*>(& pCurrentTextureMap->rotation_angle ), sizeof(float)); 				  
				break;

				case 0xa360://RGB Luma/Alpha tint 1
					infile.read(reinterpret_cast<char*>(& pCurrentTextureMap->rgbLumAlphaTint1[0] ), 3*sizeof(float)); 
				break;

				case 0xa362://RGB Luma/Alpha tint 2
					infile.read(reinterpret_cast<char*>(& pCurrentTextureMap->rgbLumAlphaTint2[0] ), 3*sizeof(float)); 
				break;
 
				case 0xafff: {
					 		
				}break;

				case 0xa000: {
					std::string name = "";
					char c = ' ';
					while(c!='\0') {
						infile.read(&c,1);
						name.push_back(c);
					}
					materials.push_back(new Material(name));
					pMaterial = materials[materials.size()-1];
				}
				break;
 
				case 0xa010: {
					pColorChannel = &pMaterial->ambient[0];			
				}	break;

				case 0xa020: {
					pColorChannel = &pMaterial->diffuse[0];
				}
				break;

				case 0xa030: {
					pColorChannel = &pMaterial->specular[0];
				}
				break;

				case 0xa040: 
					pPercent = &pMaterial->shininess;  
				break;

				case 0xa041: 
					pPercent = &pMaterial->shininess_strength;  
				break; 

				case 0xa050: 
					pPercent = &pMaterial->transparency_percent;  
				break;

				case 0xa052: 
					pPercent = &pMaterial->transparency_falloff;  
				break;

				case 0xa053: 
					pPercent = &pMaterial->reflection_blur_percent;  
				break;
 
				case 0xa084: 
					pPercent = &pMaterial->self_illum;  
				break;   
  
				case 0x0011: 
				{
					unsigned char rgb[3];
					infile.read(reinterpret_cast<char*>(&rgb[0]),3*sizeof(unsigned char)); 
					pColorChannel[0] = rgb[0]/255.0f;
					pColorChannel[1] = rgb[1]/255.0f;
					pColorChannel[2] = rgb[2]/255.0f;  
				} break;

				case 0x0030: {
					unsigned short percent;
					infile.read(reinterpret_cast<char*>(&percent),2); 
					*pPercent = percent;
				} break;
 
				default:
					infile.seekg(chunk_length-6, std::ios::cur);
			}
		}
		infile.close();

		//check if there is any material with 0 size face_ids meaning it is not used then delete it
		//size_t total = materials.size();
		size_t total = meshes.size();
		for(size_t i=0;i<total;i++) {
			C3dsMesh* pMesh = meshes[i];
			Material* pMat  = pMesh->pMaterial;
			if(pMat==0)
				continue;
			if(pMat->face_ids.size()==0) {
				for(size_t j=0;j<materials.size();j++) {
					if(pMat  == materials[j]) {
						materials.erase(materials.begin()+j);
						break;
					}
				}
			} else {
				 
				//if the current material is not applied to the whole mesh
				if(pMat->face_ids.size() != pMesh->faces.size()) {
					//use the material face ids to create the submesh_indices
					for(size_t j=0;j< pMat->face_ids.size();j++) {
						pMat->sub_indices.push_back(pMesh->faces[pMat->face_ids[j]].a);
						pMat->sub_indices.push_back(pMesh->faces[pMat->face_ids[j]].b);
						pMat->sub_indices.push_back(pMesh->faces[pMat->face_ids[j]].c);
					}
				}
			}
		} 
		  
		return true;
	}
}
