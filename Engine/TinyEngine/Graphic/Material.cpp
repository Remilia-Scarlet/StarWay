#include "TinyEngine\precomp.h"
#include "Material.h"
#include <sstream>
#include "Ash/FileSystem/fs_include.h"

MaterialPtr Material::create(const std::string& fileName)
{
	MaterialPtr ptr = MakeRefCountPtr<Material>();
	File file;
	if(file.open(fileName,File::AccessMode::READ,File::CreateMode::OPEN_EXIST))
	{
		auto data = file.readAll();
		std::istringstream stream;
		stream.str(data.data(), data.size());
		boost::archive::binary_iarchive ia(stream);

	}

}

Material::Material()
{
}
