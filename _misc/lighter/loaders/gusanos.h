#ifndef GUSANOS_LOADERS_GUSANOS_H
#define GUSANOS_LOADERS_GUSANOS_H

#include "../level.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/fstream.hpp>
namespace fs = boost::filesystem;

struct GusanosLevelLoader
{
	virtual bool canLoad(fs::path const& path, std::string& name);
	
	virtual bool load(Level*, fs::path const& path);
	
	virtual const char* getName();
	
	static GusanosLevelLoader instance;
	
	virtual ~GusanosLevelLoader()
	{
	}
};


#endif //GUSANOS_LOADERS_GUSANOS_H
