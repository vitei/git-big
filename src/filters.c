#include "filters.h"
#include "repo.h"

enum Error filtersInit(void)
{
	git_config_set_string(gRepoConfigHandle,
	                      "filter.big.clean", "git-big filter-clean");
	git_config_set_string(gRepoConfigHandle,
	                      "filter.big.smudge", "git-big filter-smudge");

	// FIXME: need to find a way to make these filters required...

	return kErrorNone;
}
