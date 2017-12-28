#include "precomp.h"
#include "LocalSetting.h"

LocalSetting* LocalSetting::instance()
{
	static LocalSetting instance;
	return &instance;
}

LocalSetting::LocalSetting()
{
}


LocalSetting::~LocalSetting()
{
}
