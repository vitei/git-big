#include "init.h"
#include "../repo.h"

int filtersInit(void)
{
	git_config_set_string(gRepoConfigHandle,
	                      "filter.media.clean", "git-big filter-clean");
	git_config_set_string(gRepoConfigHandle,
	                      "filter.media.smudge", "git-big filter-smudge");

	// FIXME: need to find a way to make these filters required...

	return kErrorNone;
}
