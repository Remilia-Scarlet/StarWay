#include "DangoEngine/precomp.h"
#include "Material.h"
#include "Ash/FileSystem/fs_include.h"
#include "Ash/Container/ArrayStream.h"

const char DEFUALT_MATERIAL_NAME[] = "DefaultMaterial";
const char MATERIAL_FOLDER[] = "game:materials\\";
const char MATERIAL_EXT[] = ".material";

MaterialPtr Material::create(const std::string& fileName)
{
	std::string fullName = std::string(MATERIAL_FOLDER) + (fileName.empty() ? std::string(DEFUALT_MATERIAL_NAME) : fileName) + MATERIAL_EXT;
	File file;
	bool result = file.open(fullName, File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
	TinyAssert(result, "Can't open material file [%s]", fullName.c_str());
	if(result)
	{
		MaterialPtr ptr = MaterialPtr{ new Material() };
		auto data = file.readAll();
		ArrayStream stream;
		stream.initAsInputStream(data.data(), data.size());
	//	boost::archive::binary_iarchive ia(stream);
	//	ia >> *ptr;
		return ptr;
	}
	return {};
}

Material::Material()
{
}
